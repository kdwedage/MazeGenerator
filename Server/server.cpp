/*****************************
* Names: Kevin Wedage, Aryan Singh
* IDs: 1532557, 1533732
* Comput 275, Winter 2019
* Final Project
*****************************/

#include <iostream>
#include "util.h"
#include "serialport.h"
#include "recursiveBack.h"

using namespace std;

// Makes a grid, checking that dimensions are appropriate
void makeGrid(int w, int h, SerialPort& Serial){

    if(w <= 0 || h <= 0){
        cout << "Invalid dimensions. Width and height must be > 0" << endl;
        return;
    }
    Grid grid(w,h);
    recursiveBacktracking(&grid, Serial);
}

// Watches the serial for a request
void manageSerial(SerialPort& Serial){
    string line;
    while(true){
        do {
            line = Serial.readline();
        }while (line.substr(0,1) != "R"); // Waits for a request

        cout << "Server: Request recieved" << endl;
        line = line.substr(2, line.size());
        int *values = parseRequest(line); // Determines the values in the request
        makeGrid(*(values), *(values + 1), Serial);
    }
}

int main(int argc, char** argv){
    string port = "/dev/ttyACM";
    // If additional parameters was passed in, then set the port to the first additional value passed in.
    if(argc > 1){
        port += argv[1];
    }else{
        port += '0';
    }
    SerialPort Serial(port.c_str());
    initializeSeed(); // Initalize the random seed.
    manageSerial(Serial);
    return 0;
}