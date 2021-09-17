//Start by including the required header for this file
#include "Effect.h"

//Define the following variables for use below
#define MAX_TWINKS 25
#define OFFSET 0xB000

//Create a structure 'Twink' and define its type
typedef struct Twink {
    short x;
    short y;
} Twink;

//Create the Twinkle object
class Twinkle : public Effect {

//Create the private variables for this class
private:
  short numTwinks;            
  bool colour;
  bool cycleSaturation;
  int FRAMES_PER_SECOND = 60;
  uint16_t iterations = 0;
  uint16_t frame = 0x0000;
    
public:
  //Create the Twinkle constructor with the appropriate variables
  Twinkle(CRGB *leds, int width, int height, bool colour, bool cycleSaturation) :
    Effect(leds, width, height),
    colour(colour),
    cycleSaturation(cycleSaturation),
    numTwinks(0) {
  }

  //Create the 'start' public function
  void start() {
    if(iterations > 2250) { //If the iterations are over 2250, then reset the frame and iteration to 0
      frame = 0x0000;
      iterations = 0;
    }
    else {  //Otherwise, increment the frame by 0x20 (HEX) / 32 (DEC) and add 1 to the iteration count
      frame += 0x20;
      iterations = iterations + 1;
    }

    //For each LED in the matrix
    for (int i = 0; i < width * height; i++) {
      if (leds[i]) {  //If the LED is illuminated, slowly fade it out over time and decrement the number of twinks
        leds[i].fadeToBlackBy(30);
        if (!leds[i]) {
          numTwinks--;
        }
      } 
      else {    //Otherwise, illuminate a random LED with a random hue and saturation based on the mode. If not colour, then display as White
        if (random(56) == 0) {
          numTwinks++;
          if (colour) {
            if (cycleSaturation) {
              uint8_t saturation = (sin16(frame + OFFSET) >> 8) + 128;
              leds[i] = CHSV(random(255), saturation, 255);
            } 
            else {
              leds[i] = CHSV(random(255), random(128, 255), 255);
            }
          } 
          else {
            leds[i] = CRGB::White;
          }
        }
      }
    }
    
    FastLED.delay(1000/FRAMES_PER_SECOND);    //Delay to a modest framerate
    LEDS.show();    //Display the LED array on the LED Matrix
  }
};
