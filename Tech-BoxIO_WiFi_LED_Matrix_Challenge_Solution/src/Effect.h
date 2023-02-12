//If not defined, then define Effect_h
#ifndef Effect_h
#define Effect_h

//Import the required library for use on this file
#import "FastLED.h"

//Create the Effect Class
class Effect {

  //Create the protected variables and functions of the class
  protected:
    CRGB *leds;
    int width, height;
  
    struct CRGB& pixel(int x, int y);
    bool inXRange(int x);
    bool inYRange(int y);

    void clearLeds();

  public:
    //Create the Effect constructor with the appropriate variables
    Effect(CRGB *leds, int width, int height);
  
    virtual void start() {};

};

#endif
