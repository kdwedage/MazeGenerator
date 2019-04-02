/*****************************
* Names: Kevin Wedage, Aryan Singh
* IDs: 1532557, 1533732
* Comput 275, Winter 2019
* Final Project
* Based off of weblog.jamisbuck.org/2010/12/27/maze-generation-recursive-backtracking
*****************************/

#ifndef _RECURSIVEBACK_H_
#define _RECURSIVEBACK_H_
#include "util.h"
#include "serialport.h"
#include <iostream>
#include <stack>

using namespace std;

void sendAndRecieve(SerialPort& Serial, string outputMessage);
void navigate(Grid *grid, stack <pair<int,int>>& locationHistory, pair<int, int> currentLocation, SerialPort& Serial);
void recursiveBacktracking(Grid *grid, SerialPort& Serial);

#endif