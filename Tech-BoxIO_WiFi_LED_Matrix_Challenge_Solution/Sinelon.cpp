//Start by including the required header for this file
#include "src/Effect.h"

//Create the Sinelon object
class Sinelon : public Effect {

//Create the private variables for this class
private:
  int pos = 0;
  int FRAMES_PER_SECOND = 60;

public:
  //Create the Sinelon constructor with the correct variables
  Sinelon(CRGB *leds, int width, int height) : Effect(leds, width, height) {}

  //Create the 'start' public function
  void start(int gHue) {
    //A colored dot sweeping back and forth, with fading trails
    fadeToBlackBy( leds, width * height, 20);   //Fade the LEDs slowly over time
    pos = beatsin16(13,0,width * height);
    leds[pos] += CHSV( gHue, 255, 192);
    
    //Send the 'leds' array out to the actual LED matrix
    FastLED.show();  
    //Insert a delay to keep the framerate modest
    FastLED.delay(1000/FRAMES_PER_SECOND); 
  }
};
