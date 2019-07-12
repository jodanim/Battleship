#include "../lib/Battleship.hpp"

int main(int argc, char**argv){
	if(argc<4){
		std::cout<<"El programa necesita los siguientes parametros en el orden indicado:\n"
		<< "\t1: IP del otro jugador(string).\n" 
 		<< "\t2: Puerto del otro jugador(int).\n"
		<< "\t3: Puerto propio(int).\n"
		<< "\t4: Orientación(0:horizontal,1:vertical).\n";
		exit(EXIT_SUCCESS);
	}
    Battleship b(argv[1],atoi(argv[2]),atoi(argv[3]),atoi(argv[4]));
    return EXIT_SUCCESS;
}
