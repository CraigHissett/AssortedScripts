/* FILE:    ARD_RAMPS_LCD_Panel_Test.pde
   DATE:    09/01/14
   VERSION: 0.1

This sketch will test all of the components of the RAMPS compatible LCD panel.
It assumes that the panel will be connected to an Arduino Mega and RAMPS shield.
However this sketch is useful if you are not intending to use it with a 3D printer
as an example of how to access the various components.

To run this test you will need to insert and SD card with a text file named 
'test.txt'. The test will display on the LCD the first 20 characters contained
within this file. You will also be prompted, via the LCD screen, to adjust the 
controls of the panel.

The sketch will perform the following tests:

Buzzer test: The buzzer will beep twice
SD Card test: Will attempt to read the contents of the test.txt file on the SD card
Dial button check: Will prompt you to press the panels dial
Reset button test: Will prompt you to press the button labelled reset on the panel
Dial CW rotate test: Will prompt you to turn the dial clockwise until count = 10
Dial CCW rotate test: Will prompt you to turn the dial counter-clockwise until count = 0

There will be a 4 second delay between each test.


You may copy, alter and reuse this code in any way you like but please leave 
reference to hobbycomponents.com in your comments if you redistribute this code. 
This software may not be used by other sellers.

THIS SOFTWARE IS PROVIDED "AS IS". HOBBY COMPONENTS LTD MAKES NO WARRANTIES, WHETHER 
EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF 
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, ACCURACY OR LACK OF NEGLIGENCE.
HOBBY COMPONENTS LTD SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR ANY DAMAGES, 
INCLUDING, BUT NOT LIMITED TO, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY 
REASON WHATSOEVER.
*/

/* Include the LCD Library */
#include <LiquidCrystal.h>

/* Include the SD card library */
#include <SD.h>

/* DIO pin used to control the SD card readers CS pin */
#define SD_CARD_CD_DIO 53 

/* Rotary encoder (dial) pins */
#define ROT_EN_A 31
#define ROT_EN_B 33

/* Rotary encoder button pin */
#define BUTTON_DIO 35
#define BUZZER_DIO 37

/* Reset button pin */
#define RESET_DIO 41

/* LCD display pins */
#define LCD_RS 16
#define LCD_EN 17
#define LCD_D4 23
#define LCD_D5 25
#define LCD_D6 27
#define LCD_D7 29


/* Create an instance of the LCD library. */
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

/* Create an instance of the SD card library */
File SDFileData;


void setup() 
{
  /* Initialise the LCD */
  lcd.begin(20, 4);
  
  /* Configure the pins used to read the status of the panel dial 
     and buttons */
  pinMode(BUZZER_DIO, OUTPUT);    
  pinMode(BUTTON_DIO, INPUT);   
  digitalWrite(BUTTON_DIO, HIGH); 
  pinMode(RESET_DIO, INPUT);   
  digitalWrite(RESET_DIO, HIGH); 
  pinMode(ROT_EN_A,INPUT);
  pinMode(ROT_EN_B,INPUT);
  digitalWrite(ROT_EN_A, HIGH);
  digitalWrite(ROT_EN_B, HIGH);
}


/* Main program loop */
void loop() 
{
  /* Output the test message to the LCD */
  lcd.setCursor(2,0); 
  lcd.print("HOBBY COMPONENTS");
  lcd.setCursor(2,1); 
  lcd.print("RAMPS LCD PANEL");
  lcd.setCursor(4,2); 
  lcd.print("HC3DPR0005");
  lcd.setCursor(8,3); 
  lcd.print("TEST");
  
  /* Wait 4 seconds */
  delay(4000);
 
  /* Buzzer test */
  BuzzerCheck();
  
  /* SD card read test */
  SDCardCheck();
  delay(4000);
  
  /* Control check */
  ControlCheck();
  delay(4000);
  
  /* All done! */
  lcd.clear();
  lcd.setCursor(3,1); 
  lcd.print("TEST COMPLETE");
  
  while(1);
}

/* Test the buzzer by beeping twice */
void BuzzerCheck(void)
{
  lcd.clear();
  lcd.setCursor(4,1); 
  lcd.print("BUZZER CHECK");
  
  digitalWrite(BUZZER_DIO, HIGH);
  delay(300);
  digitalWrite(BUZZER_DIO, LOW);
  delay(300); 
  digitalWrite(BUZZER_DIO, HIGH);
  delay(300);
  digitalWrite(BUZZER_DIO, LOW); 
}

