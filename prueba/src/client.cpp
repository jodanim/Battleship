#include "../lib/Socket.hpp"
#include <iostream>

#define PORT    1102 
#define MAXLINE 1024 
  
int main() { 
    int sockfd; 
    char buffer[MAXLINE]; 
    Socket s;
    // s.Bind(PORT);
    int n; 

    s.Write("hello from client","192.168.0.16",1101);
	printf("Hello message sent.\n"); 
	s.Read(buffer,MAXLINE);
	printf("Server : %s.\n",buffer); 

    close(sockfd); 
    return 0; 
} 