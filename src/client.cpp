#include "../lib/Battleship.hpp"
#include <string>

int main(int argc, char**argv){
    // Network n(1102,1);
    // PacketHeader header("192.168.0.16",1101);
    // char buffer[1024];
    // memset(buffer,'\0',1024);
    // for(int i = 1;i<=10000 ;i++){
        
    //     std::string message = std::to_string(i);
    //     message += "123456789-123456789-123456789-123456789-";
    //     n.sendMessage(header,message.c_str());
    //     char buffer[1024];
    //     n.receiveMessage(buffer);
    //     std::cout<<i<<":"<<buffer<<"\n";
    // }
    Battleship b("192.168.0.16",1102,1101);
    return EXIT_SUCCESS;
}