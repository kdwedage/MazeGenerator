#ifndef _UTIL_H_
#define _UTIL_H_

#include <cstdlib>
#include <ctime>
#include <utility>
#include <vector>
#include <string>

using namespace std;


struct Cell {
    bool isVisited = false;
    vector <char> walls = {'T','R','B','L'}; //Top Right Bottom Left
};

class Grid{
private:
    int width, height;
    vector <vector<Cell>> grid;
    bool * getVisitedWalls(pair <int, int> location);
public:
    Grid(int w, int h);

    char getRandomUnvisitedWall(pair<int,int> location);

     // first is x, second is y 0,0 is top left of grid
    bool getIsVisited(pair<int,int> location){
        if(location.first == -1){
            return true; // It is border
        }
        return grid.at(location.second).at(location.first).isVisited;
    }

    void removeWall(pair <int, int> location, char w);

    int getSize(){
        return width*height;
    }

    int getWidth(){
        return width;
    }
    int getHeight(){
        return height;
    }

    void setVisited(pair <int, int> location){
        grid.at(location.second).at(location.first).isVisited = true;
    }

    char wallOpposite(char w);

    pair<int,int> getTopLocation(pair <int, int> location);
    pair<int,int> getRightLocation(pair <int, int> location);
    pair<int,int> getBottomLocation(pair <int, int> location);
    pair<int,int> getLeftLocation(pair <int, int> location);


};

void initializeSeed();

int randomInt(int max);

int * parseRequest(string& line);


#endif