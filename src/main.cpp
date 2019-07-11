#include "../lib/Time.hpp"
#include "../lib/Network.hpp"

#include <sys/wait.h>

int main(int argc, char**argv){
    Network n(1101,1);
    char buffer[1024];
    memset(buffer,'\0',1024);
    forever{
        PacketHeader rec;
        rec = n.receiveMessage(buffer);
        std::cout<<iteration+1<<":"<<buffer<<std::endl;
        // PacketHeader header(rec.from,rec.portFrom);
        // n.sendMessage(header,"hello from server.");
    }
    return EXIT_SUCCESS;
}
