#include "../lib/Socket.hpp"

Socket::Socket(){
	in_addr_t answerAddress = 0;
	uint16_t answerPort = 0;
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
		perror("Socket:Constructor"); 
		exit(EXIT_FAILURE);
	}
}

Socket::~Socket(){
	Close();
}

int Socket::Bind(int port){
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);
	int len = sizeof(addr);
	int result = bind(sockfd, (const sockaddr *)&addr, len);
	if(result == -1){
		perror("Socket:Bind");
		exit(EXIT_FAILURE);
	}
	
	return result;
	
}

int Socket::Read(unsigned char * buffer, unsigned int len){
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);
	int bytes = recvfrom(sockfd, buffer, len, 0, (struct sockaddr *) &addr, &addrlen);
	if(bytes == -1){
		perror("Socket:Read");
		exit(EXIT_FAILURE);
	}
	answerPort = ntohs(addr.sin_port);
	answerAddress = addr.sin_addr.s_addr;
	buffer[bytes]='\0';

	return bytes;
}

int Socket::Write(const unsigned char * message, int len, unsigned int ip, uint16_t port){
	port = (port==0)?answerPort:port;
	struct sockaddr_in addr;
 	addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
	unsigned int address;
	if(ip != -1)
		addr.sin_addr.s_addr = ip;
	else
		addr.sin_addr.s_addr = answerAddress;
	int bytes = sendto(sockfd, message, len, 0, (const struct sockaddr *) &addr, sizeof(addr));
	if(bytes == -1){
		perror("Socket:Write");
		exit(EXIT_FAILURE);
	}
	return bytes;
}

int Socket::Close(){
	int result = close(sockfd);
	if(result == -1){
		perror("Socket:Close");
		exit(EXIT_FAILURE);
	}
	return result;
}
