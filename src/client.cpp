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
    Network n(1102);
    PacketHeader header("192.168.0.16",1101);
    n.send(header,"123456789-123456789-123456789-123456789-123456789-123456789-");
    return EXIT_SUCCESS;
}