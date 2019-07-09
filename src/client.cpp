#include "../lib/Network.hpp"

#include <sys/wait.h>
#include <thread>

int main(int argc, char**argv){
    Network n(1102,1);
    PacketHeader header("192.168.0.16",1101);
    
    for(int i = 0; ;i++){
        n.sendMessage(header,"1------------------------------------------\n2------------------------------------------\n3------------------------------------------\n4------------------------------------------\n");
        char buffer[MAX_DATA_SIZE];
        n.receiveMessage(buffer);
        std::cout<<i<<":"<<buffer<<std::endl;
    }
    return EXIT_SUCCESS;
}