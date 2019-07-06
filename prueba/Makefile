all: compile finished

################################################################################

obj/Socket.obj: src/Socket.cpp
	@echo "\033[1;35mGenerating \033[0;33m$@\033[0m"
	@g++ -c $^ -o $@

#-------------------------------------------------------------------------------

obj/client.obj: src/client.cpp
	@echo "\033[1;35mGenerating \033[0;33m$@\033[0m"
	@g++ -c $^ -o $@


obj/server.obj: src/server.cpp
	@echo "\033[1;35mGenerating \033[0;33m$@\033[0m"
	@g++ -c $^ -o $@


################################################################################

bin/client: obj/Socket.obj obj/client.obj
	@echo "\033[1;35mGenerating \033[0;33m$@\033[0m"
	@g++ $^ -o $@

bin/server: obj/Socket.obj obj/server.obj
	@echo "\033[1;35mGenerating \033[0;33m$@\033[0m"
	@g++ $^ -o $@

################################################################################

compile:compileMessage bin/client bin/server 
		 
run_client:compile runMessage 
	@.\/bin/client
	@make finished --no-print-directory
		 
run_server:compile runMessage 
	@.\/bin/client
	@make finished --no-print-directory


################################################################################

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

################################################################################
