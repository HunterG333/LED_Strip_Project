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

//strip appearence properties
uint8_t brightness = 5;

//i think rgb is a keyword so "RGB" was not working properly
struct RGBs{
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
  while(!Serial) {}
}

void loop() {

  readSerial();

  FastLED.show();
}


void readSerial(){
  if(Serial.available() > 0){

    //read and print out the command
    String command = Serial.readString();

    Serial.print(command);


    //if(values[0] == "WRGB"){
      //remoteChangeColorRGB(values[1], values[2], values[3]);
    //}

  }
}



/*
Functions for use from server
*/

//not implemented -- currently set to green/blue
void remoteChangeColorRGB(int red, int green, int blue){
  fill_solid(leds, NUM_LEDS, CRGB(red, green, blue));
}