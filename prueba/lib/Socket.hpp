#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
class Socket{
	public:
		Socket();
		~Socket();
		int Bind(int port);
		int Read(char * buffer, int len, char * ip, unsigned short port);
		int Write(const char * buffer, const char * ip, unsigned short port);
		int Close();
	private:
		int sockfd;
};
#endif