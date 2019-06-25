#include "../lib/Socket.hpp"
#include <iostream>

//	AF_INET(IPv4) AF_INET6(IPv6)
//	SOCK_STREAM(TCP) SOCK_DGRAM(UDP)

Socket::Socket(){
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		perror("Socket:Constructor"); 
}


Socket::Socket(int new_sockfd){
	sockfd = new_sockfd;
}

Socket::~Socket(){
	Close();
}

int Socket::Connect(std::string Host, int Port){
	char ip[15];
	strncpy(ip, Host.c_str(),15);
	struct sockaddr_in  addr;
	addr.sin_family = AF_INET;
	inet_aton(ip, &addr.sin_addr);
	addr.sin_port = htons(Port);
	int addrlen = sizeof(addr);
	int result = connect(sockfd, (sockaddr *) &addr, addrlen);
	if(result == -1)perror("Socket:ConnectIpv4");
	return result;
}

int Socket::Listen(int Queue){
	int result = listen(sockfd, Queue);
	if(result == -1)perror("Socket:Listen");
	return result;
}

int Socket::Bind(int Port){
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(Port);
	int len = sizeof(addr);
	int result = bind(sockfd, (const sockaddr *)&addr, len);
	if(result == -1)perror("Socket:Bind");
	return result;
}

std::string Socket::Read(int len, char *buffer){
	//char text[len];
	//if(read(sockfd, text, len) == -1)perror("Socket:Read");
	//return std::string(text);
/*	char buffer[64];
	struct sockaddr_in cliaddr;
	socklen_t clilen;
   	int n = recvfrom(sockfd, (char *)buffer, len, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &clilen); 
    buffer[n] = '\0'; 
    return std::string(buffer);
  */
	int n; 
	n = recv(sockfd, buffer, len, MSG_WAITALL);
	// buffer[n] = '\0'; 
	// printf("%s.\n",buffer); 
	printf("Hello message received.\n"); 
	return std::string(buffer,n);
}

int Socket::Write(std::string text, int port){
	//int bytes = write(sockfd, text.c_str(), text.size());
	//if(bytes == -1)perror("Socket:Write");
	//return bytes;
	struct sockaddr_in addr;
	addr.sin_family = AF_INET; 
    addr.sin_addr.s_addr = htonl(INADDR_ANY); 
    addr.sin_port = htons(port); 
	int bytes = sendto(sockfd, (const char *)text.c_str(), text.size(), MSG_CONFIRM, (const struct sockaddr *) &addr, sizeof(addr)); 
	if(bytes == -1)perror("Socket:Write");
	printf("Hello message sent.\n"); 
	return bytes;
}

/*Socket * Socket::Accept(){
	struct sockaddr_in cli_addr;
	int clilen = sizeof(cli_addr);
	int new_sockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen);
	if(new_sockfd==-1)perror("Socket:Accept");
	return new Socket(new_sockfd);
}*/

/*int Socket::Shutdown(int o){
	int result = shutdown(sockfd, o);
	if(result == -1)perror("Socket:Shutdown");
	return result;
}*/

int Socket::Close(){
	int result = close(sockfd);
	if(result == -1)perror("Socket:Close");
	return result;
}
