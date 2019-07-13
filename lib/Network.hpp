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
		 * 		message: the message to be sent.
		 */
		void sendMessage(PacketHeader header, const char * message);

		/**
		 * receiveMessage
		 * 
		 * EFE: Receives fragments of a message from the given address and rebuilds it.
		 * REQ: header: The sender address.
		 * RET: The builded message.
		 */
		std::string receiveMessage(PacketHeader header);
						
		
	private:
		Socket socket;
		Translator translator;

		// Machine ID
		unsigned int ip;																		// Your IP
		unsigned short port;																	// Your connection port
		
		double reliability;																		// The message reliability to be send
		int failcounter;																		// a counter to be printed when the message fails to be sent by its reliability

		bool exit;																				// End of program control flag
		int connectionLost;																		// Connection retry counter
		int timeoutLimit;																		// The time out limit

		std::thread receiver;																	// Infinite packet receiving loop(until end)
		std::mutex sendingPacket;																// controlls the interaction between send functions
		std::mutex acknowledging;																// controlls the interaction between runClock() and receiveACK()
		std::mutex receiving;																	// controlls the interaction between checkPacketAvailable() and receive()
		
		std::map<unsigned int,std::map<unsigned short,std::vector<Packet>>> receivedPackets;	// Contains all the received packets classified by its ip and its port
// ----------------------------------------------------------------------------------------------------------------------

		/**
		 * send
		 * 
		 * EFE: sends a packet to the given address.
		 * REQ: header: the receiver address.
		 * 		data: the fragment of a message to be sent.
		 */
		void send(PacketHeader header, const char * data);
		
		/**
		 * writeHandler
		 * 
		 * EFE: Locks the entrance of a send during another one.
		 */
		void writeHandler();

		/**
		 * runClock
		 * 
		 * EFE: Manages the time out between a send and its ack.
		 * REQ: &timeout: the direction of a shared boolean specifying the time out.
		 * 		&timeout: the direction of a shared boolean specifying the acknowledgement.
		 */
		void runClock(bool &timeout, bool &acknowledged);

		/**
		 * receiveACK
		 * EFE: Receives an ack searching inside the receivedPackets multimap
		 * REQ: The address of the machine who sends the ack to us.
		 * 		&timeout: the direction of a shared boolean specifying the time out.
		 * 		&timeout: the direction of a shared boolean specifying the acknowledgement.
		 */
		void receiveACK(PacketHeader header, bool &timeout, bool &acknowledged);


		/**
		 * sendDone
		 * EFE: Unlocks the entrance for another sends
		 */
		void sendDone();

// ----------------------------------------------------------------------------------------------------------------------

		std::string receive(PacketHeader &header);

		/**
		 * readHandler
		 * 
		 * EFE:	One packet lecture at time from a especific sender.
		 * REQ: header: sender address.
		 */
		void readHandler(PacketHeader header);	
		
// ----------------------------------------------------------------------------------------------------------------------
		
		/**
		 * checkPacketAvailable
		 * 
		 * EFE: Receives any packet and stores it on a vector inside a multimap.
		 */
		void checkPacketAvailable();

		/**
		 * sendACK
		 * 
		 * EFE: Constructs a packet as an ack packet, and sends it to the sender.
		 * REQ: the address of the sender.
		 */
		void sendACK(PacketHeader header);

// ----------------------------------------------------------------------------------------------------------------------

		/**
		 * byteArrayToPacket
		 * 
		 * EFE: Translates a byte array into a packet.
		 * REQ: bytes: The byte array.
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
