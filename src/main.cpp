#include "../lib/Network/Time.hpp"
#include "../lib/Network/Network.hpp"

#include <sys/wait.h>

int main(int argc, char**argv){
    Network n(1101);
    for(int i = 0; ;i++){
        char buffer[MAX_DATA_SIZE];
        PacketHeader rec = n.receive(buffer);
        std::cout<<i<<":"<<buffer<<std::endl;
        PacketHeader header(rec.from,rec.portFrom);
        n.send(header,"hello from server.");
    }
    return EXIT_SUCCESS;
}
