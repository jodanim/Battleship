#include "../lib/Battleship.hpp"

#include <sys/wait.h>

void printCell(int state){

}

int main(int argc, char**argv){
    // Network n(1101,1);
    // char buffer[1024];
    // memset(buffer,'\0',1024);
    // forever{
    //     PacketHeader rec = n.receiveMessage(buffer);
    //     std::cout<<iteration+1<<":"<<buffer<<std::endl;
    //     PacketHeader header(rec.from,rec.portFrom);
    //     std::string message = "Hello from server ";
    //     message += std::to_string(iteration+1);
    //     n.sendMessage(header,message.c_str());

    // }
    
    Battleship b("192.168.0.16",1101,1102);
    return EXIT_SUCCESS;
}
