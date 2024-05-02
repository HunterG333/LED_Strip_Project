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

#define TRACK_WORD_BEGINNING 0
#define TRACK_WORD_END 15
#define HOUSE_BEGINNING 16
#define HOUSE_END 18
#define RUNNING_TRACK_BEGINNING 19
#define RUNNING_TRACK_END 30

CRGB leds[NUM_LEDS];

//strip appearence properties

uint8_t brightness = 50;

//i think rgb is a keyword so "RGB" was not working properly
struct RGBs{
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

//changed to match rgb
struct HSVs{
  uint8_t hue;
  uint8_t saturation;
  uint8_t value;
};

RGBs myRGB;
HSVs myHSV;

void setup() {
  Serial.begin(1200);
  // put your setup code here, to run once:
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);

  while(!Serial) {}
}

void loop() {
  // put your main code here, to run repeatedly:

  if (Serial.available() > 0) {
    //format: function letter, value, value, value (values optional)
    String message = Serial.readStringUntil('\n');
    

    //uses strtok to split by comma
    char charMessage[message.length() + 1];
    message.toCharArray(charMessage, sizeof(charMessage));

    char* token = strtok(charMessage, ",");

    const int maxTokens = 4; // Maximum number of tokens
    char* tokens[maxTokens]; // Array to store the tokens

    int tokenCount = 0; // Counter for the number of tokens

    while(token != NULL && tokenCount < maxTokens){
      tokens[tokenCount] = token;
      tokenCount++;
      token = strtok(NULL, ",");
    }

    if(strcmp(tokens[0], "br") == 0){

      brightness = atoi(tokens[1]) % 100;

    } else if(strcmp(tokens[0], "rgb") == 0){

      myRGB.red = atoi(tokens[1]) % 256;
      myRGB.green = atoi(tokens[2]) % 256;
      myRGB.blue = atoi(tokens[3]) % 256;

    } else if(strcmp(tokens[0], "hsv") == 0){

      myHSV.hue = atoi(tokens[1]) % 256;
      myHSV.saturation = atoi(tokens[2]) % 256;
      myHSV.value = atoi(tokens[3]) % 256;
      
    } else if(strcmp(tokens[0], "wsrgb") == 0){

      setWholeStripColorRGB();

    } else if(strcmp(tokens[0], "wshsv") == 0){

      setWholeStripColorHSV();

    }

    
    Serial.flush();
    tokenCount = 0;
  }

    

  
  


  EVERY_N_MILLISECONDS(25){
    FastLED.setBrightness(brightness);
  }
  EVERY_N_MILLISECONDS(25){
    organizedColorSequence();
  }
  FastLED.show();
}



/*
Functions for use from server
*/


//not implemented -- currently set to red
void remoteChangeColorHSV(){
  //code to remotely change the HSV struct

  myHSV.hue = 0;
  myHSV.saturation = 255;
  myHSV.value = 255;
}

//not implemented -- currently set to green/blue
void remoteChangeColorRGB(){
  //code to remotely change the RGB struct

  myRGB.red = 0;
  myRGB.green = 200;
  myRGB.blue = 55;
}

/*
Color strip functions
*/


void setWholeStripColorRGB() {
  //code to change the color of the whole strip remotely using rgb

  fill_solid(leds, NUM_LEDS, CRGB(myRGB.red, myRGB.green, myRGB.blue));
}

void setWholeStripColorHSV() {
  //code to change the color of the whole strip remotely using hsv

  fill_solid(leds, NUM_LEDS, CHSV(myHSV.hue, myHSV.saturation, myHSV.value));
}

//Not Implemented
void setTRACKWORDStripColor() {
  //code to change the color of the word "TRACK" on the sign

  for (int i = TRACK_WORD_BEGINNING; i <= TRACK_WORD_END; i++) {
    leds[i] = CRGB(255, 0, 0);  //sets to red
  }
}

//Not Implemented
void setHOUSEStripColor() {
  //code to change the color of the word "HOUSE" on the sign

  for (int i = HOUSE_BEGINNING; i <= HOUSE_END; i++) {
    leds[i] = CRGB(0, 0, 255);  //sets to blue
  }
}

//Not Implemented
void setRUNNINGTRACKColor() {
  //code to change the color of the running track on the sign

  for (int i = RUNNING_TRACK_BEGINNING; i <= RUNNING_TRACK_END; i++) {
    leds[i] = CRGB(0, 255, 0);  //sets to green
  }
}

void randomColorSequence() {
  //code that generates a random color and moves it down the chain

  int hsvValue = random(256);

  for (int i = NUM_LEDS - 1; i > 0; i--) {
    leds[i] = leds[i - 1];
  }
  leds[0] = CHSV(hsvValue, 255, 255);
}

uint8_t currentHue = 0;
void organizedColorSequence() {
  //code that shifts organized hues down the chain

  for (int i = NUM_LEDS - 1; i > 0; i--) {
    leds[i] = leds[i - 1];
  }
  leds[0] = CHSV(currentHue, 255, 255);

  currentHue = (currentHue + 1) % 256;
}

uint8_t currentHue2 = random(0,20);
void semiOrderedColorSequence(){
  //code that shifts semi-organized hues down the chain

  for (int i = NUM_LEDS - 1; i > 0; i--) {
    leds[i] = leds[i - 1];
  }
  leds[0] = CHSV(currentHue, 255, 255);

  currentHue = (currentHue + random(0,20)) % 256;
}
