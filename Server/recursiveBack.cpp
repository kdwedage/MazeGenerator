/*****************************
* Names: Kevin Wedage, Aryan Singh
* IDs: 1532557, 1533732
* Comput 275, Winter 2019
* Final Project
* Based off of weblog.jamisbuck.org/2010/12/27/maze-generation-recursive-backtracking
*****************************/

#include "recursiveBack.h"

using namespace std;

// Sends string to serial, and waits for acknowledgment
void sendAndRecieve(SerialPort& Serial, string outputMessage){
    Serial.writeline(outputMessage);
    string line;
    do{
        line = Serial.readline();
    }while (line.substr(0,1) != "A"); // Waits for acknowledgment
}

// Navigates to new location on Grid
void navigate(Grid *grid, stack <pair<int,int>>& locationHistory, pair<int, int> currentLocation, SerialPort& Serial){

    while(true){
        
        char wall = grid->getRandomUnvisitedWall(currentLocation);
        
        pair <int, int> newLocation;

        bool noMoreWalls = false;

        switch(wall){
            case 'L':
                newLocation = grid->getLeftLocation(currentLocation);
                break;
            case 'R':
                newLocation = grid->getRightLocation(currentLocation);
                break;
            case 'B':
                newLocation = grid->getBottomLocation(currentLocation);
                break;
            case 'T':
                newLocation = grid->getTopLocation(currentLocation);
                break;
            default:
                noMoreWalls = true;
                break;
        }


        if(!noMoreWalls){ // There is walls
            grid->removeWall(currentLocation, wall);
            if(newLocation.first != -1){ // If the wall is a boundary wall (then newLocation.first == -1)
                grid->removeWall(newLocation, grid->wallOpposite(wall));   
            }else{ // Get another wall, if newlocation is boundary
                continue;
            }
            // Creates the output message, to be sent over serial
            string outputMessage = "M " + to_string(grid->wallOpposite(wall))
             + " " + to_string(newLocation.first) + " " + to_string(newLocation.second) + "\n";

            cout << outputMessage;
            sendAndRecieve(Serial, outputMessage); // Sends recursive movement and waits for acknowledgment
            grid->setVisited(newLocation);
            locationHistory.push(currentLocation);
            navigate(grid, locationHistory, newLocation, Serial); // Navigates to new location
        }else{
            // If there is no more walls
            // If we reached the starting spot, then finished algorithm
            if(currentLocation.first == 0 && currentLocation.second == 0){
                return;
            }else{ // Otherwise go to the previous location
                pair <int, int> previousLocation = locationHistory.top();
                locationHistory.pop();
                navigate(grid, locationHistory, previousLocation, Serial);
            }
        }
        break; // Break if we don't need to get another wall
    }
    
}

// Starts the recursive backtracking algorithm
void recursiveBacktracking(Grid *grid, SerialPort& Serial){
    grid->setVisited(make_pair(0,0)); // Sets the top left square to be visited (start square)
    stack <pair<int,int>> locationHistory; // Creates a stack of the locations visited
    locationHistory.push(make_pair(0,0));
    navigate(grid, locationHistory, make_pair(0,0), Serial); // Set the start location
    sendAndRecieve(Serial, "E\n"); // For End
}
