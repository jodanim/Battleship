#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cerrno>

class Socket{
	public:
		Socket();
		~Socket();
		int Bind(int port);
		int Read(unsigned char * buffer, unsigned int len);
		int Write(const unsigned char * message, int len, unsigned int ip = -1, uint16_t port = 0);
		int Close();
	private:
		int sockfd;
		in_addr_t answerAddress;
		uint16_t answerPort;
};
#endif