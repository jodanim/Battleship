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

unsigned int byteArrayToInt(const unsigned char * array){
    unsigned int first = array[0];
    unsigned int second = array[1]<<8;
    unsigned int third = array[2]<<16;
    unsigned int fourth = array[3]<<24;
    return first+second+third+fourth;
}

Packet stringToPacket(std::string bytes){
    unsigned char buffer[MAXWIRESIZE];
    for(int i=0;i<MAXWIRESIZE;i++)buffer[i] = bytes.c_str()[i]&0xFF;
    Packet packet;
    packet.header.to = byteArrayToInt(buffer);
    packet.header.from = byteArrayToInt(buffer+4);
    packet.header.dataSize = byteArrayToInt(buffer+8);
    packet.data = std::string((char*)buffer+12);
    return packet;
}

int main(int argc, char**argv){
    Packet packet;
    packet.header.from = 4294967240; // 255.255.255.200
    packet.header.to = 0;
    packet.data = "1ensajeMensajeMensajeMensajeMensajeMensajeMensajeMe3"; // revisar overflow
    packet.header.dataSize = packet.data.size()+1;

    std::cout<<packet.header.to<<", "<<packet.header.from<<","<<packet.header.dataSize<<packet.data<<std::endl;
    unsigned char buffer[MAXWIRESIZE];
    std::string a = packet.getBytes();
    
    for(int i=0;i<MAXWIRESIZE;i++)
		printf("%02X ",(char)a.c_str()[i]&0x000000FF);
	printf("\n");
    stringToPacket(a);
    // std::cout<<stringToPacket(a).header.to<<std::endl;
    // std::cout<<stringToPacket(a).header.from<<std::endl;
    // std::cout<<stringToPacket(a).header.dataSize<<std::endl;
    std::cout<<stringToPacket(a).data<<std::endl;

    return EXIT_SUCCESS;
}

