/*****************************
* Names: Kevin Wedage, Aryan Singh
* IDs: 1532557, 1533732
* Comput 275, Winter 2019
* Final Project
*****************************/

/* Based off of simpleclient for assignment 2 part 2, from eclass, and a1-2.cpp solution for assignment 1 part2*/

#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>
#include <TouchScreen.h>
#include <SD.h>

#define TFT_DC 9
#define TFT_CS 10
#define SD_CS 6
#define BUTTON_PIN 3

// joystick pins
#define JOY_VERT_ANALOG A1
#define JOY_HORIZ_ANALOG A0
#define JOY_SEL 8

// width/height of the display when rotated horizontally
#define TFT_WIDTH 320
#define TFT_HEIGHT 240

// constants for the joystick
#define JOY_DEADZONE 64
#define JOY_CENTRE 512
#define JOY_STEPS_PER_PIXEL 64

// touch screen pins, obtained from the documentaion
#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM  5  // can be a digital pin
#define XP  4  // can be a digital pin


// Initalize global varables for display
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// max size of buffer, including null terminator
const uint16_t buf_size = 256;
// current number of chars in buffer, not counting null terminator
uint16_t buf_len = 0;

// input buffer
char* buffer = (char *) malloc(buf_size);

// Mode to define state
int mode = 0;
bool isInTransition = true;
bool finishedDrawing = false;
// Maze width and height
int width = 1;
int height = 1;
bool isScaled = false;

// Forward declartions
void manageSerial();
void process_line();
void setupDisplay();
void displayMode0();
void displayMode1();
void drawSquare();
int * parseRequestStringToInt();
int stringToInt(String& str);

void setup() {
  Serial.begin(9600);

  // Initialization joystick
  pinMode(JOY_SEL, INPUT_PULLUP);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Setup tft and display
  setupDisplay();

  // set up buffer as empty string
  buf_len = 0;
  buffer[buf_len] = 0;
}

// Main loop
void loop() {
  if(mode == 0)
    displayMode0();
  else{
    manageSerial();
    displayMode1();
  }
}

// Checks serial and saves it to buffer
void manageSerial(){
  char in_char;
  if (Serial.available()) {
    // read the incoming byte:
    char in_char = Serial.read();

    // if end of line is received, waiting for line is done:
    if (in_char == '\n' || in_char == '\r') {
      // now we process the buffer
      process_line();
    }else {
      // add character to buffer, provided that we don't overflow.
      // drop any excess characters.
      if ( buf_len < buf_size-1 ) {
        buffer[buf_len] = in_char;
        buf_len++;
        buffer[buf_len] = 0;
      }
    }
  }
}

// Processes the Buffer
void process_line() {
  // M: for Movement
  if(buffer[0] == 'M'){
    drawSquare();
    Serial.println("A");
    finishedDrawing = false;
  // E: for End
  }else if(buffer[0] == 'E'){
    Serial.println("A");
    finishedDrawing = true;
  }

  // clear the buffer
  buf_len = 0;
  buffer[buf_len] = 0;
}

void setupDisplay(){
  tft.begin();
  tft.setRotation(-1);
  tft.fillScreen(ILI9341_BLACK);
}

