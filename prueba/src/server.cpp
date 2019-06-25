#include "../lib/Socket.hpp"
#include <iostream>
/**

#define forever for(;;)

int main(int argc, char**argv){
	int childpid;
	Socket s1, *s2;
	s1.Bind(1101);
	forever{
		childpid = fork();
		if(childpid < 0)perror("server: Error de bifurcación.");
		else if (0 == childpid) { 
			s1.Close();
			/*char char_array[n + 1]; 
			strcpy(char_array, file.c_str());
			s2->Write(char_array);*/
	/*		sleep(1);
			std::cout<<"\n"<<s2->Read(64)<<"\n";
			exit(0);
		}
	}
    return EXIT_SUCCESS;
}*/

// Server side implementation of UDP client-server model 
#define forever for(;;)
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
   
int main() { 
	Socket socket;	
	socket.Bind(1101);
    char buffer[64];
    forever{
		std::cout<<socket.Read(64,buffer);
	}
    return 0; 
} 

