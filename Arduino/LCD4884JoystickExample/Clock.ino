/************************************************************
 * Nokia LCD4884 Shield with joystick
 * 
 * This code is based on the example from Yourduino
 * 
 * This is a simple clock, the joystick is used to select hours or minutes
 * pressing the joystick will allow the value selected to be increased or decreased
 * move the joystick left or right to return to clock mode.
 * Seconds are always reset to zero after hours or minutes have been changed
 * 
 * This display has a maximum of 13 characters per line and a maximum of seven lines
 * two font sizes are available, but the large font only displays numbers and + or -
 * 
 * Increasing the y value by 9 will print on the next line, 
 * any value larger will print two lines below (up to y+18)
 * 
 * Chris Rouse April 2016
 ************************************************************/

#include <LCD4884.h>  // UPDATED version 2/16 Yourduino
#define LCD_BACKLIGHT_PIN  7
int displayDelay = 500;
int switchDelay  = 100;  // Switch scanning delay
int switchVoltage ;   // From Analog read of the button resistors
boolean flip = true; // used to flash character
boolean runFlag = true; // shows clock is in run mode
boolean setClkFlag = false; // shows clock is in set clock mode
boolean debounce = false; // only allows first change of joystick to be recorded
boolean selected = false; // shows joystick button pressed
boolean hrSet = true; // if true set hours otherwise set minutes
unsigned long previousMillis = 0; // will store last time display was updated
const long interval = 1000; // interval at which to blink (milliseconds)
char temp[10]; // used in conversion of integers for printing on display
char joyTemp[10]; //used in conversion of integers for printing on display
int pointer = 1; // used to show the option currently selected
int hrs = 0; // current hour
int mins = 0; // current minute
int secs = 1; // current second
//char zero[1] ; //used
/*********************************************************/

void setup() {
 Serial.begin(9600);
  pinMode(LCD_BACKLIGHT_PIN, OUTPUT);
  lcd.LCD_init(); // creates instance of LCD
  lcd.LCD_clear(); // blanks the display
  for (int a = 0; a < 5; a++)
  {
    digitalWrite(LCD_BACKLIGHT_PIN, LOW);
    delay(300);
    digitalWrite(LCD_BACKLIGHT_PIN, HIGH);
    delay(300);
  }
   lcd.LCD_write_string(2, 0, "Nokia LCD4884", MENU_NORMAL);
   lcd.LCD_write_string(9, 9, "Clock Demo", MENU_NORMAL); 
   lcd.LCD_write_string(0, 18, "-------------", MENU_NORMAL);   
   lcd.LCD_write_string(6, 27, "Use Joystick", MENU_NORMAL); 
   lcd.LCD_write_string(6, 36, "to set clock", MENU_NORMAL); 
   //
   delay(3000);
   lcd.LCD_clear(); // blanks the display
   //zero[0] = 0;
}
/************************************************************/
void loop() {
  readJoystick(); // check to see if joystick has been operated
  drawDisplay();  // draw the display
  clockTiming();  // update clock
  displayHours(); // update the hours
  displayMinutes(); //update the minutes
  displaySeconds(); // update the seconds
  // end main loop
}

