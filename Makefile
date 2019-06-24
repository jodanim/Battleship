all: main main_d finished

#################################################################################################################################################################################

Socket.obj: lib/Socket.hpp src/Socket.cpp
	@echo "\033[1;35mGenerating Socket object code\033[0m"
	@g++ -c src/Socket.cpp -o obj/Socket.obj


Socket.obj_d: lib/Socket.hpp src/Socket.cpp
	@echo "\033[1;36mGenerating Socket debug object code\033[0m"
	@g++ -g -c src/Socket.cpp -o obj/debug/Socket.obj


#--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Time.obj: lib/Time.hpp src/Time.cpp
	@echo "\033[1;35mGenerating Time object code\033[0m"
	@g++ -c src/Time.cpp -o obj/Time.obj


Time.obj_d: lib/Time.hpp src/Time.cpp
	@echo "\033[1;36mGenerating Time debug object code\033[0m"
	@g++ -g -c src/Time.cpp -o obj/debug/Time.obj


#--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

MessageHandler.obj: lib/MessageHandler.hpp src/MessageHandler.cpp
	@echo "\033[1;35mGenerating MessageHandler object code\033[0m"
	@g++ -c src/MessageHandler.cpp -o obj/MessageHandler.obj


MessageHandler.obj_d: lib/MessageHandler.hpp src/MessageHandler.cpp
	@echo "\033[1;36mGenerating MessageHandler debug object code\033[0m"
	@g++ -g -c src/MessageHandler.cpp -o obj/debug/MessageHandler.obj

#--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

FileManager.obj: lib/FileManager.hpp src/FileManager.cpp
	@echo "\033[1;35mGenerating FileManager object code\033[0m"
	@g++ -c src/FileManager.cpp -o obj/FileManager.obj


FileManager.obj_d: lib/FileManager.hpp src/FileManager.cpp
	@echo "\033[1;36mGenerating FileManager debug object code\033[0m"
	@g++ -g -c src/FileManager.cpp -o obj/debug/FileManager.obj


#--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Network.obj: lib/Network.hpp src/Network.cpp
	@echo "\033[1;35mGenerating Network object code\033[0m"
	@g++ -pthread -c src/Network.cpp -o obj/Network.obj


Network.obj_d: lib/Network.hpp src/Network.cpp
	@echo "\033[1;36mGenerating Network debug object code\033[0m"
	@g++ -pthread -g -c src/Network.cpp -o obj/debug/Network.obj
	
#################################################################################################################################################################################

main.obj: src/main.cpp
	@echo "\033[1;35mGenerating main object code\033[0m"
	@g++ -c src/main.cpp -o obj/main.obj 


main.obj_d: src/main.cpp
	@echo "\033[1;36mGenerating main debug object code\033[0m"
	@g++ -g -c src/main.cpp -o obj/debug/main.obj

	
#################################################################################################################################################################################

main: compileMessage Socket.obj MessageHandler.obj FileManager.obj Time.obj Network.obj  main.obj 
	@echo "\033[1;35mGenerating executable \033[0m"
	@g++ -pthread obj/main.obj obj/Socket.obj obj/MessageHandler.obj obj/FileManager.obj obj/Time.obj obj/Network.obj -o bin/main


main_d: compileMessage Socket.obj_d MessageHandler.obj_d FileManager.obj_d  Time.obj_d Network.obj_d main.obj_d 
	@echo "\033[1;36mGenerating debug executable\033[0m"
	@g++ -pthread -g obj/debug/main.obj obj/debug/Socket.obj obj/debug/MessageHandler.obj obj/debug/FileManager.obj obj/debug/Time.obj obj/debug/Network.obj -o bin/debug/main

#################################################################################################################################################################################

run:main runMessage 
	@.\/bin/main 
	@make finished --no-print-directory
debug:main_d debugMessage
	@gdb bin/debug/main
	@make finished --no-print-directory

#################################################################################################################################################################################

compileMessage:
	clear
	@echo "\033[1;31m------------------------------------------------\033[0m"
	@echo "\033[1;33m                   Compiling                    \033[0m"
	@echo "\033[1;31m------------------------------------------------\033[0m\n"

runMessage:
	@echo "\n\033[1;31m------------------------------------------------\033[0m"
	@echo   "\033[1;33m            Running the main program            \033[0m"
	@echo   "\033[1;31m------------------------------------------------\033[0m\n"

debugMessage:
	@echo "\n\033[1;31m------------------------------------------------\033[0m"
	@echo   "\033[1;33m           Debugging the main program           \033[0m"
	@echo   "\033[1;31m------------------------------------------------\033[0m\n"

finished:
	@echo "\n\033[1;31m------------------------------------------------\033[0m"
	@echo   "\033[1;33m                    Finished                    \033[0m"
	@echo   "\033[1;31m------------------------------------------------\033[0m\n"

#################################################################################################################################################################################
