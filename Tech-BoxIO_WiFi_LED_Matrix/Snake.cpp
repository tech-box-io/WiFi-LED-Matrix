//Start by including the required header for this file
#include "src/Effect.h"

//Create the Snake object
class Snake : public Effect {

//Create the private variables and functions
private:
  static const byte SNAKE_LENGTH = 16;  //Set the snake length
  int FRAMES_PER_SECOND = 30;           //Set the Frames per second to a modest level
  
  enum Direction {
      UP, DOWN, LEFT, RIGHT   //Possible directions
  };

  //Create a Pixel structure
  struct Pixel {
      uint8_t x;
      uint8_t y;
  };

  //Create a CRGB object with the size of the snake length
  CRGB colours[SNAKE_LENGTH];
  uint8_t initialHue;


  //Create a pixel object with size of snake length
  Pixel pixels[SNAKE_LENGTH];

  //Define a direction
  Direction direction;

  //Create private function for new direction
  void newDirection() {
    switch (direction) {
      case UP:
      case DOWN:
        direction = random(0, 2) == 1 ? RIGHT : LEFT;   //Randomize direction changes
        break;
        
      case LEFT:
      case RIGHT:
        direction = random(0, 2) == 1 ? DOWN : UP;    //Randomize direction changes
        
      default:
        break;
    }
  }

  //Create private function for shuffling down the snake
  void shuffleDown() {
    for (byte i = SNAKE_LENGTH - 1; i > 0; i--) {
      pixels[i] = pixels[i - 1];
    }
  }


//Create the public functions
public:
  //Create the Snake constructor with the appropriate variables
  Snake(CRGB *leds, int width, int height) : Effect(leds, width, height), initialHue(0) {
    direction = UP; //Set the initial direction
    for (int i = 0; i < SNAKE_LENGTH; i++) {
      pixels[i].x = 0;
      pixels[i].y = 0;
    }
  }

  //Create the 'start' public function
  void start() {
    clearLeds();    //Clear the LEDs
    shuffleDown();  //Advance the snakes location
    if (random(10) > 6) { //If a random number out of 10 is greater than 6, change the direction of the snake
      newDirection();
    }
    switch (direction) { 
      case UP:
        pixels[0].y = (pixels[0].y + 1) % height;
        break;
      case LEFT:
        pixels[0].x = (pixels[0].x + 1) % width;
        break;
      case DOWN:
        pixels[0].y = pixels[0].y == 0 ? height - 1 : pixels[0].y - 1;
        break;
      case RIGHT:
        pixels[0].x = pixels[0].x == 0 ? width - 1 : pixels[0].x - 1;
        break;
    }
    fill_rainbow(colours, SNAKE_LENGTH, initialHue++);    //Fill the active snake pixels with color
    for (byte i = 0; i < SNAKE_LENGTH; i++) {
      pixel(pixels[i].x, pixels[i].y) = colours[i] %= (255 - i * (255 / SNAKE_LENGTH));
    }
    
    LEDS.show();    //Display to the LED Matrix
    FastLED.delay(1000/FRAMES_PER_SECOND);  //Delay to keep the framerate modest
    
    clearLeds();    //Clear the LEDs
  }
};
