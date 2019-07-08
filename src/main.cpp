#include "../lib/Network/Time.hpp"
#include "../lib/Network/Network.hpp"

#include <sys/wait.h>

int main(int argc, char**argv){
    Network n(1101);
    while(true){
        char buffer[MAXDATASIZE];
        n.receive(buffer);
        std::cout<<buffer<<std::endl;
    }
    return EXIT_SUCCESS;
}
