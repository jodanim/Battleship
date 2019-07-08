#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "FileManager.hpp"
#include "MessageHandler.hpp"
#include "Socket.hpp"
#include "Translator.hpp"

#include <netdb.h>
#include <ifaddrs.h>

#include <vector>
#include <thread>

#define PROC_NET_ROUTE_LINE_SIZE 128
#define NETWORK_INTERFACE_MAX_LENGTH 15
#define IP_MAX_SIZE 15

struct PacketHeader{
	unsigned int from;															// Sender
	unsigned int to;															// Receiver
	unsigned short port;														// Receiver port
	unsigned short dataSize;													// value between 0 and 52
	PacketHeader(){}
	PacketHeader(unsigned int ip, unsigned short p){to = ip;port = p;}
	PacketHeader(const char * ip, unsigned short p){
		Translator translator;
		to = translator.constCharIptoIntIp(ip);
		port = p;
	}
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

		void send(PacketHeader header, const char * data);
		void* sendDone();
		
		void* checkPacketAvailable();
		PacketHeader receive(char * data);
	private:
		// Machine ID
		unsigned int ip;
		unsigned short port;

		// Control Flags
		bool sendingPacket;
		bool packetAvailable;
		bool exit;

		Socket socket;
		std::thread receiver;
		Translator translator;
		std::vector<Packet> receivedPackets;
		
		double reliability;
		int packetsSent;
		
		void readHandler();
		void writeHandler();
		Packet byteArrayToPacket(const unsigned char * bytes);
		void getLocalIp(char * ip, int family = AF_INET);
		void getDefaultInterface(char * interface);
};

#endif /* NETWORK_HPP */