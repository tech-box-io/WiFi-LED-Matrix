//Start by including the required header for this file
#include "src/Effect.h"

//Create the Constant Color class
class ConstantColor : public Effect {
  
private:
  //Set the Framerate to 60 fps
  int FRAMES_PER_SECOND = 60;
    
public:
  //Create the ConstantColor constructor with correct variables
  ConstantColor(CRGB *leds, int width, int height) : Effect(leds, width, height) {}

  //Create the 'start' public function
  void start(int c1, int c2, int c3) {
    //For all LEDs, set the color to the given c1, c2, c3 -> R, G, B value
    for (int x = 0; x < width; x++) {
      for (int y = 0; y < height; y++) {
        leds[y + (8 * x)] = CRGB(c1, c2, c3);
      }
    }
    delay(17);        //Delay to keep the framerate modest
    FastLED.show();   //Display the LEDs
  }
};
