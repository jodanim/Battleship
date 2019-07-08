#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "FileManager.hpp"
#include "MessageHandler.hpp"
#include "Socket.hpp"

#include <netdb.h>
#include <ifaddrs.h>

#include <cstdio>

// #include <algorithm>
#include <vector>
#include <thread>

#define PROC_NET_ROUTE_LINE_SIZE 128
#define NETWORK_INTERFACE_MAX_LENGTH 15
#define IP_MAX_SIZE 15

struct PacketHeader{
	unsigned int from;															// Sender
	unsigned int to;															// Receiver
	// unsigned short port;														// Receiver port
	unsigned int dataSize;													// value between 0 and 52
};

const int MAXWIRESIZE = 64;
const int MAXDATASIZE = MAXWIRESIZE - sizeof(PacketHeader);

struct Packet{
	PacketHeader header;
	char data[MAXDATASIZE];								
};

class Network{
	public:
		Network(int Port, double reliability = 1);
		~Network();

		void send(const char * ip, int port, const char * data);
		void* sendDone();
		void* checkPacketAvailable();

		PacketHeader receive(char * data);
	private:
		FileManager fileManager;

		unsigned int ip;														// This machine id.
		unsigned short port;
		Socket socket;

		bool sendingPacket;
		bool packetAvailable;
		bool exit;
		double reliability;
		
		int packetsSent;

		std::vector<Packet> receivedPackets;
		std::thread receiver;

		void readHandler();
		void writeHandler();
		void intIptoCharArrayIp(unsigned int ip, char * array);	
		// std::string intIptoStringIp(unsigned int ip);							// Ip converisón from int to string.

		unsigned int constCharIptoIntIp(const char * ip);							// Ip converisón from string to int.	
		// unsigned int stringIptoIntIp(std::string ip);							// Ip converisón from string to int.
		
		Packet byteArrayToPacket(const unsigned char * bytes);
		unsigned int byteArrayToInt(const unsigned char * array);
		void getDefaultInterface(char * interface);
		void getLocalIp(char * ip, int family = AF_INET);
};


#endif /* NETWORK_HPP */