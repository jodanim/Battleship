#include "../lib/Time.hpp"
#include "../lib/Socket.hpp"
#include "../lib/FileManager.hpp"
#include "../lib/Network.hpp"
#include "../lib/IPC/Mailbox.hpp"

#include <sys/wait.h>
#include <thread>

// void* read(void* args) {
//     Socket s = *(Socket *)args;
//     for(int i = 1;;i++){
//         char buffer[MAXWIRESIZE];
//         s.Read(buffer,MAXWIRESIZE);  
//         printf("%s\n",buffer);
//         s.Write("hello to you again",MAXWIRESIZE);
//     }
// }

// int main(int argc, char**argv){
// 	Socket s;
//     s.Bind(1101);
//     pthread_t sender;
//     int rc = pthread_create(&sender, NULL, read, (void*)&s);
//     if (rc) {
//         std::cout << "Couldn't create thread" << std::endl;
//         exit(-1);
//     }
//     pthread_join(sender,NULL);
//     return EXIT_SUCCESS;
// }


int main(int argc, char**argv){
    Network n(1101);
    while(true){
        char buffer[MAXDATASIZE];
        n.receive(buffer);
        std::cout<<buffer<<std::endl;
    }
    // std::cout<<n.receive();
    // n.receive();
    return EXIT_SUCCESS;
}
