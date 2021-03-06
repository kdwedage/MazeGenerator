/*****************************
* Names: Kevin Wedage, Aryan Singh
* IDs: 1532557, 1533732
* Comput 275, Winter 2019
* Final Project
*****************************/

#include "util.h"

// Initializes the random seed using time
void initializeSeed(){
    srand(time(NULL));
}

// Gets a random int from 0 to max, does not include max
int randomInt(int max){
    return rand() % max;
}

// Parses string for two integers
int * parseRequest(string& line){
    int wordCount = 0;
    string str[2];
    static int numbers[2];

    string temp;
    for(auto it = line.begin(); it != line.end(); ++it){
        temp = *it;
        if(temp == " "){ // If space is reached, increment word count
            wordCount++;
        }else{
            str[wordCount] += temp;
        }
    }

    // Convert strings to ints
    for(int i = 0; i < 2; i++)
        numbers[i] = stoi(str[i]);
    return numbers;
}

// Grid constructor that initalizes the member variables
Grid::Grid(int w, int h){
    width = w;
    height = h;

    for(int y = 0; y < h; y++){
        vector<Cell> rowTemp;
        for(int x = 0; x < w; x++){
            Cell newCell;
            rowTemp.push_back(newCell);
        }
        grid.push_back(rowTemp);
    }
}

// Removes a random unvisited wall for a location in the grid
char Grid::getRandomUnvisitedWall(pair <int, int> location){
    Cell cell = grid.at(location.second).at(location.first);

    char side;

    vector <char> walls = cell.walls; 

    if(walls.size() == 0){ // No walls
        return 'X';
    }

    vector <char> notVisitedWalls;

    // Check if exisiting walls is visited
    pair <int, int> testLocation;
    for( auto i : walls){
        if(i == 'T'){
            testLocation = getTopLocation(location);
            if(!getIsVisited(testLocation)){
                notVisitedWalls.push_back(i);
            }
        }else if(i == 'R'){
            testLocation = getRightLocation(location);
            if(!getIsVisited(testLocation)){
                notVisitedWalls.push_back(i);
            }
        }else if(i == 'B'){
            testLocation = getBottomLocation(location);
            if(!getIsVisited(testLocation)){
                notVisitedWalls.push_back(i);
            }
        }else if(i == 'L'){
            testLocation = getLeftLocation(location);
            if(!getIsVisited(testLocation)){
                notVisitedWalls.push_back(i);
            }
        }
    }

    if(notVisitedWalls.size() == 0){
        return 'X'; // No unvisited walls
    }


    // Gets random wall index
    int randIndex = randomInt(notVisitedWalls.size());


    side = notVisitedWalls.at(randIndex);
    return side;
}

// Removes the wall at the location in the grid
void Grid::removeWall(pair <int, int> location, char w){
    Cell cell = grid.at(location.second).at(location.first);
    
    // Looks for wall, if found erases it from walls
    for(auto iter = cell.walls.begin(); iter != cell.walls.end(); iter++){
        if(*iter == w){
            cell.walls.erase(iter);
            break;
        }
    }

    grid.at(location.second).at(location.first) = cell;
}

// Gets the location above the current location
pair<int,int> Grid::getTopLocation(pair <int, int> location){
    pair <int, int> newLocation;
    if(location.second <= 0){
        newLocation.first = -1;
        newLocation.second = -1;
    }else{
        newLocation.first = location.first;
        newLocation.second = location.second - 1;
    }
    return newLocation;
}

// Gets the location to the right of the current location
pair<int,int> Grid::getRightLocation(pair <int, int> location){
    pair <int, int> newLocation;
    if(location.first >= (width - 1)){
        newLocation.first = -1;
        newLocation.second = -1;
    }else{
        newLocation.first = location.first + 1;
        newLocation.second = location.second;
    }
    return newLocation;
}

// Gets the location below the current location
pair<int,int> Grid::getBottomLocation(pair <int, int> location){
    pair <int, int> newLocation;
    if(location.second >= (height - 1)){
        newLocation.first = -1;
        newLocation.second = -1;
    }else{
        newLocation.first = location.first;
        newLocation.second = location.second + 1;
    }
    return newLocation;
}

// Gets the location to the left of the current location
pair<int,int> Grid::getLeftLocation(pair <int, int> location){
    pair <int, int> newLocation;
    if(location.first <= 0){
        newLocation.first = -1;
        newLocation.second = -1;
    }else{
        newLocation.first = location.first - 1;
        newLocation.second = location.second;
    }
    return newLocation;
}

// Returns the wall opposite to the wall passed in.
char Grid::wallOpposite(char w){
    switch(w){
        case 'L':
            return 'R';
        case 'R':
            return 'L';
        case 'B':
            return 'T';
        case 'T':
            return 'B';
        default:
            return w;
    }
}