//Start by including the required header for this file
#include "Effect.h"

//Create the Effect object
Effect::Effect(CRGB *leds, int width, int height): leds(leds), width(width), height(height) {}

//Define the inXRange function
bool Effect::inXRange(int x) {
    return x >= 0 && x < width;
}

//Define the inYRange function
bool Effect::inYRange(int y) {
    return y >= 0 && y < height;
}

//Define the pixel struct
struct CRGB& Effect::pixel(int x, int y) {
    if (y & 1) {
        return leds[(y * width) + width - x - 1];
    } 
    else {
        return leds[(y * width) + x];
    }
}

//Define the clearLeds function
void Effect::clearLeds() {
    memset8(leds, 0, width * height * 3);
}