// Mangages the display when in Mode 0
void displayMode0(){
  // Reads the joystick values
  int v = analogRead(JOY_VERT_ANALOG);
  int h = analogRead(JOY_HORIZ_ANALOG);
  int invSelect = digitalRead(JOY_SEL);

  // Flags for determining whether to redraw
  bool buttonChanged = false;
  bool joy_moved = false;

  int buttonClick = digitalRead(BUTTON_PIN);
  if(buttonClick == LOW){ // Sets ratio to be fixed
    isScaled = !isScaled;
    buttonChanged = true;
    delay(500);
  }


  if(buttonChanged){
    tft.setTextColor(0xFFE0, ILI9341_BLACK);
    tft.setCursor(0, TFT_HEIGHT/2 + 30);
    tft.setTextSize(1);
    if(isScaled){
        tft.print("\tDimension ratio fixed");
    }else{ // Clears text
        tft.print("                        ");
    }
  }

  // Updates maze height (and width if fixed ratio)
  if (abs(v - JOY_CENTRE) > JOY_DEADZONE) {

    int delta = (v - JOY_CENTRE) / JOY_STEPS_PER_PIXEL;
    height = constrain(height + delta, 1, 200);
    
    if(isScaled){
        width = height*TFT_WIDTH/TFT_HEIGHT;
        width = constrain(width, 1, 200);   
    }
    joy_moved = true;
  }

  // Updates maze width (and height if fixed ratio)
  if (abs(h - JOY_CENTRE) > JOY_DEADZONE) {

    int delta = -(h - JOY_CENTRE) / JOY_STEPS_PER_PIXEL;
    width = constrain(width + delta, 1, 200);
    
    if(isScaled){
        height = width*TFT_HEIGHT/TFT_WIDTH;
        height = constrain(height, 1, 200);        
    }

    joy_moved = true;
  }

  // If the screen is in transition, then only draw unchanging text
  if(isInTransition){
    tft.setTextColor(0x07E0, ILI9341_BLACK);
    tft.setCursor(0,0);
    tft.setTextSize(3);
    tft.print("Welcome to Maze \n     Creator!\n\n");
    tft.setTextSize(2);
    tft.setTextColor(0xF81F, ILI9341_BLACK);
    tft.print("by Kevin Wedage\n and Aryan Singh ");

  }

  // Display the maze width and height
  if(joy_moved || isInTransition){
    tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setCursor(0, TFT_HEIGHT - 20);
    tft.print("Width : " + String(width) + ", Height : " + String(height) + "    ");

    isInTransition = false;
  }

  // Switch modes
  if(invSelect == LOW){

    tft.setTextColor(0x07FF, ILI9341_BLACK);
    tft.setCursor(0, TFT_HEIGHT/2);
    tft.setTextSize(2);
    tft.print("Creating maze ... ");

    delay(500);
    mode = 1;
    isInTransition = true;
  }
}

// Mangages the display when in Mode 1
void displayMode1(){
    if(isInTransition){
        tft.fillScreen(ILI9341_BLACK);
        isInTransition = false;

        // Draw the first square, because the server only sends new locations. 
        float squarePixelWidth = (float)TFT_WIDTH/width;
        float squarePixelHeight = (float)TFT_HEIGHT/height;
        tft.fillRect(0,0, squarePixelWidth, squarePixelHeight, 0xF81F);

        Serial.println("R " + String(width) + " " + String(height));
    }
}

void drawSquare(){
    // Insures there is something to draw and not End statement
    if(buf_len <= 3){ 
        return;
    }

    // Parses the buffer, for the wall destroyed, and the x and y coordinates of the new location
    int *values = parseRequestStringToInt();
    // Determines the dimensions of square in the maze
    float squarePixelWidth = (float)TFT_WIDTH/width;
    float squarePixelHeight = (float)TFT_HEIGHT/height;

    // Get top left position of current square
    int x = *(values + 1)*squarePixelWidth;
    int y = *(values + 2)*squarePixelHeight;

    // Fill square (Without the border)
    tft.fillRect(x + 1, y + 1, squarePixelWidth - 1, squarePixelHeight - 1, 0xF81F);

    // Get the wall to remove
    char wall = char(*(values));

    // Removes the coresponding wall, by redrawing line ontop
    int wallColor = 0xF81F;
    switch(wall){
        case 'R':
            tft.drawFastVLine(x + squarePixelWidth, y + 1, squarePixelHeight - 1, wallColor);
            tft.drawFastVLine(x + squarePixelWidth + 1, y + 1, squarePixelHeight - 1, wallColor);
            break;
        case 'L':
            tft.drawFastVLine(x , y + 1, squarePixelHeight - 1, wallColor);
            tft.drawFastVLine(x - 1, y + 1, squarePixelHeight - 1, wallColor);
            break;
        case 'T':
            tft.drawFastHLine(x + 1, y - 1, squarePixelWidth - 1, wallColor);
            tft.drawFastHLine(x + 1, y , squarePixelWidth - 1, wallColor);
            break;
        case 'B':
            tft.drawFastHLine(x + 1, y + squarePixelHeight , squarePixelWidth - 1, wallColor);
            tft.drawFastHLine(x + 1, y + squarePixelHeight + 1, squarePixelWidth - 1, wallColor);
            break;
        default:
            break;
    }
}

// Parses the buffer for 3 integers
int * parseRequestStringToInt(){
    int wordCount = 0;
    String str[3];
    static int numbers[3];

    char temp;
    for(int i = 2; i < buf_len; i++){
        temp = buffer[i];
        if(temp == ' '){
            wordCount++;
        }else{
            str[wordCount] += temp;
        }
    }

    for(int i = 0; i < 3; i++)
        numbers[i] = (str[i]).toInt();
    return numbers;
}