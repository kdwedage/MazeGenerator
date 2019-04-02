#include "recursiveBack.h"

using namespace std;


void sendAndRecieve(SerialPort& Serial, string outputMessage){
    Serial.writeline(outputMessage);
    string line;
    do{
        line = Serial.readline();
    }while (line.substr(0,1) != "A"); // Waits for acknowledgement
}

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
            if(newLocation.first != -1){
                grid->removeWall(newLocation, grid->wallOpposite(wall));   
            }else{ // Get another wall, if newlocation is boundary
                continue;
            }
            string outputMessage = "M " + to_string(grid->wallOpposite(wall))
             + " " + to_string(newLocation.first) + " " + to_string(newLocation.second) + "\n";

            cout << outputMessage;
            sendAndRecieve(Serial, outputMessage); // Sends recursive movement and waits for acknowledgment
            grid->setVisited(newLocation);
            locationHistory.push(currentLocation);
            navigate(grid, locationHistory, newLocation, Serial);
        }else{
            //cout << "No more walls at : " << currentLocation.first << "," << currentLocation.second << ")" << endl;
            if(currentLocation.first == 0 && currentLocation.second == 0){
                return;
            }else{
                pair <int, int> previousLocation = locationHistory.top();
                locationHistory.pop();
                navigate(grid, locationHistory, previousLocation, Serial);
            }
        }
        sendAndRecieve(Serial, "E\n"); // For End
        break; // Break if we don't need to get another wall
    }
    
}


void recursiveBacktracking(Grid *grid, SerialPort& Serial){
    grid->setVisited(make_pair(0,0));
    stack <pair<int,int>> locationHistory;
    locationHistory.push(make_pair(0,0));
    navigate(grid, locationHistory, make_pair(0,0), Serial); // Set the start location
}
