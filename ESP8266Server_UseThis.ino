#include <ESP8266WiFi.h>
//#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Adafruit_NeoPixel.h>
//Wifi Stuff
const char* ssid = "YourWifiSSID";
const char* password = "YourWifiPassword";
//web server
ESP8266WebServer server(80);

const int iPin = 2; // ESP8266 Pin to NeoPixel

// version with Adafruit multi-tasking design
//https://learn.adafruit.com/multi-tasking-the-arduino-part-3?view=all#put-it-all-together-dot-dot-dot


/// Neopixel classes
// Pattern types supported:
enum  pattern { NONE, RAINBOW_CYCLE, THEATER_CHASE, COLOR_WIPE, SCANNER, FADE };
// Patern directions supported:
enum  direction { FORWARD, REVERSE };

// NeoPattern Class - derived from the Adafruit_NeoPixel class
class NeoPatterns : public Adafruit_NeoPixel
{
  public:

    // Member Variables:
    pattern  ActivePattern;  // which pattern is running
    direction Direction;     // direction to run the pattern

    unsigned long Interval;   // milliseconds between updates
    unsigned long lastUpdate; // last update of position

    uint32_t Color1, Color2;  // What colors are in use
    uint16_t TotalSteps;  // total number of steps in the pattern
    uint16_t Index;  // current step within the pattern

    void (*OnComplete)();  // Callback on completion of pattern

    // Constructor - calls base-class constructor to initialize strip
    NeoPatterns(uint16_t pixels, uint8_t pin, uint8_t type, void (*callback)())
      : Adafruit_NeoPixel(pixels, pin, type)
    {
      OnComplete = callback;
    }

    // Update the pattern
    void Update()
    {
      if ((millis() - lastUpdate) > Interval) // time to update
      {
        lastUpdate = millis();
        switch (ActivePattern)
        {
          case RAINBOW_CYCLE:
            RainbowCycleUpdate();
            break;
          case THEATER_CHASE:
            TheaterChaseUpdate();
            break;
          case COLOR_WIPE:
            ColorWipeUpdate();
            break;
          case SCANNER:
            ScannerUpdate();
            break;
          case FADE:
            FadeUpdate();
            break;
          default:
            break;
        }
      }
    }

    // Increment the Index and reset at the end
    void Increment()
    {
      if (Direction == FORWARD)
      {
        Index++;
        if (Index >= TotalSteps)
        {
          Index = 0;
          if (OnComplete != NULL)
          {
            OnComplete(); // call the comlpetion callback
          }
        }
      }
      else // Direction == REVERSE
      {
        --Index;
        if (Index <= 0)
        {
          Index = TotalSteps - 1;
          if (OnComplete != NULL)
          {
            OnComplete(); // call the comlpetion callback
          }
        }
      }
    }

    // Reverse pattern direction
    void Reverse()
    {
      if (Direction == FORWARD)
      {
        Direction = REVERSE;
        Index = TotalSteps - 1;
      }
      else
      {
        Direction = FORWARD;
        Index = 0;
      }
    }

    // Initialize for a RainbowCycle
    void RainbowCycle(uint8_t interval, direction dir = FORWARD)
    {
      ActivePattern = RAINBOW_CYCLE;
      Interval = interval;
      TotalSteps = 255;
      Index = 0;
      Direction = dir;
    }

    // Update the Rainbow Cycle Pattern
    void RainbowCycleUpdate()
    {
      for (int i = 0; i < numPixels(); i++)
      {
        setPixelColor(i, Wheel(((i * 256 / numPixels()) + Index) & 255));
      }
      show();
      Increment();
    }

    // Initialize for a Theater Chase
    void TheaterChase(uint32_t color1, uint32_t color2, uint8_t interval, direction dir = FORWARD)
    {
      ActivePattern = THEATER_CHASE;
      Interval = interval;
      TotalSteps = numPixels();
      Color1 = color1;
      Color2 = color2;
      Index = 0;
      Direction = dir;
    }

