#include "../lib/Time.hpp"
#include "../lib/Socket.hpp"
#include "../lib/FileManager.hpp"
#include "../lib/Network.hpp"
#include "../lib/IPC/Mailbox.hpp"

// #include <stdio.h>
// #include <string.h>
// #include <errno.h>	//errno
// #include <sys/socket.h>
// #include <netdb.h>
// #include <ifaddrs.h>
// #include <stdlib.h>
// #include <unistd.h>

// #include <cassert>

#include <sys/wait.h>
#include <thread>

int main(int argc, char**argv){
    Network n;
    // std::thread t1(&Network::stringIptoIntIp,a,"0.0.00");
    // t1.join();
    // std::thread t2(&Network::stringIptoIntIp,a,"0.0..0");
    // t2.join();
    // std::thread t3(&Network::stringIptoIntIp,a,"0.0.0");

    // t3.join();

    return EXIT_SUCCESS;
}

