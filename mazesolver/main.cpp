#include <array>
#include <cstring>
#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <stack>

#include <SFML/Graphics.hpp>

const int windowWidth = 1300, windowHeight = 650;

// size of single cell
const int cellSize = 10;

// number of cells horizontally and vertically
const int numCellsX = windowWidth / cellSize;
const int numCellsY = windowHeight / cellSize;

// window dimensions
// start position of solver
int startX = 0, startY = 0;
int destX = numCellsX-1, destY = numCellsY-1;

typedef sf::Sprite Maze[numCellsX][numCellsY];
typedef int MazeData[numCellsX][numCellsY];

// each cell has 8 possible direction of movements
// this will store which ones are blocked and which ones are empty
typedef int PossibleDirections[8];

// possible directions index in above data type
enum Direction {
    LEFT = 0,
    RIGHT = 1,
    UP = 2,
    DOWN = 3,
    UP_LEFT = 4,
    DOWN_LEFT = 5,
    UP_RIGHT = 6,
    DOWN_RIGHT = 7,
    DEADLOCK = 8,
    START = 9,
};

// linked stack storing path
std::stack<Direction> path;

enum CellState {
    CELL_BLOCK = -1,
    CELL_EMPTY = 0,
    CELL_PATH = 1,
    CELL_VISITED = 2,
};

// colors
static const sf::Color PATH_COLOR = sf::Color(0, 185, 242);
// static const sf::Color BLOCK_COLOR = sf::Color(150, 100, 150);
static const sf::Color EMPTY_COLOR = sf::Color(16, 16, 16);
static const sf::Color BLOCK_COLOR = sf::Color(22, 97, 190);
static const sf::Color VISITED_COLOR = sf::Color(0x50, 0x7d, 0x2a); // sap green

// update maze with given data
void updateMaze(Maze& maze, MazeData& mazeData) {
    for(int i = 0; i < numCellsX;i++){
        for(int j = 0; j < numCellsY; j++){
            if(mazeData[i][j] == CELL_BLOCK){
                maze[i][j].setColor(BLOCK_COLOR);
            }else if(mazeData[i][j] == CELL_EMPTY){
                maze[i][j].setColor(EMPTY_COLOR);
            }else if(mazeData[i][j] == CELL_PATH){
                maze[i][j].setColor(PATH_COLOR);
            }else{
                maze[i][j].setColor(VISITED_COLOR);
            }
        }
    }
}

// create maze
void createMaze(Maze& maze){
    sf::Image img;
    img.create(cellSize, cellSize);

    sf::Texture tex;
    tex.loadFromImage(img);

    for(int i = 0; i < numCellsX;i++){
        for(int j = 0; j < numCellsY; j++){
            maze[i][j] = sf::Sprite();
            maze[i][j].setPosition(i*cellSize, j*cellSize);
            maze[i][j].setTexture(tex);
            maze[i][j].setColor(EMPTY_COLOR);
        }
    }
}

void fillMaze(MazeData& mazeData, CellState state){
    for(int i = 0; i < numCellsX; i++){
        for(int j = 0; j < numCellsY; j++){
            mazeData[i][j] = state;
        }
    }
}

// resets maze as if no one walked on it! heavy CIA stuff!!
void resetMaze(MazeData& mazeData){
    for(int i = 0; i < numCellsX; i++){
        for(int j = 0; j < numCellsY; j++){
            // this was a path! now this must be empty!!
            if(mazeData[i][j] == CELL_PATH || mazeData[i][j] == CELL_VISITED) mazeData[i][j] = CELL_EMPTY;
        }
    }
}

// get list of all available directions that this guy can move in
void getPossibleDirections(MazeData& mazeData, int posX, int posY, PossibleDirections& dirs){
    // initially we cannot move in any direction, so set all directions to false
    std::memset(dirs, 0, sizeof(PossibleDirections));

    if(posX + 1 < numCellsX){
        if(mazeData[posX+1][posY] == CELL_EMPTY)
            dirs[RIGHT] = 1;
    }

    if(posX - 1 >= 0){
        if(mazeData[posX-1][posY] == CELL_EMPTY)
            dirs[LEFT] = 1;
    }

    if(posY + 1 < numCellsY){
        if(mazeData[posX][posY+1] == CELL_EMPTY)
            dirs[DOWN] = 1;
    }

    if(posY - 1 >= 0){
        if(mazeData[posX][posY-1] == CELL_EMPTY)
            dirs[UP] = 1;
    }

    if((posX+1 < numCellsX) && (posY+1 < numCellsY)){
        if(mazeData[posX+1][posY+1] == CELL_EMPTY)
            dirs[DOWN_RIGHT] = 1;
    }

    if((posX+1 < numCellsX) && (posY-1 >= 0)){
        if(mazeData[posX+1][posY-1] == CELL_EMPTY)
            dirs[UP_RIGHT] = 1;
    }

    if((posX-1 >= 0) && (posY+1 < numCellsY)){
        if(mazeData[posX-1][posY+1] == CELL_EMPTY)
            dirs[DOWN_LEFT] = 1;
    }

    if((posX-1 >= 0) && (posY-1 >= 0)){
        if(mazeData[posX-1][posY-1] == CELL_EMPTY)
            dirs[UP_LEFT] = 1;
    }
}