/* Test the SD card by reading the contents of a file named test.txt */
void SDCardCheck(void)
{
  byte index;
  char SDCardText[]= "                    ";
  
  /* Display new test */
  lcd.clear();
  lcd.setCursor(3,0); 
  lcd.print("SD CARD CHECK");
  lcd.setCursor(0,1); 
  lcd.print("Initiliasing.."); 
  
  /* Initialise the SD card */
  if (!SD.begin(SD_CARD_CD_DIO)) 
  {
    /* If there was an error output this to the LCD */
    lcd.setCursor(14,1); 
    lcd.print("ERROR!"); 
    lcd.setCursor(0,3); 
    lcd.print("SD Card not detected");
  }else
  {
    lcd.setCursor(14,1); 
    lcd.print("OK! ");
  }
  
 
   /* Check if the text file exists */
   if(SD.exists("test.txt"))
   {
     lcd.setCursor(0,2); 
     lcd.print("test.txt contents:");

     /* The file exists so open it */
     SDFileData = SD.open("test.txt");
     
     /* Sequentially read the data from the file and store the first 20 characters */
     for (index = 0; index < 20; index++)
     {
       if (SDFileData.available())
         SDCardText[index] = SDFileData.read();
     }
     
     /* Display the contents on the LCD */
     lcd.setCursor(0,3); 
     lcd.print(SDCardText);
  
   }else
   {
     lcd.setCursor(0,2); 
     lcd.print("test.txt missing");   
   }
   
   /* Close the file */
   SDFileData.close();  
}

/* Check the dial and push buttons */
void ControlCheck(void)
{
  byte DialPos = 0;
  byte Last_DialPos = 0;
  byte DialCount = 0;
  
  /* Display new test */
  lcd.clear();
  lcd.setCursor(3,0); 
  lcd.print("CONTROL CHECK");
 
  /* Prompt the user to press the dial */
  lcd.setCursor(0,2); 
  lcd.print("Press dial:");
  
  /* Wait for the dial to be pressed */
  while(digitalRead(BUTTON_DIO));
  
  lcd.setCursor(12,2); 
  lcd.print("PASS"); 
  
  delay(4000);

  /* Prompt the user to press the button marked reset */  
  lcd.setCursor(0,2); 
  lcd.print("Press reset btn:    ");
  
  /* Wait for button to be pressed */
  while(digitalRead(RESET_DIO));
  
  lcd.setCursor(16,2); 
  lcd.print("PASS"); 
  
  delay(4000);
 
  /* Prompt the user to turn the dial clockwise by 10 positions */ 
  lcd.setCursor(0,2); 
  lcd.print("Rotate Dial CW:     ");

  /* Run this loop until the dial is turned 10 positions */
  while(DialCount <= 9)
  {
    /* Read the status of the dial */
    DialPos = (digitalRead(ROT_EN_B) << 1) | digitalRead(ROT_EN_A);
  
    /* Is the dial being turned clockwise ? */
    if (DialPos == 3 && Last_DialPos == 1)
    {
      /* If so increase the dial counter and display it */
      DialCount++;
      lcd.setCursor(16,2); 
      lcd.print(DialCount);
    }
  
    /* If the dial counter reaches a value of 10 then the test has passed */
    if (DialCount >= 10)
    {
      lcd.setCursor(16,2); 
      lcd.print("PASS");
    } 
    /* Remember the last position of the dial so we know when it has changed */
    Last_DialPos = DialPos;
  }

  delay(4000);
  DialCount = 10;
  lcd.setCursor(0,2); 
  
  /* Now instruct the user to rotate the dial counter-clockwise */
  lcd.print("Rotate Dial CCW:    ");
  
  /* Run this loop until the dial is turned 10 positions */
  while(DialCount)
  {
    /* Read the status of the dial */
    DialPos = (digitalRead(ROT_EN_B) << 1) | digitalRead(ROT_EN_A);
  
    /* Is the dial being turned counter-clockwise ? */
    if (DialPos == 3 && Last_DialPos == 2)
    {
      /* If so decrease the dial counter and display it */
      DialCount--;
      lcd.setCursor(17,2); 
      lcd.print(DialCount);
    }
  
    /* If the dial counter reaches a value of 0 then the test has passed */
    if (DialCount == 0)
    {
      lcd.setCursor(16,2); 
      lcd.print("PASS");
    } 
  
    /* Remember the last position of the dial so we know when it has changed */
    Last_DialPos = DialPos;
  }
}
