#include "common-lib/IPC.hpp"

int main(int argc, char* argv[]) {
    One2OneSender<CHAR255, Send> sender("channel1");
    int num;
    std::cin >> num;
    sender.get_send_buffer().write_buffer("Hello");
    sender.send();

    std::cin >> num;
    sender.get_send_buffer().write_buffer("World");
    sender.send();

    std::cin >> num;
    sender.get_send_buffer().write_buffer("98!!!");
    sender.send();


    return 0;
}