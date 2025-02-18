#include "common-lib/IPC.hpp"

int main(int argc, char* argv[]) {
    One2OneReceiver<CHAR255, Recv> receiver("channel1");
    while (1) {
        receiver.recv();
        std::cout << receiver.get_recv_buffer().read_buffer() << std::endl;
    }

    return 0;
}