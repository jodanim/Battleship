all: compile compile_d finished

#################################################################################################################################################################################

obj/Socket.obj: src/Socket.cpp
	@echo "\033[1;35mGenerating Socket object code $@\033[0m"
	@g++ -c $^ -o $@


obj/debug/Socket.obj: src/Socket.cpp
	@echo "\033[1;36mGenerating Socket debug object code\033[0m"
	@g++ -g -c $^ -o $@


#--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

obj/Time.obj: src/Time.cpp
	@echo "\033[1;35mGenerating Time object code\033[0m"
	@g++ -c $^ -o $@


obj/debug/Time.obj: src/Time.cpp
	@echo "\033[1;36mGenerating Time debug object code\033[0m"
	@g++ -g -c $^ -o $@


#--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

obj/MessageHandler.obj: src/MessageHandler.cpp
	@echo "\033[1;35mGenerating MessageHandler object code\033[0m"
	@g++ -c $^ -o $@


obj/debug/MessageHandler.obj: src/MessageHandler.cpp
	@echo "\033[1;36mGenerating MessageHandler debug object code\033[0m"
	@g++ -g -c $^ -o $@

#--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

obj/FileManager.obj: src/FileManager.cpp
	@echo "\033[1;35mGenerating FileManager object code\033[0m"
	@g++ -c $^ -o $@


obj/debug/FileManager.obj: src/FileManager.cpp
	@echo "\033[1;36mGenerating FileManager debug object code\033[0m"
	@g++ -g -c $^ -o $@


#--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

obj/Network.obj: src/Network.cpp
	@echo "\033[1;35mGenerating Network object code\033[0m"
	@g++ -c $^ -o $@


obj/debug/Network.obj: src/Network.cpp
	@echo "\033[1;36mGenerating Network debug object code\033[0m"
	@g++ -g -c $^ -o $@
	
#################################################################################################################################################################################

obj/main.obj: src/main.cpp
	@echo "\033[1;35mGenerating main object code\033[0m"
	@g++ -c $^ -o $@


obj/debug/main.obj: src/main.cpp
	@echo "\033[1;36mGenerating main debug object code\033[0m"
	@g++ -g -c $^ -o $@

	
#################################################################################################################################################################################

bin/main: obj/Socket.obj obj/MessageHandler.obj obj/FileManager.obj obj/Time.obj obj/Network.obj  obj/main.obj
	@echo "\033[1;35mGenerating executable \033[0m"
	@g++ -pthread $^ -o $@


bin/debug/main: obj/debug/Socket.obj obj/debug/MessageHandler.obj obj/debug/FileManager.obj obj/debug/Time.obj obj/debug/Network.obj obj/debug/main.obj 
	@echo "\033[1;36mGenerating debug executable\033[0m"
	@g++ -pthread -g $^ -o $@

#################################################################################################################################################################################


compile:compileMessage bin/main 
	@echo "\033[1;35mCompiled\033[0m"
	
compile_d:compileMessage bin/debug/main
	@echo "\033[1;36mDebug compiled\033[0m"
	 
run:compile runMessage 
	@.\/bin/main 
	@make finished --no-print-directory
debug:compile_d debugMessage
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
