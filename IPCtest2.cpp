#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

struct SharedData {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int sequence;         // 消息序列号
    char buffer[256];
};

int main(int argc, char* argv[]) {
    const char* shm_name = "/broadcast_shm";
    int fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, sizeof(SharedData));
    SharedData* shared_data = static_cast<SharedData*>(mmap(nullptr, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));

    // 初始化互斥锁和条件变量
    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&shared_data->mutex, &mutex_attr);

    pthread_condattr_t cond_attr;
    pthread_condattr_init(&cond_attr);
    pthread_condattr_setpshared(&cond_attr, PTHREAD_PROCESS_SHARED);
    pthread_cond_init(&shared_data->cond, &cond_attr);

    static thread_local int last_sequence = -1; // 消费者本地序列号

    if (argc > 1 && strcmp(argv[1], "writer") == 0) {
        // 生产者广播消息
        std::cout << "Broadcaster started." << std::endl;
        pthread_mutex_lock(&shared_data->mutex);
        strcpy(shared_data->buffer, "Broadcast Message!");
        shared_data->sequence++;
        pthread_cond_broadcast(&shared_data->cond); // 广播通知所有消费者
        pthread_mutex_unlock(&shared_data->mutex);
    } else {
        // 消费者等待新消息
        std::cout << "Consumer started. PID: " << getpid() << std::endl;
        pthread_mutex_lock(&shared_data->mutex);
        while (shared_data->sequence == last_sequence) {
            pthread_cond_wait(&shared_data->cond, &shared_data->mutex);
        }
        std::cout << "Consumer " << getpid() << " received: " << shared_data->buffer 
                  << " (seq=" << shared_data->sequence << ")" << std::endl;
        last_sequence = shared_data->sequence;
        pthread_mutex_unlock(&shared_data->mutex);
    }

    // 清理资源
    munmap(shared_data, sizeof(SharedData));
    close(fd);
    if (argc > 1) shm_unlink(shm_name);
    return 0;
}