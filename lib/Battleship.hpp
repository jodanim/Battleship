#ifndef BATTLESHIP_HPP
#define BATTLESHIP_HPP

#include "Network.hpp"
#include <vector>

enum Ships{submarine = 1,destroyer,cruiser,battleship,aricrafCarrier};

enum Orientiation{horizontal,vertical};

enum CellState{empty,filled,destroyed,failed};

class Battleship{
	public:
		Battleship(const char * enemyIp, unsigned short enemyPort, unsigned short ownPort);
		~Battleship();
		int getCoordinate(const char * coordinate);
		int getInput();
		void attack(int pos);
		void updateGrid(int pos);
		void setShip(Ships ship);
		void setGrid();
		void printCell(CellState state);
		void printGrid();
		void runClock();
	private:
		Network * network;
		PacketHeader * header;
        std::vector<CellState> grid;
        std::vector<CellState> enemyGrid;
		bool gameover = false;
		bool winner = false;
		bool yourTurn = false;
		int shipCells = 18;
		int enemyShipCells = 18;
};

#endif /* BATTLESHIP_HPP */