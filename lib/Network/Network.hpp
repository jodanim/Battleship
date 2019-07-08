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
	unsigned short portFrom;													// Sender port
	unsigned short portTo;														// Receiver port
	unsigned short id;															// Fragment id
	unsigned char dataSize;														// 0-48
	PacketHeader(){}
	PacketHeader(unsigned int ip, unsigned short port):id(0){to = ip;portTo = port;}
	PacketHeader(const char * ip, unsigned short port){
		Translator translator;
		to = translator.constCharIptoIntIp(ip);
		portTo = port;
	}
};

const int MAX_WIRE_SIZE = 64;
const int MAX_DATA_SIZE = MAX_WIRE_SIZE - sizeof(PacketHeader);

struct Packet{
	PacketHeader header;
	char data[MAX_DATA_SIZE];
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