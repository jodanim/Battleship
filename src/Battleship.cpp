#include "../lib/Battleship.hpp"

Battleship::Battleship(const char * enemyIp, unsigned short enemyPort, unsigned short ownPort){
    srand(time(NULL));
    network = new Network(ownPort,1);
    setGrid();
    header = new PacketHeader(enemyIp,enemyPort);
    std::cout<<"Waiting for connection.\n";
    
    char buffer[8];
    int random;
    do{
        random = rand()%100;
        network->sendMessage(*header,std::to_string(random).c_str());
        network->receiveMessage(buffer);
        yourTurn = random > atoi(buffer);
    }while(random == atoi(buffer));

    std::cout<<"first random turn: "<<"You: "<< random <<" The enemy: "<<buffer<<"\n";
    sleep(5);
    while(!gameover){
        printGrid();
        if(yourTurn){
            std::cout<<"\033[KYour turn, select a coordinate to attack:\n\033[K";
            int pos = getInput();
            attack(pos);
        }else{
            std::cout<<"\033[KWaiting for enemy move.\n\033[K";
            network->receiveMessage(buffer);
            int pos = getCoordinate(buffer);
            updateGrid(pos);
        }
    }
    printGrid();
    if(winner){
        std::cout<<"\033[K\033[1;32mCongratulations you are the winner\n\033[K";
    }else{
        std::cout<<"\033[K\033[1;31mYou loss\n\033[K";
    }
}

Battleship::~Battleship(){
    delete network;
    delete header;
}

int Battleship::getCoordinate(const char * coordinate){
    int x,y;
    if(strlen(coordinate)>2){
        return -1;
    }  
    x = coordinate[1]-'1';
    if(coordinate[0]<'a'){
        y = coordinate[0] - 'A';
    }else{
        y = coordinate[0] - 'a';
    }
    return y*10+x+1;
}

int Battleship::getInput(){
    std::string input;
    int coordinate;
    do{
        getline(std::cin,input);
        coordinate = getCoordinate(input.c_str());
        if(coordinate == -1 || coordinate >= 100){
            std::cout<<"\033[1;31mWrong input\n";
        }
    }while(coordinate == -1 || coordinate >= 100);
    return coordinate;
}



void Battleship::attack(int pos){
    network->sendMessage(*header,std::to_string(pos).c_str());
    char buffer[8];
    network->receiveMessage(buffer);
    int state = atoi(buffer);

    if(state == filled){
        enemyGrid.at(pos) = destroyed;
        yourTurn = !yourTurn;
        enemyShipCells--;
        if(enemyShipCells == 0){
            gameover = true;
            winner = true;
        }
    }else if(state == empty){
        enemyGrid.at(pos) = failed;
        yourTurn = !yourTurn;
    }
}

void Battleship::updateGrid(int pos){
    network->sendMessage(*header,std::to_string(grid.at(pos)).c_str());
    if(grid.at(pos) == filled){
        grid.at(pos) = destroyed;
        yourTurn = !yourTurn;
        shipCells--;
        if(shipCells == 0){
            gameover = true;
            winner = false;
        }
    }else if(grid.at(pos) == empty){
        grid.at(pos) = failed;
        yourTurn = !yourTurn;
    }
}


void Battleship::setShip(Ships ship){
    bool positioned = false;
    while(!positioned){
        int x = rand()%10;
        int y = rand()%10;
        int orentiation = rand()%2;
        if(orentiation == vertical && y <= 10-ship){
            bool filledCell = false;
            for(int i = y; i < y+ship; i++){
                if(grid.at(i*10+x) == filled){
                    filledCell = true;
                    break;
                }
            }
            if(!filledCell){
                for(int i = y; i < y+ship; i++){
                    grid.at(i*10+x) = filled;
                }
                positioned = true;
            }
        }else if(orentiation == horizontal && x <= 10-ship){
            bool filledCell = false;
            for(int i = x; i < x+ship; i++){
                if(grid.at(y*10+i) == filled){
                    filledCell = true;
                    break;
                }
            }
            if(!filledCell){
                for(int i = x; i < x+ship; i++){
                    grid.at(y*10+i) = filled;
                }
                positioned = true;
            }
        }
    }
}

void Battleship::setGrid(){
    for(int i = 0; i < 100; i++){
        grid.push_back(empty);
        enemyGrid.push_back(empty);
    }
    setShip(aricrafCarrier);
    setShip(battleship);
    setShip(cruiser);
    setShip(destroyer);
    setShip(destroyer);
    setShip(submarine);
    setShip(submarine);
}


void Battleship::printCell(CellState state){
    if(state == empty){
        std::cout<<"\033[34m░░░\033[0m";
    }else if(state == filled){
        std::cout<<"\033[35m███\033[0m";
    }else if(state == destroyed){
        std::cout<<"\033[48:5:196m   \033[0m";
    }else{
        std::cout<<"\033[1;30m▒▒▒\033[0m";
    }
}

void Battleship::printGrid(){
    std::cout<<"\033[0;0f";
    std::cout<<"\033[K\033[32m                Your Board                \033[0m\n";
    std::cout<<"\033[K\033[1;32m   0   1   2   3   4   5   6   7   8   9  \033[0m\n";
    std::cout<<"\033[K\033[1;34m ╔═══╦═══╦═══╦═══╦═══╦═══╦═══╦═══╦═══╦═══╗\033[0m\n";
    for(int i = 0; i < 10; i++){
        std::cout<<"\033[K\033[1;32m"<<(char)('A'+i)<<"\033[0m";
        for(int j = 0; j <10; j++){
            std::cout<<"\033[1;34m║\033[0m";
            printCell(grid[i*10+j]);
        }
        std::cout<<"\033[1;34m║\033[0m\n";
        if(i<9){
            std::cout<<"\033[K\033[1;34m ╠═══╬═══╬═══╬═══╬═══╬═══╬═══╬═══╬═══╬═══╣\033[0m\n";
        }
    }
    std::cout<<"\033[K\033[1;34m ╚═══╩═══╩═══╩═══╩═══╩═══╩═══╩═══╩═══╩═══╝\033[0m\n\n";
    std::cout<<"\033[K\033[31m               Enemy Board                \033[0m\n";
    std::cout<<"\033[K\033[1;32m   0   1   2   3   4   5   6   7   8   9  \033[0m\n";
    std::cout<<"\033[K\033[1;34m ╔═══╦═══╦═══╦═══╦═══╦═══╦═══╦═══╦═══╦═══╗\033[0m\n";
    for(int i = 0; i < 10; i++){
        std::cout<<"\033[K\033[1;32m"<<(char)('A'+i)<<"\033[0m";
        for(int j = 0; j <10; j++){
            std::cout<<"\033[1;34m║\033[0m";
            printCell(enemyGrid[i*10+j]);
        }
        std::cout<<"\033[1;34m║\033[0m\n";
        if(i<9){
            std::cout<<"\033[K\033[1;34m ╠═══╬═══╬═══╬═══╬═══╬═══╬═══╬═══╬═══╬═══╣\033[0m\n";
        }
    }
    std::cout<<"\033[K\033[1;34m ╚═══╩═══╩═══╩═══╩═══╩═══╩═══╩═══╩═══╩═══╝\033[0m\n";
}