    // Update the Theater Chase Pattern
    void TheaterChaseUpdate()
    {
      for (int i = 0; i < numPixels(); i++)
      {
        if ((i + Index) % 3 == 0)
        {
          setPixelColor(i, Color1);
        }
        else
        {
          setPixelColor(i, Color2);
        }
      }
      show();
      Increment();
    }

    // Initialize for a ColorWipe
    void ColorWipe(uint32_t color, uint8_t interval, direction dir = FORWARD)
    {
      ActivePattern = COLOR_WIPE;
      Interval = interval;
      TotalSteps = numPixels();
      Color1 = color;
      Index = 0;
      Direction = dir;
    }

    // Update the Color Wipe Pattern
    void ColorWipeUpdate()
    {
      setPixelColor(Index, Color1);
      show();
      Increment();
    }

    // Initialize for a SCANNNER
    void Scanner(uint32_t color1, uint8_t interval)
    {
      ActivePattern = SCANNER;
      Interval = interval;
      TotalSteps = (numPixels() - 1) * 2;
      Color1 = color1;
      Index = 0;
    }

    // Update the Scanner Pattern
    void ScannerUpdate()
    {
      for (int i = 0; i < numPixels(); i++)
      {
        if (i == Index)  // Scan Pixel to the right
        {
          setPixelColor(i, Color1);
        }
        else if (i == TotalSteps - Index) // Scan Pixel to the left
        {
          setPixelColor(i, Color1);
        }
        else // Fading tail
        {
          setPixelColor(i, DimColor(getPixelColor(i)));
        }
      }
      show();
      Increment();
    }

    // Initialize for a Fade
    void Fade(uint32_t color1, uint32_t color2, uint16_t steps, uint8_t interval, direction dir = FORWARD)
    {
      ActivePattern = FADE;
      Interval = interval;
      TotalSteps = steps;
      Color1 = color1;
      Color2 = color2;
      Index = 0;
      Direction = dir;
    }

    // Update the Fade Pattern
    void FadeUpdate()
    {
      // Calculate linear interpolation between Color1 and Color2
      // Optimise order of operations to minimize truncation error
      uint8_t red = ((Red(Color1) * (TotalSteps - Index)) + (Red(Color2) * Index)) / TotalSteps;
      uint8_t green = ((Green(Color1) * (TotalSteps - Index)) + (Green(Color2) * Index)) / TotalSteps;
      uint8_t blue = ((Blue(Color1) * (TotalSteps - Index)) + (Blue(Color2) * Index)) / TotalSteps;

      ColorSet(Color(red, green, blue));
      show();
      Increment();
    }

    // Calculate 50% dimmed version of a color (used by ScannerUpdate)
    uint32_t DimColor(uint32_t color)
    {
      // Shift R, G and B components one bit to the right
      uint32_t dimColor = Color(Red(color) >> 1, Green(color) >> 1, Blue(color) >> 1);
      return dimColor;
    }

    // Set all pixels to a color (synchronously)
    void ColorSet(uint32_t color)
    {
      for (int i = 0; i < numPixels(); i++)
      {
        setPixelColor(i, color);
      }
      show();
    }

    // Returns the Red component of a 32-bit color
    uint8_t Red(uint32_t color)
    {
      return (color >> 16) & 0xFF;
    }

    // Returns the Green component of a 32-bit color
    uint8_t Green(uint32_t color)
    {
      return (color >> 8) & 0xFF;
    }

    // Returns the Blue component of a 32-bit color
    uint8_t Blue(uint32_t color)
    {
      return color & 0xFF;
    }

    // Input a value 0 to 255 to get a color value.
    // The colours are a transition r - g - b - back to r.
    uint32_t Wheel(byte WheelPos)
    {
      WheelPos = 255 - WheelPos;
      if (WheelPos < 85)
      {
        return Color(255 - WheelPos * 3, 0, WheelPos * 3);
      }
      else if (WheelPos < 170)
      {
        WheelPos -= 85;
        return Color(0, WheelPos * 3, 255 - WheelPos * 3);
      }
      else
      {
        WheelPos -= 170;
        return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
      }
    }
};


/****************************************************************
 *
 *
 *
 ****************************************************************/


