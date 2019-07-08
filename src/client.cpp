#include "../lib/Network/Network.hpp"

#include <sys/wait.h>
#include <thread>

int main(int argc, char**argv){
    Network n(1102,1);
    PacketHeader header("192.168.0.16",1101);
    n.send(header,"123456789-123456789-123456789-123456789");
    return EXIT_SUCCESS;
}