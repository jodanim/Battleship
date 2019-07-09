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
#include <mutex>

#define PROC_NET_ROUTE_LINE_SIZE 128
#define NETWORK_INTERFACE_MAX_LENGTH 15
#define IP_MAX_SIZE 15

struct PacketHeader{
	unsigned int messageSize;													// Full Message Size
	unsigned int from;															// Sender
	unsigned int to;															// Receiver
	unsigned short portFrom;													// Sender port
	unsigned short portTo;														// Receiver port
	unsigned short id;															// 0-65535
	unsigned char dataSize;														// 0-44
	unsigned char FrameNum;														// 0|1
	PacketHeader(){}
	PacketHeader(unsigned int ip, unsigned short port){to = ip; portTo = port;}
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
		
		void sendMessage(PacketHeader header, const char * message);	// Fragmentation
		void receiveMessage(char * message);							// Defragmentation
		
		void send(PacketHeader header, const char * data);				// Packet send
		void* sendDone();												// FlagChanger

		void* checkPacketAvailable();									// Packet receive
		PacketHeader receive(char * data);								// Packet pickup

	private:
		// Machine ID
		unsigned int ip;
		unsigned short port;

		// Control Flags
		bool sendingPacket;
		bool packetAvailable;
		bool exit;

		Socket socket;
		std::mutex receiving;
		std::thread receiver;
		Translator translator;
		std::vector<Packet> receivedPackets;

		std::vector<Packet> fragments;
		
		double reliability;
		


		void readHandler();												// one packet lecture at time
		void writeHandler();											// one packet write at time
		Packet byteArrayToPacket(const unsigned char * bytes);			// gets an array of bytes and transforms it into a packet
		void getLocalIp(char * ip, int family = AF_INET);				// gets this machine local ip
		void getDefaultInterface(char * interface);						// gets this machine default interface from "/proc/net/route"
};

#endif /* NETWORK_HPP */
