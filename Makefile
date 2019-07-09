all: compile compile_d finished

################################################################################

clean_obj:
	@echo "\033[1;31mDeleting all the objects\033[0m"
	@rm -r obj/*
	@mkdir obj/debug
	@touch obj/debug/.gitkeep

################################################################################

obj/Socket.obj: src/Socket.cpp
	@echo "\033[1;35mGenerating \033[0;33m$@\033[0m"
	@g++ -c $^ -o $@

obj/debug/Socket.obj: src/Socket.cpp
	@echo "\033[1;36mGenerating \033[0;33m$@\033[0m"
	@g++ -g -c $^ -o $@

#-------------------------------------------------------------------------------

obj/Time.obj: src/Time.cpp
	@echo "\033[1;35mGenerating \033[0;33m$@\033[0m"
	@g++ -c $^ -o $@

obj/debug/Time.obj: src/Time.cpp
	@echo "\033[1;36mGenerating \033[0;33m$@\033[0m"
	@g++ -g -c $^ -o $@

#-------------------------------------------------------------------------------

obj/MessageHandler.obj: src/MessageHandler.cpp
	@echo "\033[1;35mGenerating \033[0;33m$@\033[0m"
	@g++ -c $^ -o $@

obj/debug/MessageHandler.obj: src/MessageHandler.cpp
	@echo "\033[1;36mGenerating \033[0;33m$@\033[0m"
	@g++ -g -c $^ -o $@

#-------------------------------------------------------------------------------

obj/FileManager.obj: src/FileManager.cpp
	@echo "\033[1;35mGenerating \033[0;33m$@\033[0m"
	@g++ -c $^ -o $@


obj/debug/FileManager.obj: src/FileManager.cpp
	@echo "\033[1;36mGenerating \033[0;33m$@\033[0m"
	@g++ -g -c $^ -o $@

#-------------------------------------------------------------------------------

obj/Translator.obj: src/Translator.cpp
	@echo "\033[1;35mGenerating \033[0;33m$@\033[0m"
	@g++ -c $^ -o $@


obj/debug/Translator.obj: src/Translator.cpp
	@echo "\033[1;36mGenerating \033[0;33m$@\033[0m"
	@g++ -g -c $^ -o $@

#-------------------------------------------------------------------------------


obj/Network.obj: src/Network.cpp
	@echo "\033[1;35mGenerating \033[0;33m$@\033[0m"
	@g++ -c $^ -o $@

obj/debug/Network.obj: src/Network.cpp
	@echo "\033[1;36mGenerating \033[0;33m$@\033[0m"
	@g++ -g -c $^ -o $@
	
#-------------------------------------------------------------------------------

obj/main.obj: src/main.cpp
	@echo "\033[1;35mGenerating \033[0;33m$@\033[0m"
	@g++ -c $^ -o $@

obj/client.obj: src/client.cpp
	@echo "\033[1;35mGenerating \033[0;33m$@\033[0m"
	@g++ -c $^ -o $@

obj/debug/main.obj: src/main.cpp
	@echo "\033[1;36mGenerating \033[0;33m$@\033[0m"
	@g++ -g -c $^ -o $@

################################################################################

bin/main: obj/Socket.obj obj/MessageHandler.obj obj/FileManager.obj obj/Time.obj obj/Translator.obj obj/Network.obj obj/main.obj 
	@echo "\033[1;35mGenerating \033[0;33m$@\033[0m"
	@g++ -pthread $^ -o $@

bin/client: obj/Socket.obj obj/MessageHandler.obj obj/FileManager.obj obj/Time.obj obj/Translator.obj obj/Network.obj obj/client.obj
	@echo "\033[1;35mGenerating \033[0;33m$@\033[0m"
	@g++ -pthread $^ -o $@

bin/debug/main: obj/debug/Socket.obj obj/debug/MessageHandler.obj obj/debug/FileManager.obj obj/debug/Time.obj obj/debug/Translator.obj obj/debug/Network.obj obj/debug/main.obj 
	@echo "\033[1;36mGenerating \033[0;33m$@\033[0m"
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
