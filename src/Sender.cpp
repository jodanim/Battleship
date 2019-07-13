#include "../lib/Network.hpp"

/*
 *	Arguments:
 *	1: Receiver ip
 *	2: Receiver port
 *	3: Port to bind
 *	4: Reliability
 *	5: Message
 * 	6: Repetitions
*/
int main(int argc, char**argv){
	if(argc<6){
		std::cout<<"El programa necesita los siguientes parametros en el orden indicado:\n"
		<< "\t1: IP del receptor(string).\n" 
 		<< "\t2: Puerto del receptor(int).\n"
		<< "\t3: Puerto a enlazar(int).\n"
		<< "\t4: Confiabilidad(double).\n"
		<< "\t5: Mensage(string).\n"
		<< "\t6: Repeticiones(int).\n";
		exit(EXIT_SUCCESS);
	}
	
	std::cout<<"Enviando Mensaje\n\n" << argv[5]<<"\n";
    Network n(atoi(argv[3]),atof(argv[4]));
    PacketHeader header(argv[1],atoi(argv[2]));
    for(int i = 0; i<atoi(argv[6]);i++){
		std::cout<<"\nEnvio #"<<i+1<<"\n\n";
    	n.sendMessage(header,argv[5]);
    }
    return EXIT_SUCCESS;
}