Direction selectRandomDirection(PossibleDirections& dirs){
    // check if any movable direction exists or not
    bool exist = false;
    for(int i = 0; i < 8; i++){
        if(dirs[i] == 1){
            exist = true;
            break;
        }
    }

    // if not exists then this is a deadlock
    if(!exist) return DEADLOCK;

    // select random direction
    Direction dir = Direction(rand() % 8);
    if(dirs[dir] == 1){
        return dir;
    }else{
        return selectRandomDirection(dirs);
    }
}

Direction selectDirection(PossibleDirections& dirs){
    for(int i = 0; i < 8; i++){
        if(dirs[i] == 1){
            return Direction(i);
        }
    }
    return DEADLOCK;
}

// converts given direction into vector <X, Y>
void getDirectionVector(Direction dir, int& X, int& Y){
    X = 0;
    Y = 0;
    switch(dir){
    case RIGHT:
        X = 1;
        break;
    case LEFT:
        X = -1;
        break;
    case UP:
        Y = -1;
        break;
    case DOWN:
        Y = 1;
        break;
    case UP_RIGHT:
        X = 1;
        Y = -1;
        break;
    case DOWN_RIGHT:
        X = 1;
        Y = 1;
        break;
    case UP_LEFT:
        X = -1;
        Y = -1;
        break;
    case DOWN_LEFT:
        X = -1;
        Y = 1;
        break;
    }
}

// take one step towards finding maze solution
bool mazeStep(MazeData& mazeData, bool restart = false){

    static int posX = startX, posY = startY;
    static PossibleDirections dirs;

    if(restart){
        posX = startX;
        posY = startY;
    }


    getPossibleDirections(mazeData, posX, posY, dirs);
    Direction dir = selectRandomDirection(dirs);
    // Direction dir = selectDirection(dirs);
    int dirX, dirY;

    if(dir == DEADLOCK){
        // get last direction
        dir = path.top();
        path.pop();

        // get relative direction vector
        getDirectionVector(dir, dirX, dirY);

        // mark this cell as visited
        mazeData[posX][posY] = CELL_VISITED;

        // reverse direction
        posX -= dirX;
        posY -= dirY;
    }else{
        // add this step to path
        path.push(dir);

        // move
        getDirectionVector(dir, dirX, dirY);
        posX += dirX;
        posY += dirY;
    }


    // maze solved! yay!!
    if(posX == destX && posY == destY){
        std::cout << "MAZE IS NOW SOLVED!" << std::endl;
        mazeData[destX][destY] = CELL_PATH;
        return false;
    }

    // set current position as path block
    if(posX < 0 || posY < 0 || posX >= numCellsX || posY >= numCellsY) return false;
    mazeData[posX][posY] = CELL_PATH;

    return true;
}

void generateMaze(MazeData& mazeData, int seed){
    for(int i = 0; i < numCellsX; i++){
        for(int j = 0; j < numCellsY; j++){
            if(rand() % seed == 0) mazeData[i][j] = CELL_BLOCK;
            else mazeData[i][j] = CELL_EMPTY;
        }
    }
}

// draw maze
void drawMaze(sf::RenderWindow& window, Maze& maze){
    for(int i = 0; i < numCellsX; i++){
        for(int j = 0; j < numCellsY; j++){
            window.draw(maze[i][j]);
        }
    }
}

int main() {
    // render window
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "MazeRunner", sf::Style::Titlebar | sf::Style::Close);

    // maze
    Maze maze;
    MazeData mazeData;

    // create maze graphical representation
    createMaze(maze);

    // fill maze data with all block cells, except the starting one
    srand(time(nullptr));
    generateMaze(mazeData, rand()%10+5);
    mazeData[startX][startY] = CELL_EMPTY;

    // update maze representation according to maze data
    updateMaze(maze, mazeData);

    window.setFramerateLimit(120);

    bool movable = true;
    sf::Time pauseTime = sf::seconds(5);
    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed){
                window.close();
            }
        }

        window.clear(sf::Color::Black);

        movable = mazeStep(mazeData);
        updateMaze(maze, mazeData);

        if(movable == false) {
            std::cout << "DEADLOCK!" << std::endl;
            sf::sleep(pauseTime); // take a pause
            generateMaze(mazeData, (rand()%10)+5);

            // empty out stack
            while(!path.empty()) path.pop();

            movable = mazeStep(mazeData, true); // restart maze
            updateMaze(maze, mazeData);
        }

        drawMaze(window, maze);

        window.display();
    }

    return 0;
}
