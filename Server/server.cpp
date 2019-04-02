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


// Max Grid size is 200 by 200


void makeGrid(int w, int h, SerialPort& Serial){

    if(w <= 0 || h <= 0){
        cout << "Invalid dimensions. Width and height must be > 0" << endl;
        return;
    }
    Grid grid(w,h);
    recursiveBacktracking(&grid, Serial);
}

void manageSerial(SerialPort& Serial){
    string line;
    while(true){
        do {
            line = Serial.readline();
        }while (line.substr(0,1) != "R");

        cout << "Server: Request recieved" << endl;
        line = line.substr(2, line.size());
        int *values = parseRequest(line); // Determines the values in the request
        makeGrid(*(values), *(values + 1), Serial);
    }
}

int main(int argc, char** argv){
    string port = "/dev/ttyACM";
    if(argc > 1){
        port += argv[1];
    }else{
        port += '0';
    }
    SerialPort Serial(port.c_str());
    initializeSeed();
    manageSerial(Serial);
    return 0;
}