#ifndef __COMMON_LIB_IPC__
#define __COMMON_LIB_IPC__

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <cstring>
#include <system_error>
#include <stdexcept>

template <typename T>
class One2One {
protected:
    template <typename U>
    struct InnerType {
        pthread_mutex_t mutex;
        pthread_cond_t cond;
        bool data_available;
        bool initialized;
        U data;
    };
    std::string mmName;
    int mmFD = -1;
    InnerType<T>* mmData = nullptr;

    void create_channel(const std::string& channel_name) {
        mmName = channel_name;

        mmFD = shm_open(channel_name.c_str(), O_CREAT | O_RDWR | O_EXCL, 0666);
        if (mmFD == -1) {
            if (errno == EEXIST) {
                throw std::runtime_error("Channel already exists");
            } else {
                throw std::system_error(errno, std::generic_category());
            }
        }

        if (ftruncate(mmFD, sizeof(InnerType<T>)) == -1) {
            close(mmFD);
            throw std::system_error(errno, std::generic_category());
        }
        
        mmData = static_cast<InnerType<T>*>(mmap(
            nullptr, sizeof(InnerType<T>), PROT_READ | PROT_WRITE, MAP_SHARED, mmFD, 0));
        if (mmData == MAP_FAILED) {
            close(mmFD);
            mmFD = -1;
            throw std::system_error(errno, std::generic_category());
        }
    
        pthread_mutexattr_t mutex_attr;
        pthread_mutexattr_init(&mutex_attr);
        pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
        pthread_mutex_init(&mmData->mutex, &mutex_attr);
    
        pthread_condattr_t cond_attr;
        pthread_condattr_init(&cond_attr);
        pthread_condattr_setpshared(&cond_attr, PTHREAD_PROCESS_SHARED);
        pthread_cond_init(&mmData->cond, &cond_attr);

        pthread_mutex_lock(&mmData->mutex);
        mmData->data_available = false;
        mmData->initialized = true;
        pthread_cond_broadcast(&mmData->cond);
        pthread_mutex_unlock(&mmData->mutex);
    }

    void connect_channel(const std::string& channel_name) {
        mmName = channel_name;
        const int max_retries = 100; // 10 seconds with 100ms interval
        int retries = 0;

        while (true) {
            mmFD = shm_open(channel_name.c_str(), O_RDWR, 0666);
            if (mmFD != -1) break;
            if (errno != ENOENT) {
                throw std::system_error(errno, std::generic_category());
            }
            if (retries++ >= max_retries) {
                throw std::runtime_error("Channel does not exist after retries");
            }
            usleep(100000); // 100ms
        }

        mmData = static_cast<InnerType<T>*>(mmap(
            nullptr, sizeof(InnerType<T>), PROT_READ | PROT_WRITE, MAP_SHARED, mmFD, 0));
        if (mmData == MAP_FAILED) {
            close(mmFD);
            mmFD = -1;
            throw std::system_error(errno, std::generic_category());
        }

        pthread_mutex_lock(&mmData->mutex);
        while (!mmData->initialized) {
            pthread_cond_wait(&mmData->cond, &mmData->mutex);
        }
        pthread_mutex_unlock(&mmData->mutex);
    }

    void destroy_channel() {
        if (mmData) {
            munmap(mmData, sizeof(InnerType<T>));
            mmData = nullptr;
        }
        if (mmFD != -1) {
            close(mmFD);
            mmFD = -1;
        }
        shm_unlink(mmName.c_str());
    }

    void disconnect_channel() {
        if (mmData) {
            munmap(mmData, sizeof(InnerType<T>));
            mmData = nullptr;
        }
        if (mmFD != -1) {
            close(mmFD);
            mmFD = -1;
        }
    }

    T& shared_buff() {
        return mmData->data;
    }
    
    One2One() = default; 
    One2One(const One2One&) = delete; 
    One2One& operator=(const One2One&) = delete; 
};

template <typename T, template <typename> class BUFF>
class One2OneSender : public One2One<T> {
private:
    BUFF<T> buff;
public:
    One2OneSender(const std::string& channel_name) {
        this->create_channel(channel_name);
    }
    ~One2OneSender() {
        this->destroy_channel();
    }
    void send() {
        pthread_mutex_lock(&this->mmData->mutex);
        this->shared_buff().copy(buff.get_buffer());
        this->mmData->data_available = true;
        pthread_cond_signal(&this->mmData->cond);
        pthread_mutex_unlock(&this->mmData->mutex);
    }
    BUFF<T>& get_send_buffer() {
        return buff;
    }
};

template <typename T, template <typename> class BUFF>
class One2OneReceiver : public One2One<T> {
private:
    BUFF<T> buff;
public:
    One2OneReceiver(const std::string& channel_name) {
        this->connect_channel(channel_name);
    }
    ~One2OneReceiver() {
        this->disconnect_channel();
    }
    void recv() {
        pthread_mutex_lock(&this->mmData->mutex);
        while (!this->mmData->data_available) {
            pthread_cond_wait(&this->mmData->cond, &this->mmData->mutex);
        }
        buff.get_buffer().copy(this->shared_buff());
        this->mmData->data_available = false;
        pthread_mutex_unlock(&this->mmData->mutex);
    }
    BUFF<T>& get_recv_buffer() {
        return buff;
    }
};

template <typename T, template<typename> class SendBuff = Send, template<typename> class RecvBuff = Recv>
class TwoWayChannel {
private:
    One2OneSender<T, SendBuff> sender_;
    One2OneReceiver<T, RecvBuff> receiver_;

    static std::string get_send_channel(const std::string& base, ChannelRole role) {
        return base + (role == ChannelRole::First ? "_AB" : "_BA");
    }

    static std::string get_recv_channel(const std::string& base, ChannelRole role) {
        return base + (role == ChannelRole::First ? "_BA" : "_AB");
    }

public:
    TwoWayChannel(const std::string& base_name, ChannelRole role)
        : sender_(get_send_channel(base_name, role)),
          receiver_(get_recv_channel(base_name, role)) {}

    void send() {
        sender_.send();
    }

    void recv() {
        receiver_.recv();
    }

    auto& get_send_buffer() {
        return sender_.get_send_buffer();
    }

    auto& get_recv_buffer() {
        return receiver_.get_recv_buffer();
    }
};

template <typename T>
class message {
private:
    T data;
public:
    typedef T data_type;
    void write(const data_type& src) {
        data = src;
    }
    void copy(const T& src) {
        data = src;
    }
    data_type read() {
        return data;
    }
};

template <>
class message<char[255]> {
private:
    char data[255];
public:
    typedef char* data_type;
    void write(const data_type src) {
        strncpy(data, src, sizeof(data)-1);
        data[sizeof(data)-1] = '\0';
    }
    data_type read() {
        return data;
    }
    void copy(const message<char[255]>& src) {
        strncpy(data, src.data, sizeof(data)-1);
        data[sizeof(data)-1] = '\0';
    }
};

using CHAR255 = message<char[255]>;

template <typename T>
class Send {
private:
    T buff;
public:
    void write_buffer(typename T::data_type data) {
        buff.write(data);
    }
    T& get_buffer() {
        return buff;
    }
};

template <typename T>
class Recv {
private:
    T buff;
public:
    typename T::data_type read_buffer() {
        return buff.read();
    }
    T& get_buffer() {
        return buff;
    }
};

#endif