#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "FileManager.hpp"
#include "MessageHandler.hpp"
#include "Socket.hpp"
#include "Translator.hpp"

#include <netdb.h>
#include <ifaddrs.h>

#include <vector>
#include <map>
#include <thread>
#include <mutex>

bool condition = true;
#define forever for(int iteration=0;condition;iteration=(iteration+1)%2147489647)
#define PROC_NET_ROUTE_LINE_SIZE 128
#define NETWORK_INTERFACE_MAX_LENGTH 15
#define IP_MAX_SIZE 15
#define CONECTION_RETRIES 50

struct PacketHeader{
	unsigned int messageSize;													// Full Message Size
	unsigned int from;															// Sender
	unsigned int to;															// Receiver
	unsigned short portFrom;													// Sender port
	unsigned short portTo;														// Receiver port
	unsigned short id;															// fragment id
	unsigned char dataSize;														// 0 - MAX_DATA_SIZE
	char frameNum;																// 0|1 stop and wait
	PacketHeader():frameNum(-1){}
	PacketHeader(unsigned int ip, unsigned short port):frameNum(-1){to = ip; portTo = port;}
	PacketHeader(const char * ip, unsigned short port):frameNum(-1){
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
		/**
		 * Constructor
		 * 
		 * EFE: Initializes the variables, bind the socket port and starts the infinite packet available checker
		 * REQ: Port: Where to bind the socket.
		 * 		Reliablity: the probability of socket failure.
		 */ 
		Network(int Port, double reliability);

		/**
		 * Destructor
		 * 
		 * EFE: Terminates the infinite packet available checker.
		 */ 
		~Network();
		
		/**
		 * sendACK
		 * 
		 * EFE: Constructs a packet as an ack packet, and sends it
		 */
		void sendACK(PacketHeader header);

		void receiveACK(PacketHeader header, bool &timeout, bool &acknowledged);
		
		// Manages timeout
		void runClock(bool &timeout, bool &acknowledged);

		/**
		 * 
		 */
		void sendMessage(PacketHeader header, const char * message);
		
		// Defragmentation.
		PacketHeader receiveMessage(char * message);



		void send(PacketHeader header, const char * data);				// Packet send
		
		void sendDone();												
		void checkPacketAvailable();									// Packet receive
		PacketHeader receive(char * data, PacketHeader header);								// Packet pickup
		
	private:
		// Machine ID
		unsigned int ip;
		unsigned short port;

		// Control Flags

		bool firstPacketAvailable;
		bool exit;
		int connectionLost;

		Socket socket;
		std::mutex sendingPacket;
		std::mutex acknowledge;
		std::mutex receiving;
		
		std::thread receiver;
		Translator translator;
		std::vector<Packet> firstPacket;
		std::map<unsigned int,std::map<unsigned short,std::vector<Packet>>> receivedPackets;
		std::map<unsigned int,std::map<unsigned short,bool>> incommingMessage;
		std::map<unsigned int,std::map<unsigned short,char>> receivedACK;
		std::map<unsigned int,std::map<unsigned short,char>> lastSendedACK;

		std::vector<Packet> fragments;
		
		double reliability;
		

		/**
		 * readHandler
		 * 
		 * EFE:	One packet lecture at time from a especific sender
		 * REQ: header: sender id 
		 */
		void readHandler(PacketHeader header);												

		/**
		 * writeHandler
		 * 
		 * EFE:Writes one packet at time
		 */
		void writeHandler();	

		/**
		 * byteArrayToPacket
		 * 
		 * EFE: Translates a byte array into a packet.
		 */
		Packet byteArrayToPacket(const unsigned char * bytes);
		/**
		 * getLocalIp
		 * 
		 * EFE: Gets this machine local ip
		 * REQ: ip: A buffer where to store the local ip.
		 * 		family: AF_INET for ipv4 | AF_INET6 for ipv6
		 * RET: Your local ip.
		 */ 
		void getLocalIp(char * ip, int family = AF_INET);

		/**
		 * getDefaultInterface
		 * 
		 * EFE: Gets this machine default interface from "/proc/net/route"
		 * REQ: interface: A buffer where to store the default interface.
		 * RET: The default interface on the given buffer.
		 */
		void getDefaultInterface(char * interface);
};

#endif /* NETWORK_HPP */
