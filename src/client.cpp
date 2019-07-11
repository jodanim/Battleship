#include "../lib/Network.hpp"
#include <string>

int main(int argc, char**argv){
    Network n(1102,1);
    PacketHeader header("192.168.0.16",1101);
    
    for(int i = 1;i<=1000 ;i++){
        std::cout<<i<<":\n";
        n.sendMessage(header,"123456789-");
        // char buffer[MAX_DATA_SIZE];
        // n.receive(buffer);
        
        // std::cout<<"\n\033[36m----------------------------------------------------\033[0m\n"<<std::endl;

    }
    // std::cout<<"\n\033[35m----------------------------------------------------\033[0m\n"<<std::endl;
    return EXIT_SUCCESS;
}