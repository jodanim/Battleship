all: compile finished

################################################################################

clean:
	@echo "\033[1;31mDeleting all the objects\033[0m"
	@rm -r obj/*

################################################################################

obj/Socket.obj: src/Socket.cpp
	@echo "\033[1;35mGenerating \033[0;33m$@\033[0m"
	@g++ -c $^ -o $@

#-------------------------------------------------------------------------------

obj/MessageHandler.obj: src/MessageHandler.cpp
	@echo "\033[1;35mGenerating \033[0;33m$@\033[0m"
	@g++ -c $^ -o $@

#-------------------------------------------------------------------------------

obj/FileManager.obj: src/FileManager.cpp
	@echo "\033[1;35mGenerating \033[0;33m$@\033[0m"
	@g++ -c $^ -o $@

#-------------------------------------------------------------------------------

obj/Translator.obj: src/Translator.cpp
	@echo "\033[1;35mGenerating \033[0;33m$@\033[0m"
	@g++ -c $^ -o $@

#-------------------------------------------------------------------------------

obj/Network.obj: src/Network.cpp
	@echo "\033[1;35mGenerating \033[0;33m$@\033[0m"
	@g++ -c $^ -o $@

#-------------------------------------------------------------------------------

obj/Battleship.obj: src/Battleship.cpp
	@echo "\033[1;35mGenerating \033[0;33m$@\033[0m"
	@g++ -c $^ -o $@

#-------------------------------------------------------------------------------

obj/Player.obj: src/Player.cpp
	@echo "\033[1;35mGenerating \033[0;33m$@\033[0m"
	@g++ -c $^ -o $@

################################################################################

bin/Player: obj/Socket.obj obj/MessageHandler.obj obj/FileManager.obj obj/Translator.obj obj/Network.obj obj/Battleship.obj obj/Player.obj 
	@echo "\033[1;35mGenerating \033[0;33m$@\033[0m"
	@g++ -pthread $^ -o $@

################################################################################

compile:compileMessage bin/Player

run_player1: compile runMessage
	@./bin/Player 0.0.0.0 1101 1102 0
	@make finished --no-print-directory

run_player2: compile runMessage
	@./bin/Player 0.0.0.0 1102 1101 1
	@make finished --no-print-directory
	
################################################################################

compileMessage:
	clear
	@echo "\033[1;31m------------------------------------------------\033[0m"
	@echo "\033[1;33m                   Compiling                    \033[0m"
	@echo "\033[1;31m------------------------------------------------\033[0m\n"

runMessage:
	clear
	@echo "\033[1;31m------------------------------------------------\033[0m"
	@echo "\033[1;33m                    Running                     \033[0m"
	@echo "\033[1;31m------------------------------------------------\033[0m\n"

finished:
	@echo "\n\033[1;31m------------------------------------------------\033[0m"
	@echo   "\033[1;33m                    Finished                    \033[0m"
	@echo   "\033[1;31m------------------------------------------------\033[0m\n"

################################################################################
