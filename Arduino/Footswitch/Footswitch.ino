/*
 Keyboard Button test
  For the Arduino Leonardo and Micro.
  Sends a text string when a button is pressed.
 
 The circuit:
 * 10-kiloOhm resistor attached from digital pins to ground
 
 6-way selector - use series of resistors to give different values to analog pin.
 A - Playback functions:    Play/Pause/Stop, ff, rw, Beginning, End
 B - Audio Functions:       Vol+, Vol-,
 C -
 D - Recording Functions:   Arm record,
 E - Interface functions:   Open Software, change presets etc
 F - PC Functions:          Lock,Log in, Log out, Open Recording Software, Shutdown
 
Upon button press being detected the code should run the void based on the current selector switch setting.
 
 */
 
//Create Array to store void names for each selector/button combo
String ButtonMap[6][6]={
{"1", "PlayPause", "FF", "RW", "VolUp", "VolDwn"},
{"2", "NA", "NA", "NA", "NA", "NA"},
{"3", "NA", "NA", "NA", "NA", "NA"},
{"4", "NA", "NA", "NA", "NA", "NA"},
{"5", "NA", "NA", "NA", "NA", "NA"},
{"6", "LockPC", "NA", "NA", "NA", "NA"}};
 
//const int ButtonCount = 5;
int ButtonPins[]={4, 5, 6, 7, 8};
const int ButtonCount = sizeof(ButtonPins)/sizeof(int);
int SelectorPin = A3;
 
int PreviousState[ButtonCount];
 
//define variables to hold Selecotr switch position, button pressed and the associated function to Run
int SelectorPos;
int CurrentPos;
int ButtonPressed;
String ToRun;
 
void setup() {
  // open the serial port and keyboard control:
  Serial.begin(9600);
  Keyboard.begin();
 
//Step through ButtonPin Array, assign each as INPUT with a PreviousState of LOW
for (int ThisPin = 0; ThisPin < ButtonCount; ThisPin++)
      {pinMode(ButtonPins[ThisPin], INPUT);
      PreviousState[ThisPin] = LOW;}
 
CurrentPos=0;
 
MenuFlash();
delay(3000);
//SelectorScan();
     
}
 
void loop() {
  SelectorScan();
  ButtonScan();
  //delay(10);
}
 
void SelectorScan()
{/* 5v to GND with a 5 resistor ladder in between (2k resistor?). Each pin of selecotr connected at
points (1 before first resistor then every intersection between each resistor.
Use analog pin read to get value, and map to 1-6; SelectorPos will equal this output.*/
 
int Reading = analogRead(SelectorPin);
 
 
if (Reading >= 301 && Reading <= 450)
  {SelectorPos = 6;}
else if (Reading >= 0 && Reading <= 100)
  {SelectorPos = 5;}
else if (Reading >= 101 && Reading <= 100)
  {SelectorPos = 4;}
else if (Reading >= 900 && Reading <= 1050)
  {SelectorPos = 3;}  
else if (Reading >= 601 && Reading <= 750)
  {SelectorPos = 2;}
else if (Reading >= 451 && Reading <= 600)
  {SelectorPos = 1;}
 
 
  //Check for change - if so refresh the screen menu
  if (SelectorPos != CurrentPos)
  {MenuFlash();}
 
  //Then set current value
  CurrentPos = SelectorPos;
}
 
void ButtonScan(){
/*Use a For Loop to go through the button pin array checking all states - any changes then check SelectSwitch() and somehow
fire the void associated in the ButtonMap array.
In theory the array should be accessed such as ButtonMap[ButtonPressed, 1-5]][SelectorSwitchReturn, 1-6]  */
int State[ButtonCount];
 
for (int ThisPin = 0; ThisPin < ButtonCount; ThisPin++) {
  //Stick in the loop to read and compare each pin. Then fire a void to calculate and fire the correct void, maybe?
  State[ThisPin] = digitalRead(ButtonPins[ThisPin]);
  if ((State[ThisPin] != PreviousState[ThisPin]) && (State[ThisPin] == HIGH))
    { ButtonPressed = ThisPin +1;
      ToRun = (ButtonMap[SelectorPos-1][ButtonPressed]);
      ProcessPress();
      PreviousState[ThisPin]=State[ThisPin];
      }
  }
}
void ProcessPress()
{ /*Takes the values from ReadSelector void (SelectorPos, -1) and ReadButtons (ButtonPressed) and finds what void should be triggered from the array
EG */
 
if (ToRun == "PlayPause")
//{Keyboard.println("Play/Pause keypress triggered.");}
{Serial.println(analogRead(SelectorPin));
Serial.println("Play/Pause keypress triggered.");}
 
else if (ToRun == "FF")
//{Keyboard.println("Fast forward keypress triggered.");}
{Serial.println(analogRead(SelectorPin));
Serial.println("FF keypress triggered.");}
 
else if (ToRun =="RW")
//{Keyboard.println("Rewind keypress triggered.");}
{Serial.println(analogRead(SelectorPin));
Serial.println("RW keypress triggered.");}
 
else if (ToRun == "VolUp")
{Serial.println(analogRead(SelectorPin));
Serial.println("Volume Up triggered.");}
 
else if (ToRun == "VolDwn")
{Serial.println(analogRead(SelectorPin));
Serial.println("Volume down triggered.");}
 
else if (ToRun == "LockPC")
    {Serial.println("Logging out...");
    delay(2000);  
 
    Keyboard.press(131);
    Keyboard.print("l");
    Keyboard.releaseAll();}
else if (ToRun == "NA")
  {Serial.println("NA - No function assigned");}
 
else
  {Serial.println(ToRun && "- no such function defined");}
 
}
 
void MenuFlash()
  {Serial.println("Screen update to follow...");
 
}
