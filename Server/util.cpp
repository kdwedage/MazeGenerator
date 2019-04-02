#include "util.h"

void initializeSeed(){
    srand(time(NULL));
}

// Does not include max
int randomInt(int max){
    return rand() % max;
}

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


char Grid::getRandomUnvisitedWall(pair <int, int> location){
    Cell cell = grid.at(location.second).at(location.first);

    char side;

    vector <char> walls = cell.walls; 

    if(walls.size() == 0){
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
        return 'X'; // No more walls
    }


    // Gets random wall index
    int randIndex = randomInt(notVisitedWalls.size());


    side = notVisitedWalls.at(randIndex);
    return side;
}

void Grid::removeWall(pair <int, int> location, char w){
    Cell cell = grid.at(location.second).at(location.first);
    
    // Looks for char, if found erases it from walls
    for(auto iter = cell.walls.begin(); iter != cell.walls.end(); iter++){
        if(*iter == w){
            cell.walls.erase(iter);
            break;
        }
    }

    grid.at(location.second).at(location.first) = cell;
}

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