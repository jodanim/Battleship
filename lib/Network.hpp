#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "FileManager.hpp"
#include "MessageHandler.hpp"
#include "Socket.hpp"

#include <cstring>

#include <netdb.h>
#include <ifaddrs.h>

#include <algorithm>
#include <string>
#include <vector>
#include <thread>



struct PacketHeader{
	unsigned int to;															// Receiver
	unsigned int from;															// Sender
	unsigned int dataSize;														// value between 0 and 52
};

const int MAXWIRESIZE = 64;
const int MAXDATASIZE = MAXWIRESIZE - sizeof(PacketHeader);

struct Packet{
	PacketHeader header;														
	std::string data;		
	std::string getBytes(){
		unsigned char buffer[MAXWIRESIZE];
		memcpy(buffer,(const unsigned char*)&header,MAXWIRESIZE-MAXDATASIZE);
		memcpy(buffer+12,(const unsigned char*)data.c_str(),MAXDATASIZE);
		for(int i=0;i<MAXWIRESIZE;i++)
			printf("%02X ",buffer[i]);
		printf("\n");
		
		std::cout<<std::endl<<"salida: "<<std::string((char*)buffer+12)<<std::endl<<std::endl;
		

		
		return std::string((const char *)buffer,MAXWIRESIZE-MAXDATASIZE+header.dataSize);
	};												
};

class Network{
	public:
		Network(int Port = 1101, double reliability = 1);
		~Network();

		void send(std::string ip, std::string data);
		Packet receive();

		void sendDone();
		void checkPacketAvailable();


	private:
		std::string intIptoStringIp(unsigned int ip);							// Ip converisón from int to string.
		unsigned int stringIptoIntIp(std::string ip);							// Ip converisón from string to int.
		void packetToByteArray(Packet packet,unsigned char * array);
		Packet byteArrayToPacket(const unsigned char * array);
		unsigned int byteArrayToInt(const unsigned char * array);
		std::string getDefaultInterface();
		std::string getLocalIp(int family = AF_INET);

		FileManager fileManager;

		unsigned int ip;														// This machine id.
		Socket * socket;

		bool sendingPacket;
		bool packetAvailable;
		double reliability;

		std::vector<Packet> receivedPackets;
};

#endif /* NETWORK_HPP */