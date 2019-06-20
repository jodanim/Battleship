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
	unsigned char size;															// value between 0 and 56 defining the size on bytes of the rest of the package
};

struct Packet{
	PacketHeader header;														
	std::string data;															
};

const int MAXWIRESIZE = 64;
const int MAXDATASIZE = MAXWIRESIZE - sizeof(PacketHeader);

class Network{
	public:
		Network(int Port = 1101, double reliability = 1);
		~Network();

		void send(std::string ip, std::string data);
		Packet receive();

		void sendDone();
		void checkPacketAvailable();


	private:
		unsigned int ip;														// This machine id.
		Socket socket;													


		bool sendingPacket;
		bool packetAvailable;
		double reliability;

		std::string intIptoStringIp(unsigned int ip);							// Ip converisón from int to string.
		unsigned int stringIptoIntIp(std::string ip);							// Ip converisón from string to int.
		
		FileManager fileManager;
		std::string getDefaultInterface();
		std::string getLocalIp(int family = AF_INET);

		std::vector<Packet> receivedPackets;
};

#endif /* NETWORK_HPP */