/**************************************************************/
// Display Routines
/**************************************************************/
void drawDisplay(){
  // draws display
 lcd.LCD_write_string(3, 0, "+Nokia Clock+", MENU_NORMAL);
  if (runFlag){
    lcd.LCD_write_string(0, 20, "RUN", MENU_NORMAL); 
    //pointer = 0;
  }
  else lcd.LCD_write_string(0, 20, "run", MENU_NORMAL);
  //
  if(setClkFlag && hrSet){
    lcd.LCD_write_string(40, 20, "HOUR", MENU_HIGHLIGHT);
  }
  else lcd.LCD_write_string(40, 20, "hour", MENU_NORMAL);
  //
  if (setClkFlag && hrSet == false){ 
    lcd.LCD_write_string(65, 20, "MIN", MENU_HIGHLIGHT); 
  }
  else lcd.LCD_write_string(65, 20, "min", MENU_NORMAL); 
  //
  if(runFlag) lcd.LCD_write_string(20, 20, "set", MENU_NORMAL);
  else lcd.LCD_write_string(20, 20, "SET", MENU_NORMAL);  
}
/**************************************************************/
void displayHours(){
  // this displays the hours in 24 hour mode
  itoa(hrs,temp,10);
  if(setClkFlag && selected && hrSet){
    secs = 0; // reset seconds
    if (flip){
      if (hrs<10){
        lcd.LCD_write_string(3, 18, "0", MENU_HIGHLIGHT);
        lcd.LCD_write_string(9, 18, temp, MENU_HIGHLIGHT);
      }
      else lcd.LCD_write_string(3, 18, temp, MENU_HIGHLIGHT);
    }
    else if (hrs<10){
        lcd.LCD_write_string(3, 18, "0", MENU_NORMAL);
        lcd.LCD_write_string(9, 18, temp, MENU_NORMAL);
    } 
    else lcd.LCD_write_string(3, 18, temp, MENU_NORMAL); 
  }
  //
  else{
    lcd.LCD_write_string(3, 18, temp, MENU_NORMAL);
    if (hrs<10){ // this will overwrite the last line if hrs <10
      lcd.LCD_write_string(3, 18,"0", MENU_NORMAL);
      lcd.LCD_write_string(9, 18,temp, MENU_NORMAL);
    }
  }
  // add a colon
  //
  lcd.LCD_write_string(15, 18, ":", MENU_NORMAL);
}
/**************************************************************/
void displayMinutes(){
  // updates the mutes display
  // set minutes
  itoa(mins,temp,10);
  if(setClkFlag && selected && hrSet == false){
    secs = 0; // reset seconds
    if (flip){
      if (mins<10){
        lcd.LCD_write_string(21, 18, "0", MENU_HIGHLIGHT);
        lcd.LCD_write_string(27, 18, temp, MENU_HIGHLIGHT);
      }
      else lcd.LCD_write_string(21, 18, temp, MENU_HIGHLIGHT);
    }
    else if (mins<10){
        lcd.LCD_write_string(21, 18, "0", MENU_NORMAL);
        lcd.LCD_write_string(27, 18, temp, MENU_NORMAL);
    } 
    else lcd.LCD_write_string(21, 18, temp, MENU_NORMAL); 
  }
  else{
    lcd.LCD_write_string(21, 18, temp, MENU_NORMAL);
    if (mins<10){ // this will overwrite the last line if hrs <10
      lcd.LCD_write_string(21, 18,"0", MENU_NORMAL);
      lcd.LCD_write_string(27, 18,temp, MENU_NORMAL);
    }
  }
  //
  // add a colon
  lcd.LCD_write_string(33, 18, ":", MENU_NORMAL);  
}
/**************************************************************/
void displaySeconds(){
  // updates the seconds display
  itoa(secs,temp,10);
  if (secs<10){
   lcd.LCD_write_string(39, 18,"0", MENU_NORMAL);
   lcd.LCD_write_string(45, 18,temp, MENU_NORMAL);
  }
  else lcd.LCD_write_string(39, 18,temp, MENU_NORMAL);  
}
/**************************************************************/
// Joystick Routines
/**************************************************************/
void readJoystick(){
  lcd.LCD_write_string(60, 10, "     ", MENU_NORMAL); // delete last joystick value
  switchVoltage = analogRead(0); // read the voltage
  if(switchVoltage >800) debounce = false; // rest debounce if joystick is at rest
  itoa(switchVoltage,joyTemp,10); // convert the voltage ready to display
  lcd.LCD_write_string(60, 10, joyTemp, MENU_NORMAL); // display voltage
  /*------------------------------------------------------*/
  // see where the joystick is pointing
  /*------------------------------------------------------*/
  if (switchVoltage == 0){ // joystick LEFT
    // use to change mode options, moving left
    selected = false; // keep clock running until joystick pressed
    if(debounce == false){ // ignore the joystick until it is released
      pointer = pointer - 1; // calculate next option
      if (pointer <1) pointer = 3;
      switch (pointer){
        case (1):
          runFlag = true; setClkFlag = false; 
        break;

        case(2):
          runFlag = false; setClkFlag = true; hrSet = true;
        break;

        case(3):
          runFlag = false; setClkFlag = true; hrSet = false;
        break;
      }
       debounce = true; // wait for joystick to be released
    }
  }
  /*------------------------------------------------------*/  
  else if (switchVoltage > 130 && switchVoltage < 180){ // joystick PRESSED
    selected = true; 
    delay(switchDelay);
  }
  /*------------------------------------------------------*/
  else if (switchVoltage > 180 && switchVoltage < 400) // joystick DOWN
  {
    if(setClkFlag && selected && hrSet){ // decrease value of hours
      hrs = hrs - 1;
      if(hrs < 0) hrs = 23;
    }
    if(setClkFlag && selected && hrSet == false){ // decrease value of minutes
      mins = mins - 1;
      if(mins < 0) mins = 59;
    }
    delay(500); // delay between changing values if joystick pressed continuously
  }
  /*------------------------------------------------------*/
  else if (switchVoltage > 400 && switchVoltage < 600) // joystick RIGHT
  {
    selected = false; // need to wait for joystick to be pressed
    if(debounce == false){ // only read joystick once until it is released
      pointer = pointer +1; // update more option
      if (pointer == 4) pointer = 1;
      switch (pointer){
        case (1):
          runFlag = true; setClkFlag = false;
        break;

        case(2):
          runFlag = false; setClkFlag = true; hrSet = true;
        break;

        case(3):
          runFlag = false; setClkFlag = true; hrSet = false;
        break;
      }
       debounce = true; // wait for joystick to be released
    }   
    delay(switchDelay);
  }
  /*------------------------------------------------------*/ 
  else if (switchVoltage > 600 && switchVoltage < 800) // joystick UP
  {
    if(setClkFlag && selected && hrSet){ // increment hours
      hrs = hrs + 1;
      if(hrs > 23) hrs = 0;
    }
    if(setClkFlag && selected && hrSet == false){ // increment minutes
      mins = mins + 1;
      if(mins > 59) mins = 0;
    }    
    delay(500); // delay used if joystick continuously pressed
  }
  /*------------------------------------------------------*/ 
  else if (switchVoltage > 800){  // joystick MIDDLE (at rest)          
    delay(switchDelay);
  }
} 
/**************************************************************/
// Clock Routines
/**************************************************************/
void clockTiming(){
  // this is the main timing for the clock routine
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if(setClkFlag && selected) flip = !flip;
    else{
      flip = false;
      secs = secs + 1;
      if(secs >59){ 
        secs = 0;
        mins = mins +1;
      }
      if(mins > 59){
          hrs = hrs+1;
          mins = 0;
      }
      if(hrs >23){
        hrs = 0;
      }
    }
  }
}
/**************************************************************/
