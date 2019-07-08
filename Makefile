all: compile compile_d finished

################################################################################

obj/Socket.obj: src/Socket.cpp
	@echo "\e[1;35mGenerating \e[0;33m$@\e[0m"
	@g++ -c $^ -o $@

obj/debug/Socket.obj: src/Socket.cpp
	@echo "\e[1;36mGenerating \e[0;33m$@\e[0m"
	@g++ -g -c $^ -o $@

#-------------------------------------------------------------------------------

obj/Time.obj: src/Time.cpp
	@echo "\e[1;35mGenerating \e[0;33m$@\e[0m"
	@g++ -c $^ -o $@

obj/debug/Time.obj: src/Time.cpp
	@echo "\e[1;36mGenerating \e[0;33m$@\e[0m"
	@g++ -g -c $^ -o $@

#-------------------------------------------------------------------------------

obj/MessageHandler.obj: src/MessageHandler.cpp
	@echo "\e[1;35mGenerating \e[0;33m$@\e[0m"
	@g++ -c $^ -o $@

obj/debug/MessageHandler.obj: src/MessageHandler.cpp
	@echo "\e[1;36mGenerating \e[0;33m$@\e[0m"
	@g++ -g -c $^ -o $@

#-------------------------------------------------------------------------------

obj/FileManager.obj: src/FileManager.cpp
	@echo "\e[1;35mGenerating \e[0;33m$@\e[0m"
	@g++ -c $^ -o $@


obj/debug/FileManager.obj: src/FileManager.cpp
	@echo "\e[1;36mGenerating \e[0;33m$@\e[0m"
	@g++ -g -c $^ -o $@

#-------------------------------------------------------------------------------

obj/Translator.obj: src/Translator.cpp
	@echo "\e[1;35mGenerating \e[0;33m$@\e[0m"
	@g++ -c $^ -o $@


obj/debug/Translator.obj: src/Translator.cpp
	@echo "\e[1;36mGenerating \e[0;33m$@\e[0m"
	@g++ -g -c $^ -o $@

#-------------------------------------------------------------------------------


obj/Network.obj: src/Network.cpp
	@echo "\e[1;35mGenerating \e[0;33m$@\e[0m"
	@g++ -c $^ -o $@

obj/debug/Network.obj: src/Network.cpp
	@echo "\e[1;36mGenerating \e[0;33m$@\e[0m"
	@g++ -g -c $^ -o $@
	
#-------------------------------------------------------------------------------

obj/main.obj: src/main.cpp
	@echo "\e[1;35mGenerating \e[0;33m$@\e[0m"
	@g++ -c $^ -o $@

obj/client.obj: src/client.cpp
	@echo "\e[1;35mGenerating \e[0;33m$@\e[0m"
	@g++ -c $^ -o $@

obj/debug/main.obj: src/main.cpp
	@echo "\e[1;36mGenerating \e[0;33m$@\e[0m"
	@g++ -g -c $^ -o $@

################################################################################

bin/main: obj/Socket.obj obj/MessageHandler.obj obj/FileManager.obj obj/Time.obj obj/Translator.obj obj/Network.obj obj/main.obj 
	@echo "\e[1;35mGenerating \e[0;33m$@\e[0m"
	@g++ -pthread $^ -o $@

bin/client: obj/Socket.obj obj/MessageHandler.obj obj/FileManager.obj obj/Time.obj obj/Translator.obj obj/Network.obj obj/client.obj
	@echo "\e[1;35mGenerating \e[0;33m$@\e[0m"
	@g++ -pthread $^ -o $@

bin/debug/main: obj/debug/Socket.obj obj/debug/MessageHandler.obj obj/debug/FileManager.obj obj/debug/Time.obj obj/debug/Translator.obj obj/debug/Network.obj obj/debug/main.obj 
	@echo "\e[1;36mGenerating \e[0;33m$@\e[0m"
	@g++ -pthread -g $^ -o $@

################################################################################


compile:compileMessage bin/main 

compile_client:compileMessage bin/client 
	
compile_d:compileMessage bin/debug/main
	 
run:compile runMessage 
	@.\/bin/main 
	@make finished --no-print-directory

run_client:compile_client runMessage 
	@.\/bin/client 
	@make finished --no-print-directory

debug:compile_d debugMessage
	@gdb bin/debug/main
	@make finished --no-print-directory

################################################################################

compileMessage:
	clear
	@echo "\e[1;31m------------------------------------------------\e[0m"
	@echo "\e[1;33m                   Compiling                    \e[0m"
	@echo "\e[1;31m------------------------------------------------\e[0m\n"

runMessage:
	@echo "\n\e[1;31m------------------------------------------------\e[0m"
	@echo   "\e[1;33m            Running the main program            \e[0m"
	@echo   "\e[1;31m------------------------------------------------\e[0m\n"

debugMessage:
	@echo "\n\e[1;31m------------------------------------------------\e[0m"
	@echo   "\e[1;33m           Debugging the main program           \e[0m"
	@echo   "\e[1;31m------------------------------------------------\e[0m\n"

finished:
	@echo "\n\e[1;31m------------------------------------------------\e[0m"
	@echo   "\e[1;33m                    Finished                    \e[0m"
	@echo   "\e[1;31m------------------------------------------------\e[0m\n"

################################################################################
