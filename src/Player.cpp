#include "../lib/Battleship.hpp"

int main(int argc, char**argv){
	if(argc<3){
		std::cout<<"El programa necesita los siguientes parametros en el orden indicado:\n"
		<< "\t1: IP del otro jugador(string).\n" 
 		<< "\t2: Puerto del otro jugador(int).\n"
		<< "\t3: Puerto propio(int).\n";
		exit(EXIT_SUCCESS);
	}
    Battleship b(argv[1],atoi(argv[2]),atoi(argv[3]));
    return EXIT_SUCCESS;
}
