//Start by including the required header for this file
#include "src/Effect.h"

//Create the Stripes object
class Stripes : public Effect {
private:
    
public:
  //Create the Stripes constructor with the appropriate variables
  Stripes(CRGB *leds, int width, int height) : Effect(leds, width, height) {}

  //Create the 'stripe' public function
  void stripe(CRGB c1, CRGB c2, int width2) {
    for(int i=0; i< (height * width); i++) {  //Display alternating stripes based on the given color and stripe width
      if(i % (width2 * 2) < width2) {
        leds[i] = c1;
      }
      else {
        leds[i] = c2;
      } 
    }
    FastLED.show();   //Display the LED array on the LED Matrix
  }
};
