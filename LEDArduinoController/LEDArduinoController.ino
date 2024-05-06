/*
NeonSignProject.ino
Custom NEON Sign program with server control
Version: 0.0
Author: Hunter Greer
*/

#include <FastLED.h>
#include <string.h>


#define NUM_LEDS 300
#define LED_PIN 13

CRGB leds[NUM_LEDS];

//command attributes
const byte numChars = 32;
char receivedChars[numChars];  // an array to store the received data

boolean newData = false;

//strip appearence properties
uint8_t brightness = 5;

//i think rgb is a keyword so "RGB" was not working properly
struct RGBs {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

RGBs myRGB;

void setup() {
  Serial.begin(1200);

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);

  //initialize brightness
  FastLED.setBrightness(brightness);

  //loop until serial connection is ready
  while (!Serial) {}
}

void loop() {

  readSerial();

  FastLED.show();
}

/*
* Supported Commands for Serial Input
* "RGB,R,G,B" for setting the strip to a whole color where R, G, and B are the number values that correspond to red, green, and blue color values respectively that range from 0-255
* "BR,V" for changing the brightness of the strip to the specified V value
*/
void readSerial() {

  while (Serial.available() > 0) {
    int commandArrLength = 4;
    String commandArr[commandArrLength];

    //read command
    String command = Serial.readString();
    const int cmdLength = command.length();

    //convert command to char array
    char* char_array = new char[cmdLength];
    strcpy(char_array, command.c_str());

    //split command into parts seperated by ","
    char* p;
    p = strtok(char_array, ",");

    //loop through and add the split elements to commandArr
    int i = 0;
    while (p != NULL) {

      commandArr[i] = p;
      p = strtok(NULL, ",");
      i++;
    }

    //loop through and print out commandArr to make sure it has the elements we need
    /*for(int i = 0; i < commandArrLength; i++){
      Serial.println(commandArr[i]);
    }
    */
    Serial.print(commandArr[0]);
    if (commandArr[0].equals("RGB")) {
      int val1 = commandArr[1].toInt();
      int val2 = commandArr[2].toInt();
      int val3 = commandArr[3].toInt();
      remoteChangeColorRGB(val1, val2, val3);
    } else if (commandArr[0].equals("BR")) {
      int val1 = commandArr[1].toInt();
      changeBrightness(val1);
    }



    delete[] char_array;
  }
}



/*
LED Functions
*/

//change the whole strip to the specified red green and blue values
void remoteChangeColorRGB(int red, int green, int blue) {
  fill_solid(leds, NUM_LEDS, CRGB(red, green, blue));
}

void changeBrightness(int brightness) {
  FastLED.setBrightness(brightness);
}

//displays a binary error code 0 = blue, 1 = red onnto the led strip. The binary string length must not be higher than the number of LEDs
void displayError(int errorNumber) {

  //base case
  if (errorNumber == 0) {
    leds[0] = CRGB::Blue;
    return;
  }

  Serial.print(errorNumber);

  // array to store binary number
  int binaryNum[32];

  // counter for binary array
  int i = 0;
  while (errorNumber > 0) {

    // storing remainder in binary array
    binaryNum[i] = errorNumber % 2;
    errorNumber = errorNumber / 2;
    i++;
  }

  // printing binary array in reverse order
  for (int j = i - 1; j >= 0; j--)
    if (binaryNum[j] == 0) {
      leds[j] = CRGB::Blue;
    } else {
      leds[j] = CRGB::Red;
    }
}
