/*****************************
* Names: Kevin Wedage, Aryan Singh
* IDs: 1532557, 1533732
* Comput 275, Winter 2019
* Final Project
*****************************/

#ifndef _UTIL_H_
#define _UTIL_H_

#include <cstdlib>
#include <ctime>
#include <utility>
#include <vector>
#include <string>

using namespace std;

// A cell is an element in the Grid
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

    // Returns a random unvisited wall
    char getRandomUnvisitedWall(pair<int,int> location);

    // Checks if a location on the grid is visited before
    bool getIsVisited(pair<int,int> location){
        if(location.first == -1){
            return true; // It is border
        }
        return grid.at(location.second).at(location.first).isVisited;
    }

    // Removes the coresponding wall
    void removeWall(pair <int, int> location, char w);

    // Gets the size of the grid
    int getSize(){
        return width*height;
    }

    // Gets the width of the grid
    int getWidth(){
        return width;
    }

    // Gets the height of the grid
    int getHeight(){
        return height;
    }

    // Sets a location to be visited
    void setVisited(pair <int, int> location){
        grid.at(location.second).at(location.first).isVisited = true;
    }

    // Gets the wall opposite to the current wall. (wallOpposite(Right) = left)
    char wallOpposite(char w);

    // Gets the coordinates of the coresponding location
    pair<int,int> getTopLocation(pair <int, int> location);
    pair<int,int> getRightLocation(pair <int, int> location);
    pair<int,int> getBottomLocation(pair <int, int> location);
    pair<int,int> getLeftLocation(pair <int, int> location);


};

// Initializes the random seed
void initializeSeed();

// Gets a random int from 0 to max
int randomInt(int max);

// Parses a string request into two ints
int * parseRequest(string& line);


#endif