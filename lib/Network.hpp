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

#define forever for(int iteration=0;;iteration=(iteration+1)%2147489647)
#define PROC_NET_ROUTE_LINE_SIZE 128
#define CONECTION_RETRIES 30

struct PacketHeader{
	unsigned int messageSize;													// Full Message Size 	0-4
	unsigned int from;															// Sender				5-8
	unsigned int to;															// Receiver				9-12
	unsigned short portFrom;													// Sender port			13-14
	unsigned short portTo;														// Receiver port		15-16
	unsigned short dataSize;													// 0 - MAX_DATA_SIZE	17-18
	unsigned short frameNum;													// 0|1 stop and wait	18-20
	PacketHeader():frameNum(-1){}
	PacketHeader(unsigned int ip, unsigned short port):frameNum(-1){to = ip; portTo = port;}
	PacketHeader(unsigned int ip, unsigned short port, bool inverse):frameNum(-1){from = ip; portFrom = port;}
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
		 * sendMessage
		 * 
		 * EFE: Sends a fragmented message to the given address.
		 * REQ: header: the receiver address.
		 * 		message: the message to be send.
		 */
		void sendMessage(PacketHeader header, const char * message);

		/**
		 * receiveMessage
		 * 
		 * EFE: Receives fragments of a message from the given address and rebuilds it.
		 * REQ: header: The sender address.
		 * REQ: The builded message.
		 */
		std::string receiveMessage(PacketHeader header);
						
		
	private:
		Socket socket;
		Translator translator;

		// Machine ID
		unsigned int ip;
		unsigned short port;
		
		double reliability;


		bool exit;																				// End of program control flag
		int connectionLost;																		// Connection retry counter

		std::thread receiver;																	// Infinite packet receiving loop(until end)
		std::mutex sendingPacket;																// 
		std::mutex acknowledging;																// 
		std::mutex receiving;																	// 
		
		std::map<unsigned int,std::map<unsigned short,std::vector<Packet>>> receivedPackets;
// ----------------------------------------------------------------------------------------------------------------------

		void send(PacketHeader header, const char * data);				// Packet send
		
		/**
		 * writeHandler
		 * 
		 * EFE:
		 */
		void writeHandler();	

		// Manages timeout
		void runClock(bool &timeout, bool &acknowledged);

		void receiveACK(PacketHeader header, bool &timeout, bool &acknowledged);

		void sendDone();						

// ----------------------------------------------------------------------------------------------------------------------

		std::string receive(PacketHeader &header);								// Packet pickup

		/**
		 * readHandler
		 * 
		 * EFE:	One packet lecture at time from a especific sender
		 * REQ: header: sender id 
		 */
		void readHandler(PacketHeader header);	
		
// ----------------------------------------------------------------------------------------------------------------------
		/**
		 * Receives any packet and stores it on a vector inside a multimap
		 */
		void checkPacketAvailable();									// Packet receive

		/**
		 * sendACK
		 * 
		 * EFE: Constructs a packet as an ack packet, and sends it
		 */
		void sendACK(PacketHeader header);

		

// ----------------------------------------------------------------------------------------------------------------------

		/**
		 * byteArrayToPacket
		 * 
		 * EFE: Translates a byte array into a packet.
		 */
		Packet byteArrayToPacket(const unsigned char * bytes);

// ----------------------------------------------------------------------------------------------------------------------

		/**
		 * getLocalIp
		 * 
		 * EFE: Gets this machine local ip
		 * REQ: ip: A buffer where to store the local ip.
		 * 		family: AF_INET for ipv4 | AF_INET6 for ipv6
		 * RET: Your local ip.
		 */ 
		std::string getLocalIp(int family = AF_INET);

		/**
		 * getDefaultInterface
		 * 
		 * EFE: Gets this machine default interface from "/proc/net/route"
		 * REQ: interface: A buffer where to store the default interface.
		 * RET: The default interface on the given buffer.
		 */
		std::string getDefaultInterface();

};

#endif /* NETWORK_HPP */
