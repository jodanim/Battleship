#include "../lib/Network.hpp"

/*
 *	Arguments:
 *	1: sender ip
 *	2: sender port
 *	3: Port to bind
*/
int main(int argc, char**argv){
	if(argc<2){
		std::cout<<"El programa necesita los siguientes parametros en el orden indicado:\n"
		<< "\t1: IP del emisor(string).\n" 
 		<< "\t2: Puerto del emisor(int).\n"
		<< "\t3: Puerto propio a enlazar(int).\n";
		exit(EXIT_SUCCESS);
	}
	std::cout<<"Recibiendo mensaje\n\n";
    Network n(atoi(argv[3]),1);
    forever{
        PacketHeader header(argv[1],atoi(argv[2]));
        std::string message = n.receiveMessage(header);
        std::cout<<iteration+1<<": "<<message<<"\n\n";
    }
    return EXIT_SUCCESS;
}
