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
		Socket(int port);
		~Socket();
		int Bind();
		int Read(char * buffer, int len);
		int Write(const char * buffer, const char * ip);
		int Close();
	private:
		int sockfd;
		int port;

};
#endif