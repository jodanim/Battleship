#include "../lib/Socket.hpp"
#include <iostream>

#define PORT    1102 
#define MAXLINE 1024 
  
int main() { 
    int sockfd; 
    char buffer[MAXLINE]; 
    char *hello = (char *)"Hello from client"; 

  
    Socket s;
    s.Bind(PORT);
    int n; 
    s.Write(hello,"192.168.0.16",1101);
	printf("Hello message sent.\n"); 

	std::cout<<"\nread\n";
	s.Read(buffer,MAXLINE,"192.168.0.16",1101);
    printf("Server : %s\n", buffer); 
  
    close(sockfd); 
    return 0; 
} 