// Define a NeoPattern for the ring
//  as well as some completion routines
NeoPatterns Ring1(16, iPin, NEO_GRB + NEO_KHZ800, &Ring1Complete);

// Ring1 Completion Callback
void Ring1Complete() {
  Ring1.Reverse();
}
/****************************************************************/

void setup(void)
{
  Serial.begin(115200);

  // Initialize
  Ring1.begin();
  Ring1.TheaterChase(Ring1.Color(255, 255, 0), Ring1.Color(0, 0, 50), 100);
  Ring1.Update();


  // Connect to WiFi network
  Serial.print("Connecting to: ");
  Serial.println(ssid);
  //    colorWipe(strip.Color(255, 0, 0), 50); // Red

  WiFi.begin(ssid, password);


  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //    colorWipe(strip.Color(0, 0, 255), 50); // Blue

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //Ring1.RainbowCycle(16);

  server.on("/", handle_root);

  server.on("/set", handleRGB);

  server.on("/R", []() {
    server.send(200, "text/plain", "Red");
    Serial.println("Red");
    Ring1.ColorWipe(Ring1.Color(255, 0, 0), 50); // Red
  });

  server.on("/G", []() {
    server.send(200, "text/plain", "Green");
    Serial.println("Green");

    Ring1.ColorWipe(Ring1.Color(0, 255, 0), 50); // Green
  });


  server.on("/B", []() {
    server.send(200, "text/plain", "Blue");
    Serial.println("Blue");

    Ring1.ColorWipe(Ring1.Color(0, 0, 255), 50); // Blue
  });


  server.on("/W", []() {
    server.send(200, "text/plain",  "White Scanner");
    Serial.println("White Scanner");

    Ring1.Scanner(Ring1.Color(255, 255, 255), 30); // White
  });


  server.on("/X", []() {
    server.send(200, "text/plain",  "Rainbow");

    Serial.println("Rainbow");
    Ring1.RainbowCycle(20);
  });

  server.on("/Y", []() {
    server.send(200, "text/plain",  "Rainbow Cycle");
    Serial.println("Rainbow Cycle");
    Ring1.RainbowCycle(10);
  });

  server.on("/Z", []() {
    server.send(200, "text/plain",  "Chaser Rainbow");
    Serial.println("Chaser Rainbow");
    Ring1.RainbowCycle(50);
  });

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
    Serial.println("Request Handled - Inline");
  });

  server.on("/config", []() {
    server.send(200, "text/plain", "/R=Red, /G=Green, /B=Blue, /W=White Scanner, /X=Rainbow, /Y= Rainbow Cycle, /Z=Chaser Rainbow, /set=RGB");
    Serial.println("Config Infomation here");
  });
  server.on("/?", []() {
    server.send(200, "text/plain", "/R=Red, /G=Green, /B=Blue, /W=White Scanner, /X=Rainbow, /Y= Rainbow Cycle, /Z=Chaser Rainbow, /set=RGB");
    Serial.println("Config Infomation here");
  });



  server.begin();

}

void loop(void)
{
  Ring1.Update();
  server.handleClient();
}


void handle_root() {

  server.send(200, "text/plain",  "White Chaser");
  Serial.println("White Chaser");

  Ring1.TheaterChase(Ring1.Color(127, 127, 127), 0, 50); // White

}

void handleRGB() {
  long  LEDvalueR = 255;
  long  LEDvalueG = 255;
  long  LEDvalueB = 255;

  if (!server.hasArg("R")) return returnFail("BAD ARGS");
  LEDvalueR = (server.arg("R").toInt());
  if (!server.hasArg("G")) return returnFail("BAD ARGS");
  LEDvalueG = (server.arg("G").toInt());
  if (!server.hasArg("B")) return returnFail("BAD ARGS");
  LEDvalueB = server.arg("B").toInt();

  server.send(200, "text/plain",  "RGB Color");
  Ring1.ColorWipe(Ring1.Color(LEDvalueR, LEDvalueG, LEDvalueB), 30); // RGB

  Serial.println("RGB Color");

}

void returnFail(String msg)
{
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(500, "text/plain", msg + "\r\n");
  Serial.println("RGB Color - Bad Args");
}


