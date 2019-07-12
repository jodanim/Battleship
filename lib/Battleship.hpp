#ifndef BATTLESHIP_HPP
#define BATTLESHIP_HPP

#include "Network.hpp"

// Ship sise enum: submarine, destroyer, cruiser, battleship, aricrafCarrier
enum Ships{submarine = 1, destroyer, cruiser, battleship, aricrafCarrier};

// Ship orientation enum: horizontal, vertical 
enum Orientiation{horizontal, vertical};

// Cell state enum: empty, filled, destroyed, failed
enum CellState{empty, filled, destroyed, failed};

class Battleship{
	public:
		
		/**
		 * Constructor
		 * 
		 * EFE: Initialize variables and starts the battleship game.
		 * REQ: enemyIp: The enemy ip adress.
		 * 		enemyPort: The enemy port number.
		 * 		ownPort: your port number to be bind.
		 */
		Battleship(const char * enemyIp, unsigned short enemyPort, unsigned short ownPort, int orientation);
		
		/**
		 * Destructor
		 * 
		 * EFE: Deletes objetcs created.
		 */
		~Battleship();

		/**
		 * getCoordinate
		 * 
		 * EFE: Converts const char array coordinate(A1-J10) into an int coordinate.
		 * REQ: coordinate: Any const char array.
		 * RET: The int converted coordinate.
		 */ 
		int getCoordinate(std::string coordinate);
		
		/**
		 * getInput 
		 * 
		 * EFE: Waits for user input and calls getCoordinate with it.
		 * RET: the given coordinate on a numeric format.
		 */
		int getInput();
		
		/**
		 * attack
		 * 
		 * EFE: Sends a message to the enemy and waits for an answer to change the given cell state.
		 * REQ: pos: the coordinate where to attack.
		 */
		void attack(int pos);

		/**
		 * updateGrid
		 * 
		 * EFE: Sends a message to the enemy and indicating if he damaged or not his flet.
		 * REQ: pos: the coordinate where to update.
		 */
		void updateGrid(int pos);

		/**
		 * setShip
		 * 
		 * EFE: Sets a ship in a random position.
		 * REQ: ship: the ship tipe.
		 */
		void setShip(Ships ship);
		
		/**
		 * setGrid
		 * 
		 * EFE: Sets all the ships in a random position.
		 */
		void setGrid();

		/**
		 * printCell
		 * 
		 * EFE: Prints a cell acording to its state.
		 * REQ: state: the actual cell state.
		 */
		void printCell(CellState state);

		/**
		 * printGrid
		 * 
		 * EFE: Prints both grids starting at the position 0,0 of the terminal.
		 */
		void printGrid(int orientation);
		
	private:

		// To manage the conection
		Network * network;

		// Enemy information
		PacketHeader * header;

		// Own 10x10 grid
        std::vector<CellState> grid;

		// The 10x10 enemy grid
        std::vector<CellState> enemyGrid;

		// Game state flag.
		bool gameover = false;

		// Game winner flag.
		bool winner = false;

		// Game turn flag.
		bool yourTurn = false;

		// Own ship cells counter
		int shipCells = 18;

		// Enemy ship cells counter
		int enemyShipCells = 18;
};

#endif /* BATTLESHIP_HPP */