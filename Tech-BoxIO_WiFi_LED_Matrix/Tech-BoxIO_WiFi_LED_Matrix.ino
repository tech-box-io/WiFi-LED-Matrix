//Start by including the needed libraries for the ESP8266, FastLED, Adafruit GFX, and FastLED NeoMatrix
//Make sure that you have included the .zip libraries in your Arduino installation
#include <ESP8266WiFi.h>            // ESP8266 Wifi library
#include <ESP8266WebServer.h>       // Web server functions
#include <ESP8266mDNS.h>            // Web server functions
#include <SPI.h>                    // SPI Communication Library
#include <FastLED.h>                

//ADA
#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_I2CDevice.h>
//END ADA

#include <FastLED_NeoMatrix.h>

//Include the additional C++ files for the separate pattern routines
#include "Twinkle.cpp"
#include "Snake.cpp"
#include "Sprite.cpp"
#include "Sinelon.cpp"
#include "Stripes.cpp"
#include "ConstantColor.cpp"

//ESP8266 NodeMCU Pin Constants
static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10  = 1;

// Define the following variables 
#define NUM_LEDS 64           //Define the number of LEDs in the matrix
#define FINAL_LED_POS 63      //Define the position of the final LED
#define WIDTH 8               //Define the width of the LED Matrix
#define HEIGHT 8              //Define the height of the LED Matrix
#define LED_TYPE WS2812B      //Define the LED type for the LED Matrix
#define COLOR_ORDER GRB       //Define the color order the LED's need to display the appropriate color (Green, Red, Blue)
#define BRIGHTNESS 100        //Set the brightness of the LED Matrix
#define FRAMES_PER_SECOND 60  //Set the framerate of the LED Matrix (note: not all functions use this framerate)
#define DATA_PIN D2           //Set the Data Output pin for the NodeMCU to communicate with the LED Matrix
#define FORWARD 0             //Set the forward direction value for the LED Matrix
#define BACKWARD 1            //Set the backward direction value for the LED Matrix
#define SLOW 250              //Set the slow speed for the LED Matrix
#define MEDIUM 75             //Set the medium speed for the LED Matrix
#define FAST 20               //Set the fast speed for the LED Matrix

//Initialize the CRGB variables used in setting LED colors
CRGB leds[NUM_LEDS];
CRGB c1;
CRGB c2;
CRGBPalette16 palette = PartyColors_p;

//Define and Initialize the following variables
unsigned char     reduce = 1;       //Set the value used to reduce brightness of LED Pixels
String            webPage = "";     //Initialize an empty string for the webpage
String            temp = "";        //Initialize an empty string for a temporary value
String            Group1 = "O1";    //Set the value of Group1 to O1
String            customMessage = ""; //Initialize an empty string for the custom message field
String            spriteNumber = "";  //Initialize an empty string for the sprite number field
String            RVal = "";        //Initialize an empty string for the Red Value of the constant color field
String            GVal = "";        //Initialize an empty string for the Green Value of the constant color field
String            BVal = "";        //Initialize an empty string for the Blue Value of the constant color field

bool              direction = FORWARD;      //Set the initial direction to forward
bool              messageComplete = false;  //Create a flag for when the message is complete
byte              dothue = 0;               //Used to set a hue (color) value for a pixel
const int         msglen = 50;              //Maximum length of the message
uint8_t           gCurrentPatternNumber = 0; //Index value of which pattern is currently displayed
uint8_t           gHue = 0;                 //Rotating "base color" hue used by many of the patterns
uint8_t           BeatsPerMinute = 90;      //Set a beats per minute value for the BPM pattern
uint8_t           beat = beatsin8( BeatsPerMinute, 64, 255 ); //Setup the beat variable by using beatsin8
int               spriteNumberInt = 1;      //Set the integer version of the sprite number to be displayed by the Sprites function
int               RValInt = 0;              //Set the integer version of the Red Value to be displayed by Constant Color
int               GValInt = 0;              //Set the integer version of the Green Value to be displayed by Constant Color
int               BValInt = 0;              //Set the integer version of the Blue Value to be displayd by Constant Color
int               textLength = 0;           //Create a variable for holding the length of the text to be displayed
int               spriteIncrement = 0;      //Create a variable for indexing the current sprite on display
int               pos;                      //Create a variable for current position
int               m = 1;                    //Create a variable for cycling through the color wheel
int               lastspriteNumberInt = 0;  //Create a variable for tracking the last sprite number as an integer

//Initialize a snake object
Snake             snake(leds, WIDTH, HEIGHT);


//FastLED NeoMatrix
//Define the leds to be used, the width and height of the matrix, the matrix orientation and the matrix type
//For more information on this function, explore the FastLED NeoMatrix Library
FastLED_NeoMatrix *matrix = new FastLED_NeoMatrix(leds, WIDTH, HEIGHT, WIDTH/8, 1, 
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
    NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE );

//Define the colors for the matrix
const uint16_t colors[] = {
  matrix->Color(255, 0, 0), matrix->Color(0, 255, 0), matrix->Color(0, 0, 255) };

//Define the leds to be used, the width and height of the matrix, the matrix orientation and the matrix type
//For more information on this function, explore the FastLED NeoMatrix Library
FastLED_NeoMatrix *strip = new FastLED_NeoMatrix(leds, WIDTH, HEIGHT, WIDTH/8, 1, 
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
    NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE );
unsigned char matrixFire[WIDTH][HEIGHT];
unsigned char line[WIDTH];
int pcnt = 0;
int x    = matrix->width();
int pass = 0;
//END FastLED NeoMatrix


// Set your wifi credetials here, for open wifi leave password empty
//const char* ssid = "";
//const char* password = "";

WiFiClient        espClient;    //Define the WiFi Client
MDNSResponder     mdns;         //Define the MDNS Responder
ESP8266WebServer  server(80);   //Define the ESP8266 Web Server


////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {  
  Serial.begin(115200);

  // You can comment this section out for using DHCP
  //IPAddress ip(192, 168, 1, XX); // where xx is the desired IP Address
  //IPAddress gateway(192, 168, 1, 254); // set gateway to match your network
  //IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your network
  //WiFi.config(ip, gateway, subnet);   // Comment this line for using DHCP

  //Set the WiFi mode and connect using DHCP
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  //While not connected, print "." via Serial 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //Print the connection information and signal strength via serial once connected
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Signal [RSSI]: ");
  Serial.println(WiFi.RSSI());

  //Start the MDNS Responder
  if (mdns.begin("max7219_display", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }

  //Start the HTTP Server for the webpage
  server.on("/", handleMainPage);
  server.on("/set", handleSetCommand);        // Handling parameter set
  server.begin();
  Serial.println("HTTP server started");

  //Add LEDs for the FastLED library to use
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  //Set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  randomSeed(analogRead(A0));
  generateLine();
  
  //Start the FastLED_NeoMatrix Matrices
  strip->begin();
  matrix->begin();
  matrix->setTextWrap(false);
  matrix->setBrightness(40);
  matrix->setTextColor(colors[0]);

  //Must wait for the initial message display to complete to customize the LED Matrix
  while(messageComplete == false) {
    doMessage();
    server.handleClient();
  }
}  // end of setup
////////////////////////////////////////////////////////////////////////////////////////////////




// List of FastLED patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { doMessage, doSnake, stripes, doSprite, bpmWithGlitter, confetti, sinelon, colorWipe, Fire_2D, juggle, doTheaterChasingRainbow, bpm, doTwinkle, doRainbow, constantColor, off };




////////////////////////////////////////////////////////////////////////////////////////////////
//Create the HTML webpage the ESP8266 will use for the web server
//To explore the HTML, CSS, and Javascript of the webpage, look at the commented webpage code on this product's guide page
void handleMainPage() {
  
  webPage = "<html lang=\"en\"> <head> <meta charset=\"utf-8\"> <title>TechBox NeoPixel Matrix</title> </head> <style> .TechBox { background-color: #f1f1f1; color: #161616; border: 4px solid #00b0f0; margin: 1%; padding: 1%; font-family:\"Gill Sans\", \"Gill Sans MT\", \"Myriad Pro\", \"DejaVu Sans Condensed\", Helvetica, Arial, \"sans-serif\"; letter-spacing: 1px; font-size:120%; } .req02 { required: false; } .req05 { required: false; } .req16 { required: false; } .submit { width:180px; height:50px; font-size:25px; background: #00b0f0; color: #fff; border: 4px solid #363636; -webkit-appearance: none; } .submit:hover { background:#00bfff; } .box02{ color: #fff; padding: 5px; border: 4px solid #363636; display: none; margin-top: 5px; } .box05{ color: #fff; padding: 5px; border: 4px solid #363636; display: none; margin-top: 5px; } .box16{ color: #fff; padding: 5px; border: 4px solid #363636; display: none; margin-top: 5px; } .t02{ background: #00b0f0; } .t05{ background: #00b0f0; } .t16{ background: #00b0f0; } .container { display: block; position:relative; display:inline-block; float:left; cursor: pointer; font-size: 100%; -webkit-user-select: none; -moz-user-select: none; -ms-user-select: none; user-select: none; } .container input { position: absolute; opacity: 0; cursor: pointer; } .checkmark { position: absolute; top: 0px; left: -40px; height: 24px; width: 24px; background-color: #969696; border-radius: 50%; } .container:hover input ~ .checkmark { background-color: #767676; } .container input:checked ~ .checkmark { background-color: #00b0f0; } .checkmark:after { content: \"\"; position: absolute; display: none; } .container input:checked ~ .checkmark:after { display: block; } .container .checkmark:after { top: 8px; left: 8px; width: 8px; height: 8px; border-radius: 50%; background: white; } </style> <body  style=\"background-color:#262626;\"> <div class=\"TechBox\"> <center>";
  
  webPage += "<img src=\"data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHZpZXdCb3g9IjAgMCA1NDkuOTEgMTE3LjA4Ij48ZGVmcz48c3R5bGU+LmNscy0xe2ZpbGw6IzAwYjBmMDt9PC9zdHlsZT48L2RlZnM+PGcgaWQ9IkxheWVyXzIiIGRhdGEtbmFtZT0iTGF5ZXIgMiI+PGcgaWQ9IkxheWVyXzEtMiIgZGF0YS1uYW1lPSJMYXllciAxIj48cGF0aCBkPSJNNjIuNDUsMjEuMzVWMzcuODZINjkuNnY5LjU1TDQ0LjgsNjguMTZWMTIuNzFsOC41Myw4LjZMNTgsMTYuNjVsLTExLjg5LTEyTDQxLjQ0LDAsMzYuODIsNC42NmwtMTEuODksMTIsNC42Miw0LjY2TDM4LDEyLjg0Vjc4LjQ1TDEzLjA1LDU4Ljg4VjQ4LjM3QTkuNzEsOS43MSwwLDAsMCw5LjYyLDI5LjYxLDkuNzEsOS43MSwwLDAsMCw2LjE4LDQ4LjM3VjYxLjg4aC4wNkwzOCw4Ni43OXY0Ljc4YTEyLjg5LDEyLjg5LDAsMSwwLDYuODUsMFY3Ni43MUw3Ni41OSw1MC4xMmwtLjEtLjEyaC4xVjM3Ljg2aDcuMTZWMjEuMzVaIi8+PHBhdGggY2xhc3M9ImNscy0xIiBkPSJNMTU2LjYyLDEyLjEzVjMxLjkxSDE0MS4zNnY3OUgxMTUuNjd2LTc5aC0xNS4yVjEyLjEzWiIvPjxwYXRoIGNsYXNzPSJjbHMtMSIgZD0iTTE2Mi40OCwxMi4xM2g0Mi44NVYzMS45MUgxODguMThWNTAuNjVoMTZ2MTguOGgtMTZWOTEuMTdIMjA3VjExMUgxNjIuNDhaIi8+PHBhdGggY2xhc3M9ImNscy0xIiBkPSJNMjc0LjQyLDU1LjE2SDI0OC43M1YzOHEwLTcuNDktLjgzLTkuMzV0LTMuNjMtMS44NmMtMi4xMiwwLTMuNDYuNzYtNCwyLjI2cy0uODUsNC43Ni0uODUsOS43N3Y0NS45cTAsNy4yLjg1LDkuMzl0My44NSwyLjJjMS45MSwwLDMuMTYtLjczLDMuNzUtMi4ycy44OS00LjkyLjg5LTEwLjM0VjcxLjM0aDI1LjY5djMuODVxMCwxNS4zNS0yLjE2LDIxLjc2dC05LjU5LDExLjI1UTI1NS4yNiwxMTMsMjQ0LjM5LDExM3EtMTEuMjksMC0xOC42MS00LjA5YTE5Ljg4LDE5Ljg4LDAsMCwxLTkuNzEtMTEuMzNxLTIuMzgtNy4yMy0yLjM4LTIxLjc1VjQ3YTEzMC4yOCwxMzAuMjgsMCwwLDEsLjczLTE2LDIyLjQ3LDIyLjQ3LDAsMCwxLDQuMzctMTAuMjgsMjQuMjksMjQuMjksMCwwLDEsMTAuMDctNy43OCwzNi4yMiwzNi4yMiwwLDAsMSwxNC44LTIuODRxMTEuMzUsMCwxOC43NCw0LjM5dDkuNywxMC45NHEyLjMyLDYuNTYsMi4zMiwyMC4zOVoiLz48cGF0aCBjbGFzcz0iY2xzLTEiIGQ9Ik0zNDIuNzIsMTIuMTNWMTExSDMxN1Y2OS40NWgtNy42OVYxMTFIMjgzLjY0VjEyLjEzaDI1LjY5VjQ3LjQ3SDMxN1YxMi4xM1oiLz48cGF0aCBkPSJNMzU1LjkxLDEyLjEzaDI1LjYzcTEyLjE1LDAsMTguNCwxLjlhMTguMjksMTguMjksMCwwLDEsMTAuMTEsNy42NXEzLjg0LDUuNzYsMy44NCwxOC41NywwLDguNjctMi43MSwxMi4wOHQtMTAuNzIsNS4yNXE4LjkxLDIsMTIuMDksNi42OHQzLjE3LDE0LjMydjkuMTZxMCwxMC0yLjI5LDE0Ljg0YTEyLjMzLDEyLjMzLDAsMCwxLTcuMjksNi42cS01LDEuNzctMjAuNTEsMS43N0gzNTUuOTFaTTM4MS42LDI5VjUxYzEuMSwwLDItLjA2LDIuNTctLjA2UTM4OCw1MSwzODksNDkuMDl0MS0xMC42NXEwLTQuNjMtLjg2LTYuNWE0LjEsNC4xLDAsMCwwLTIuMjMtMi4zNUExOS45NCwxOS45NCwwLDAsMCwzODEuNiwyOVptMCwzNy4zNVY5NHE1LjQzLS4xOCw2LjkzLTEuNzF0MS41LTcuNVY3NS42MXEwLTYuMzUtMS4zNS03LjY5VDM4MS42LDY2LjM5WiIvPjxwYXRoIGQ9Ik00ODMuNjUsNzBxMCwxNC44OC0uNywyMS4wOGEyMy41OCwyMy41OCwwLDAsMS0xNC4zNywxOS4yQTM2LjM2LDM2LjM2LDAsMCwxLDQ1My45MywxMTNhMzcuMTksMzcuMTksMCwwLDEtMTQuMjUtMi42LDIzLjc2LDIzLjc2LDAsMCwxLTE0Ljc0LTE5LjA3QTIwOC4zMywyMDguMzMsMCwwLDEsNDI0LjIxLDcwVjUzLjA5cTAtMTQuOS43LTIxLjA5YTIzLjU5LDIzLjU5LDAsMCwxLDE0LjM3LTE5LjE5LDM2LjE2LDM2LjE2LDAsMCwxLDE0LjY1LTIuNzUsMzcuMTksMzcuMTksMCwwLDEsMTQuMjUsMi41OSwyMy43OSwyMy43OSwwLDAsMSwxNC43NCwxOS4wOCwyMDguMzMsMjA4LjMzLDAsMCwxLC43MywyMS4zNlpNNDU4LDM3LjUzcTAtNi45LS43Ny04LjgyYTMsMywwLDAsMC0zLjE0LTEuOTNBMy41LDMuNSwwLDAsMCw0NTEsMjguMzRxLTEuMDcsMS41Ni0xLjA3LDkuMTlWODMuNjdjMCw1Ljc0LjI0LDkuMjguNywxMC42MmEzLjA3LDMuMDcsMCwwLDAsMy4yNywyQTMuMTQsMy4xNCwwLDAsMCw0NTcuMjIsOTRjLjQ5LTEuNTQuNzQtNS4yMy43NC0xMVoiLz48cGF0aCBkPSJNNTQ1LjMzLDEyLjEzbC05LDQzLjcxTDU0OS45MSwxMTFINTI2LjFhMzI1LjgxLDMyNS44MSwwLDAsMS03LjgxLTM2cS0xLDkuNDEtMi43MSwxOS45TDUxMy4xNiwxMTFoLTI1bDkuMzQtNTUuMTEtOS4zNC00My43MUg1MTNxLjY3LDQuMDksMi42OCwxMy42MSwxLjYxLDcuMjYsMi42MywxMy4zN2w1LjQ5LTI3WiIvPjwvZz48L2c+PC9zdmc+\"";
  
  webPage += "alt=\"Tech-Box.io Logo\" style=\"padding-top: 25px; max-width: 450px;\"> <h1>RGB NeoPixel Matrix</h1> <p style=\"margin: 5px; margin-bottom: 20px;\">This sketch lets the user select the LED mode for the 8x8 pixel WS2812b display.</p> <p style=\"margin: 5px; margin-bottom: 20px;\">Please select one of the options below and click \"Update\" to update the screen:</p> <div style=\"display: inline-block;\">";
  
  webPage += "<form action=\"/set\"> <label class=\"container\">Demo <input type=\"radio\"";
  if(Group1 == "O1") {
    webPage += " checked ";
  }

  webPage += " name=\"Group1\" value=\"O1\"> <span class=\"checkmark\"></span> </label><br><br> <label class=\"container\">Custom Message <input type=\"radio\"";
  if(Group1 == "t02") { webPage += " checked "; }
  
  webPage += " name=\"Group1\" value=\"t02\"> <span class=\"checkmark\"></span> </label><br><br> <label class=\"container\">Twinkle <input type=\"radio\"";
  if(Group1 == "O3") { webPage += " checked "; }
  
  webPage += "name=\"Group1\" value=\"O3\"> <span class=\"checkmark\"></span> </label><br><br> <label class=\"container\">Snake <input type=\"radio\"";
  if(Group1 == "O4") { webPage += " checked "; }
  
  webPage += "name=\"Group1\" value=\"O4\"> <span class=\"checkmark\"></span> </label><br><br> <label class=\"container\">Sprites <input type=\"radio\"";
  if(Group1 == "t05") { webPage += " checked "; }
  
  webPage += "name=\"Group1\" value=\"t05\"> <span class=\"checkmark\"></span> </label><br><br> <label class=\"container\">Fire! <input type=\"radio\"";
  if(Group1 == "O6") { webPage += " checked "; }
  
  webPage += "name=\"Group1\" value=\"O6\"> <span class=\"checkmark\"></span> </label><br><br> <label class=\"container\">Stripes <input type=\"radio\"";
  if(Group1 == "O7") { webPage += " checked "; }
  
  webPage += "name=\"Group1\" value=\"O7\"> <span class=\"checkmark\"></span> </label><br><br> <label class=\"container\">Juggle <input type=\"radio\"";
  if(Group1 == "O8") { webPage += " checked "; }
  
  webPage += "name=\"Group1\" value=\"O8\"> <span class=\"checkmark\"></span> </label><br><br> <label class=\"container\">Confetti <input type=\"radio\"";
  if(Group1 == "O9") { webPage += " checked "; }
  
  webPage += "name=\"Group1\" value=\"O9\"> <span class=\"checkmark\"></span> </label><br><br> <label class=\"container\">Scanning <input type=\"radio\"";
  if(Group1 == "10") { webPage += " checked "; }
  
  webPage += "name=\"Group1\" value=\"10\"> <span class=\"checkmark\"></span> </label><br><br> <label class=\"container\">Beats per Minute (BPM) <input type=\"radio\"";
  if(Group1 == "11") { webPage += " checked "; }
  
  webPage += "name=\"Group1\" value=\"11\"> <span class=\"checkmark\"></span> </label><br><br> <label class=\"container\">BPM with Glitter <input type=\"radio\"";
  if(Group1 == "12") { webPage += " checked "; }
  
  webPage += "name=\"Group1\" value=\"12\"> <span class=\"checkmark\"></span> </label><br><br> <label class=\"container\">Theater Chasing <input type=\"radio\"";
  if(Group1 == "13") { webPage += " checked "; }
  
  webPage += "name=\"Group1\" value=\"13\"> <span class=\"checkmark\"></span> </label><br><br> <label class=\"container\">Rainbow <input type=\"radio\"";
  if(Group1 == "14") { webPage += " checked "; }
  
  webPage += "name=\"Group1\" value=\"14\"> <span class=\"checkmark\"></span> </label><br><br> <label class=\"container\">Color Wipe <input type=\"radio\"";
  if(Group1 == "15") { webPage += " checked "; }
  
  webPage += "name=\"Group1\" value=\"15\"> <span class=\"checkmark\"></span> </label><br><br> <label class=\"container\">Constant Color <input type=\"radio\"";
  if(Group1 == "t16") { webPage += " checked "; }
  
  webPage += "name=\"Group1\" value=\"t16\"> <span class=\"checkmark\"></span> </label><br><br> <label class=\"container\">Off <input type=\"radio\"";
  if(Group1 == "17") { webPage += " checked "; }
  
  webPage += "name=\"Group1\" value=\"17\"> <span class=\"checkmark\"></span> </label><br><br> <div class=\"t02 box02\"> <p> <strong>*All Fields Required*</strong> <br><br> <label> Message:<br> <input type=\"text\" name=\"customMessage\" value=\"";
  if(Group1 == "t02") { webPage += customMessage; }
  
  webPage += "\" style=\"width:200px\" class=\"req02\" maxlength=\"50\"> </label> <br><br> Max message length - 50 characters </p> </div> <div class=\"t05 box05\"> <p> <strong>*All Fields Required*</strong> <br><br> 0 - Rotate All<br> 1 - Heart<br> 2 - Pac-Man Pink Ghost<br> 3 - Pac-Man Purple Ghost<br> 4 - Pac-Man Green Ghost<br> 5 - Pac-Man Blue Ghost<br> 6 - Pac-Man<br> 7 - Charged Creeper<br> 8 - Mario<br> 9 - Mario Shy Guy<br> 10 - Kirby<br> 11 - Adventure Time Finn<br> 12 - Creeper<br> 13 - Yoshi<br> 14 - Blue Potion<br> 15 - Green Potion<br> 16 - Orange Potion<br> 17 - Red Potion<br> 18 - Pikachu<br><br> <label> Sprite Selection:<br> <input type=\"number\" name=\"spriteSelect\" value=\"";
  if(Group1 == "t05") { webPage += spriteNumber; }
  
  webPage += "\" style=\"width:120px\" class=\"req05\" max=\"18\" min=\"0\" step=\"1\"> </label> <br><br> </p> </div> <div class=\"t16 box16\">  <p> <strong>*All Fields Required*</strong> <br><br> <label> R: <input type=\"number\" name=\"R\" value=\"";
  if(Group1 == "t16") { webPage += RVal; }
  
  webPage += "\" style=\"width:40px\" class=\"req16\" max=\"255\" min=\"0\" step=\"1\"> <label> &nbsp <label> G: <input type=\"number\" name=\"G\" value=\"";
  if(Group1 == "t16") { webPage += GVal; }
  
  webPage += "\" style=\"width:40px\" class=\"req16\" max=\"255\" min=\"0\" step=\"1\"> </label> &nbsp <label> B: <input type=\"number\" name=\"B\" value=\"";
  if(Group1 == "t16") { webPage += BVal; }
  
  webPage += "\" style=\"width:40px\" class=\"req16\" max=\"255\" min=\"0\" step=\"1\"> </label> <br> <br> Enter values between 0 and 255 </p> </div> <br> <input class=\"submit\" type=\"submit\" value=\"Update\"> </form> </div> <p style=\"font-size: 90%; color: #000;\"> RSSI: ";

  //Get the WiFi Signal strength and add it to the webpage display
  webPage += WiFi.RSSI();
  if(WiFi.RSSI() > -70) { webPage += " [Excellent]"; }
  else if(WiFi.RSSI() > -85) { webPage += " [Good]"; }
  else if(WiFi.RSSI() > -100) { webPage += " [Fair]"; }
  else { webPage += " [Poor]"; }

  //Get the ESP8266 uptime and add it to the webpage display
  webPage += "<br/>";
  webPage += "Uptime [min]: ";
  webPage += millis() / (1000*60);
  
  webPage += "</p> <p style=\"font-size: 80%; color: #08088A;\"> TechBox RGB NeoPixel Matrix v11.05 |  <a href=\"mailto:support@tech-box.io\">Email Us!</a> | <a href=\"https://tech-box.io/blogs/guides/wifi-led-matrix-instruction-guide\" target=\"_blank\">Project Files</a> </p> </center> </div>";
  
  //Note: The network must have a valid internet connection for this script to run. If the network is not connected to the internet, the script will not run, and conditional display boxes will NOT display
  webPage += "<script src=\"https://code.jquery.com/jquery-1.12.4.min.js\"></script> <script> $(document).ready(function(){ $('input[type=\"radio\"]').click(function(){ var inputValue = $(this).attr(\"value\"); var targetBox = $(\".\" + inputValue); if(targetBox.not(\".box02\")){ $(\".req02\").prop(\"required\", false); $(\".box02\").not(targetBox).hide(); } if(targetBox.not(\".box05\")){ $(\".req05\").prop(\"required\", false); $(\".box05\").not(targetBox).hide(); } if(targetBox.not(\".box16\")){ $(\".req16\").prop(\"required\", false); $(\".box16\").not(targetBox).hide(); } if(targetBox.is(\".box02\")){ $(\".req02\").prop(\"required\", true); $(\".box02\").show(); } if(targetBox.is(\".box05\")){ $(\".req05\").prop(\"required\", true); $(\".box05\").show(); } if(targetBox.is(\".box16\")){ $(\".req16\").prop(\"required\", true); $(\".box16\").show(); }});});</script></body></html>";

  //Send the webpage to the client and print the action via serial
  server.send(200, "text/html", webPage); 
  Serial.println("Web page request");
}
////////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////
//Create a function to handle the Set Command
void handleSetCommand() { 
  String response = "";

  //Check the Argument's parameters
  if (server.args() == 0) { //If parameters are empty, set the response accordingly
    response = "No parameter";
  } 
  else { //If the parameter is Group1, set the server argument and response accordingly
    if (server.argName(0) == "Group1") {
      Group1 = server.arg("Group1");
      response = " Mode updated! ";
    }
    
    if(Group1 == "t02") { //If Group1 is equal to t02, set the Custom Message string and change/add to the response accordingly
      if(server.argName(1) == "customMessage") {
        customMessage = server.arg("customMessage");
        if(customMessage != "") {
          response += " Custom message updated to: ";
        }        
        response += customMessage;
      }
    }
    if(Group1 == "t05") { //If Group1 is equal to t05, set the sprite number selected and change/add to the response accordingly
      if(server.argName(2) == "spriteSelect") {
        spriteNumber = server.arg("spriteSelect");
        if(spriteNumber != "") {
          spriteNumberInt = spriteNumber.toInt();
          response += " Sprite number updated to: ";
        }
        response += spriteNumber;
      }
    }
    if(Group1 == "t16") { //If Group1 is equal to t16, set the following values and responses
      if(server.argName(3) == "R") { //Set the Red Value and change/add to the response accordingly
        RVal = server.arg("R");
        if(RVal != "") {
          RValInt = RVal.toInt();
          response += " Red Value updated to: ";
        }
        response += RVal;
      }
      if(server.argName(4) == "G") { //Set the Green Value and change/add to the response accordingly
        GVal = server.arg("G");
        if(GVal != "") {
          GValInt = GVal.toInt();
          response += " Green Value updated to: ";
        }
        response += GVal;
      }
      if(server.argName(5) == "B") { //Set the Blue Value and change/add to the response accordingly
        BVal = server.arg("B");
        if(BVal != "") {
          BValInt = BVal.toInt();
          response += " Blue Value updated to: ";
        }
        response += BVal;
      }
    }
    if (response == "" ) {
      response = "invalid parameter";
    }
  }

  //Print the change request response that was created via serial
  Serial.print("Change request: ");
  Serial.println(response);

  //Format the response to HTML and send the response to the client
  response = "<html><head><meta http-equiv=\"refresh\" content=\"2; url=/\"></head><body>" + response + "</body></html>";
  server.send(200, "text/html", response);  //Returns the HTTP response
}
////////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////
void loop() { 
  //Handle the Client Event
  server.handleClient();

  //Begin desired routine based on Group1 value
  if(Group1 == "O1") { //Demo
    demo();
  }
  else if(Group1 == "t02") { //Custom Message
    selectMessage();
  }
  else if(Group1 == "O3") { //Twinkle
    doTwinkle();
  }
  else if(Group1 == "O4") { //Snake
    doSnake();
  }
  else if(Group1 == "t05") { //Sprites
    selectSprite();
  }
  else if(Group1 == "O6") { //2D Fire
    Fire_2D();
  }
  else if(Group1 == "O7") { //Stripes
    stripes();
  }
  else if(Group1 == "O8") { //Juggle
    juggle();
  }
  else if(Group1 == "O9") { //Confetti
    confetti();
  }
  else if(Group1 == "10") { //Scanning
    sinelon();
  }
  else if(Group1 == "11") { //Beats per Minute (BPM)
    bpm();
  }
  else if(Group1 == "12") { //BPM with Glitter
    bpmWithGlitter();
  }
  else if(Group1 == "13") { //Theater Chasing
    theaterChaseRainbow(1,MEDIUM);  //You can change the speed based on changing the second value in the function call "MEDIUM" ?
  }
  else if(Group1 == "14") { //Rainbow
    rainbow(1,1);
  }
  else if(Group1 == "15") { //Color Wipe
    colorWipe();
  }
  else if(Group1 == "t16") { //Constant Color
    selectconstantColor();
  }
  else if(Group1 == "17") { //Off
    off();
  }
}  // end of loop
////////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////
//Function for advancing current pattern when in demo mode
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern() {
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}
////////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////
//Demo Mode with auto-rotating patterns
void demo() {
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  
  //*NOTE: NOT ALL PATTERNS WILL CHANGE ON THE TIMER, doTwinkle, doSprite, and doSnake CHANGE EVERY ~30 SECONDS*
  EVERY_N_SECONDS( 20 ) { FastLED.clear(); FastLED.show(); nextPattern(); } // change patterns periodically (every 20 seconds) 
}
////////////////////////////////////////////////////////////////////////////////////////////////




//Turns all LEDs off
////////////////////////////////////////////////////////////////////////////////////////////////
void off() {
  FastLED.clear();
  FastLED.show();
}
////////////////////////////////////////////////////////////////////////////////////////////////




//Send a Message via the LEDs
////////////////////////////////////////////////////////////////////////////////////////////////
//Create initial message to show what IP Address on the same WiFi network to go to for customization
void doMessage() {
  temp = "Tech-Box.io - Visit http://";
  temp += WiFi.localIP().toString();
  temp +=" to customize";
  message(temp);
}

//Print the initial message to the LED Matrix, wait 100ms between refreshes
void message(String text) {
  matrix->fillScreen(0);    //Clear the LED Matrix
  matrix->setCursor(x, 0);  //Set the LED Cursor location
  matrix->print(text);      //Print the text
  textLength = text.length() * -7;  //Find the text length and offset it for smooth wraparound
  if(--x < textLength) {    
    x = matrix->width();
    if(++pass >= 3) pass = 0;
    matrix->setTextColor(colors[pass]);   //Rotate colors through Red, Green and Blue on each pass
    messageComplete = true;
  }
  matrix->show();   //Display to the matrix
  delay(100);
}
////////////////////////////////////////////////////////////////////////////////////////////////




//Send a Message over the LEDs
////////////////////////////////////////////////////////////////////////////////////////////////
//Create a custom message string that can be printed to the LED Matrix
void selectMessage() {
  String mess = customMessage;
  selectMessage(mess);
}

//Print the custom message to the LED Matrix
void selectMessage(String text) {
  matrix->fillScreen(0);    //Clear the LED Matrix
  matrix->setCursor(x, 0);  //Set the LED Cursor location
  matrix->print(text);      //Print the text
  textLength = text.length() * -7;    //Find the text length and offset it for smooth wraparound
  //Serial.println(textLength);
  if(--x < textLength) {
    x = matrix->width();
    if(++pass >= 3) pass = 0;
    matrix->setTextColor(colors[pass]);     //Rotate colors through Red, Green and Blue on each pass
  }
  matrix->show();   //Display to the matrix
  delay(100);
}
////////////////////////////////////////////////////////////////////////////////////////////////




//LEDs twinkle and change color
////////////////////////////////////////////////////////////////////////////////////////////////
//Start the Twinkle pattern by calling the Twinkle.cpp file and function
void doTwinkle() {
  Twinkle twinkle(leds, WIDTH, HEIGHT, true, true);
  twinkle.start();
}
////////////////////////////////////////////////////////////////////////////////////////////////




//LEDs move like a Snake
////////////////////////////////////////////////////////////////////////////////////////////////
//Start the Snake pattern by calling the Snake.cpp file and function
void doSnake() {
  snake.start();
}
////////////////////////////////////////////////////////////////////////////////////////////////




//LEDs make an 8x8 image
////////////////////////////////////////////////////////////////////////////////////////////////
//Start the Sprite Pattern by calling the Sprite.cpp file and function under the Demo Mode
void doSprite() {
  Sprite sprite(leds, WIDTH, HEIGHT); //Pass the required sprite parameters: leds and LED Matrix Width and Height
  sprite.start(0,spriteIncrement);    //Start the sprite function
  delay(2000);                        //Display the sprite for 2 seconds then loop
  spriteIncrement = spriteIncrement + 1;  //Increment the sprite displayed when in demo mode
  if(spriteIncrement == 18) {         //If on the final sprite, restart with the first sprite
    spriteIncrement = 0;  
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////




//LEDs make an 8x8 image
////////////////////////////////////////////////////////////////////////////////////////////////
//Start the Sprite Pattern by calling the Sprite.cpp file and function under the customized selection
void selectSprite() {
  Sprite sprite(leds, WIDTH, HEIGHT); //Pass the required sprite parameters: leds and LED Matrix Width and Height
  if(spriteNumberInt == 0) {          //If the sprite selection is 0, continue rotating through the different sprites
    sprite.start(spriteNumberInt, spriteIncrement);
    delay(2000);
    spriteIncrement = spriteIncrement + 1;  //Increment the sprite displayed while rotating
    if(spriteIncrement == 18) {
      spriteIncrement = 0;
    }
  }
  else {  //If the sprite selection is not 0, then display the selected sprite indefinitely
    if(spriteNumberInt != lastspriteNumberInt) {
      sprite.start(spriteNumberInt, spriteIncrement);
    }
    lastspriteNumberInt = spriteNumberInt;
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////




//Set all LEDs to a Constant Color
////////////////////////////////////////////////////////////////////////////////////////////////
//Set the LED Matrix to a constant color (White) in demo mode by calling the ConstantColor.cpp file and function
void constantColor() {
  ConstantColor constantColor(leds, WIDTH, HEIGHT);
  constantColor.start(255, 255, 255);
}
////////////////////////////////////////////////////////////////////////////////////////////////




//Set all LEDs to a Constant Color
////////////////////////////////////////////////////////////////////////////////////////////////
//Set the LED Matrix to a constant color based on user input by calling the ConstantColor.cpp file and function
void selectconstantColor() {
  ConstantColor constantColor(leds, WIDTH, HEIGHT);
  constantColor.start(RValInt, GValInt, BValInt);
}
////////////////////////////////////////////////////////////////////////////////////////////////




//LEDs scan back and forth on all lines at once, "juggling"
////////////////////////////////////////////////////////////////////////////////////////////////
//Create a juggling pattern that "bounces" the LEDs side to side at different speeds and colors
void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20); //Fade each LED slowly
  
  dothue = 0;   //Initial Dot Hue
  for( int i = 0; i < 8; i++) { //Increment the hue and update LEDs
    leds[beatsin16(i+7,0,FINAL_LED_POS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
  
  //Send the 'leds' array out to the actual LED strip
  FastLED.show();  
  
  //Insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 
  
  //Do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } //Slowly cycle the "base color" through the rainbow
}
////////////////////////////////////////////////////////////////////////////////////////////////




//Effect to add glitter to different patterns
////////////////////////////////////////////////////////////////////////////////////////////////
//Using a random number, randomly add sparks of glitter to various patterns when called
void addGlitter( fract8 chanceOfGlitter) {
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////




//LEDs sparkly like confetti
////////////////////////////////////////////////////////////////////////////////////////////////
//Create randomly colored speckles that flash in quickly and fade slowly
void confetti() {
  fadeToBlackBy( leds, NUM_LEDS, 10); //Fade the leds slowly over time
  pos = random16(NUM_LEDS);           //Generate a random LED to illuminate
  leds[pos] += CHSV( gHue + random8(64), 200, 255); //Generate a random color for the random LED to recieve
  
  //Send the 'leds' array out to the actual LED strip
  FastLED.show();  
  
  //Insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND);
  
  //Do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } //Slowly cycle the "base color" through the rainbow
}
////////////////////////////////////////////////////////////////////////////////////////////////




//LEDs scan across the grip
////////////////////////////////////////////////////////////////////////////////////////////////
//Start the Sinelon pattern by calling the Sinelong.cpp file and function
void sinelon() {
  Sinelon sinelon(leds, WIDTH, HEIGHT); //Pass the required parameters to the sinelong function: leds and Matrix Width and Height
  sinelon.start(gHue);
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } //Slowly cycle the "base color" through the rainbow
}
////////////////////////////////////////////////////////////////////////////////////////////////




//LEDs are synced to "X" beats per minute
//////////////////////////////////////////////////////////////////////////////////////////////// 
void bpm() {
  //Colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  BeatsPerMinute = 90;              //Set the desired beats per minute (can set to match a desired song)
  palette = PartyColors_p;          //Choose the desired color palette from the palette presets
  beat = beatsin8( BeatsPerMinute, 64, 255);    //Configure the beat
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*22));    //Create 'leds' array to store illumination values
  }
  
  //Send the 'leds' array out to the actual LED strip
  FastLED.show();  
  
  //Insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND);
  
  //Do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } //Slowly cycle the "base color" through the rainbow
}
////////////////////////////////////////////////////////////////////////////////////////////////




//LEDs are synced to "X" beats per minute, *BUT WITH GLITTER*
////////////////////////////////////////////////////////////////////////////////////////////////
void bpmWithGlitter() {
  //Colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  BeatsPerMinute = 90;            //Set the desired beats per minute (can set to match a desired song)
  palette = PartyColors_p;        //Choose the desired color palette from the palette presets
  beat = beatsin8( BeatsPerMinute, 64, 255);    //Configure the beat
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*22));    //Create 'leds' array to store illumination values
  }
  addGlitter(80);   //Add sparkles via the "addGlitter" function above
  
  //Send the 'leds' array out to the actual LED strip
  FastLED.show();  
  
  //Insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND);
  
  //Do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
}
////////////////////////////////////////////////////////////////////////////////////////////////




// Wipes color from end to end
////////////////////////////////////////////////////////////////////////////////////////////////
void colorWipe() {
  //Wipe and change the color of the LED matrix from one corner to the other
  for(int i=0; i<2; i++) { 
    colorWipeLowLevel(randomColor(),FAST,direction);  //Set the speed and direction of the color wipe
    direction = !direction;   //Change the direction after completing the color wipe (wipe in reverse)
  }
}

//Lower level function for changing the pixel color in the correct sequence
void colorWipeLowLevel(CRGB c, int speed, int direction) {
  for(int i=0; i<NUM_LEDS; i++) {   //Complete this function for the full number of LEDs on the Matrix
    if(direction == FORWARD) {    //If the direction is going forward, then set LED[i] to color 'c'
      leds[i] = c;
    }
    else {                        //Else, if the direction is not forward, then set LED[total number of leds - 1 - i] to color 'c'
      leds[NUM_LEDS-1-i] = c;
    }
    FastLED.show();   //Show the LEDs on the matrix
    delay(speed);     //Delay the given speed to slow the refresh rate
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////




// Rainbow colors that slowly cycle across LEDs
////////////////////////////////////////////////////////////////////////////////////////////////
void rainbow(int cycles, int speed) {
  if(cycles == 0) {
    for(int i=0; i< NUM_LEDS; i++) {  //For each LED on the Matrix
      leds[i] = Wheel(((i * 256 / NUM_LEDS)) & 255);  //Set the color of LED[i] to the color generated by the Wheel function below
    }
    FastLED.show();   //Show the LEDs on the Matrix
  }
  else {
    for(int j=0; j<256*cycles; j++) {   //For each of the cycles given
      for(int i=0; i< NUM_LEDS; i++) {    //And for each of the LEDs on the matrix
        leds[i] = Wheel(((i * 256 / NUM_LEDS) + j) & 255);    //Set the color of each LED to the color generated by the Wheel function given i and j
      }
      FastLED.show();   //Show the LEDs on the Matrix
      delay(speed);     //Delay the given speed to slow the refresh rate
    }
  }
}

void doRainbow(){   //Call the lower level rainbow function in a format that matches the other function calls
  rainbow(1, 1);
}
////////////////////////////////////////////////////////////////////////////////////////////////




// Theater-style crawling lights with rainbow effect on the LED grid
////////////////////////////////////////////////////////////////////////////////////////////////
void theaterChaseRainbow(int cycles, int speed) {
  //Cycle colors in the wheel, increment by 4
  if(m >= 255) {  //If the color value for the wheel is above the maximum of 255, reset to 1
    m = 1;
  }
  else {    //Otherwise increment the color value for the wheel by 4
     m + 4;
  }
  
  for (int q=0; q < 3; q++) {   //For each of the possible arrangements when turning on every 3rd LED (ie, turn on LED 1, LED2, and LED3 on their respective loops)
    for (int i=0; i <= FINAL_LED_POS; i=i+3) {
      pos = i+q;
      leds[pos] = Wheel( (i+m) % 255);    //Turn every third pixel on
    }
    
    FastLED.show();   //Show the LEDs on the Matrix

    delay(speed);   //Delay the given speed to slow the refresh rate

    for (int i=0; i <= FINAL_LED_POS; i=i+3) {
      leds[i+q] = CRGB::Black;  //Turn every third pixel off
    }
  }
}

void doTheaterChasingRainbow() {  //Call the lower level theater Chase Rainbow function in a format that matches the other function calls
  theaterChaseRainbow(1, MEDIUM); 
}
////////////////////////////////////////////////////////////////////////////////////////////////




//Alternating stripe patterns
////////////////////////////////////////////////////////////////////////////////////////////////
void stripes() {
  Stripes stripes(leds, WIDTH, HEIGHT);   //Create the stripes Object
  
  for(int i=0; i<3; i++) {    //For 3 loops, execute the following
    if((Group1 != "O7") && (Group1 != "O1")) { break; } //If the Group1 value has changed since starting the loop, stop the function
    c1 = randomColor();       //Assign c1 a random color
    c2 = randomColor();       //Assign c2 a random color
    stripes.stripe(c1,c2,2);  //Call the Stripes.cpp file and function
    server.handleClient();    //Check the server to see if there are any client args to handle
    delay(1000);              //Delay 1 second (display the current stripe pattern for 1 second)
    stripes.stripe(c2,c1,2);  //Call the Stripes.cpp file and function, with reversed colors
    server.handleClient();    //Check the server to see if there are any client args to handle
    delay(1000);              //Delay 1 second (display the current srtipe pattern for 1 second)
  }
  for(int i=0; i<3; i++) {
    if((Group1 != "O7") && (Group1 != "O1")) { break; }
    c1 = randomColor();       //Assign c1 a random color
    c2 = randomColor();       //Assign c1 a random color
    stripes.stripe(c1,c2,3);  //Call the Stripes.cpp file and function
    server.handleClient();    //Check the server to see if there are any client args to handle
    delay(1000);              //Delay 1 second (display the current srtipe pattern for 1 second)
    stripes.stripe(c2,c1,3);  //Call the Stripes.cpp file and function, with reversed colors
    server.handleClient();    //Check the server to see if there are any client args to handle
    delay(1000);              //Delay 1 second (display the current srtipe pattern for 1 second)
  }
  for(int i=0; i<3; i++) {
    if((Group1 != "O7") && (Group1 != "O1")) { break; }
    c1 = randomColor();       //Assign c1 a random color
    c2 = randomColor();       //Assign c1 a random color
    stripes.stripe(c1,c2,4);  //Call the Stripes.cpp file and function
    server.handleClient();    //Check the server to see if there are any client args to handle
    delay(1000);              //Delay 1 second (display the current srtipe pattern for 1 second)
    stripes.stripe(c2,c1,4);  //Call the Stripes.cpp file and function, with reversed colors      
    server.handleClient();    //Check the server to see if there are any client args to handle
    delay(1000);              //Delay 1 second (display the current srtipe pattern for 1 second)
  }
  for(int i=0; i<3; i++) {
    if((Group1 != "O7") && (Group1 != "O1")) { break; }
    c1 = randomColor();       //Assign c1 a random color  
    c2 = randomColor();       //Assign c1 a random color
    stripes.stripe(c1,c2,5);  //Call the Stripes.cpp file and function
    server.handleClient();    //Check the server to see if there are any client args to handle
    delay(1000);              //Delay 1 second (display the current srtipe pattern for 1 second)
    stripes.stripe(c2,c1,5);  //Call the Stripes.cpp file and function, with reversed colors
    server.handleClient();    //Check the server to see if there are any client args to handle
    delay(1000);              //Delay 1 second (display the current srtipe pattern for 1 second)
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////




//LEDs create a rising 2D Fire
////////////////////////////////////////////////////////////////////////////////////////////////
//A color with 3 components: r(red), g(green) and b(blue)
typedef struct {
  unsigned char r;
  unsigned char g;
  unsigned char b;
} ColorRGB;

//A color with 3 components: h(hue), s(saturation) and v(value)
typedef struct {
  unsigned char h;
  unsigned char s;
  unsigned char v;
} ColorHSV;

//These values are substracetd from the generated values to give a shape to the animation
const unsigned char valueMask[WIDTH][HEIGHT]= {
  {64 , 0  , 0  , 0  , 0  , 0  , 0  , 64 },
  {96 , 32 , 0  , 0  , 0  , 0  , 32 , 96 },
  {128, 64 , 32 , 0  , 0  , 32 , 64 , 128},
  {160, 96 , 64 , 32 , 32 , 64 , 96 , 160},
  {192, 128, 96 , 64 , 64 , 96 , 128, 192},
  {255, 160, 128, 96 , 96 , 128, 160, 255},
  {255, 192, 160, 128, 128, 160, 192, 255},
  {255, 255, 192, 160, 160, 192, 255, 255},
};

//These are the hues for the fire, 
//Ranging between 0 (~red) to 9 (~yellow)
const unsigned char hueMask[WIDTH][HEIGHT]= {
  {1, 4, 7, 9, 9, 8, 4, 1},
  {1, 3, 5, 7, 9, 7, 3, 1},
  {1, 3, 5, 6, 7, 6, 3, 1},
  {1, 2, 4, 5, 5, 5, 2, 1},
  {1, 2, 4, 4, 4, 4, 2, 1},
  {0, 1, 2, 3, 3, 2, 1, 0},
  {0, 0, 1, 2, 2, 1, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0}
};

//Create the animation by shifting the line upward to give the illusion of motion
void Fire_2D() {
  if (pcnt >= 100) {  //If the frame has been displayed for long enough
    shiftUp();        //Call the shiftUp function to shift the entire matrix upward on the LED Matrix
    generateLine();   //Call the generateLine function to generate a new line for the bottom row of the LED Matrix
    pcnt = 0;         //Set the percentage of time displayed to 0
  }
  drawFrame(pcnt);    //Call the drawFrame function to interpolate the lines
  pcnt+=30;           //Increment the percentage
  delay(20);          //Delay a modest amount of time to slow the refresh rate
}

//Converts an HSV color to RGB color
void HSVtoRGB(void *vRGB, void *vHSV) {
  float r, g, b, h, s, v; //This function works with floats between 0 and 1
  float f, p, q, t;
  int i;
  ColorRGB *colorRGB=(ColorRGB *)vRGB;
  ColorHSV *colorHSV=(ColorHSV *)vHSV;

  //Obtain float values between 0 and 1 for the given H, S and V values
  h = (float)(colorHSV->h / 256.0);  
  s = (float)(colorHSV->s / 256.0);
  v = (float)(colorHSV->v / 256.0);

  //If saturation is 0, the color is a shade of grey
  if(s == 0.0) {
    b = v;
    g = b;
    r = g;
  }
  //If saturation > 0, more complexity is needed
  else {
    h *= 6.0;             //Bring hue to a number between 0 and 6 for easier calculations
    i = (int)(floor(h));  //Round down the decimal value (i.e. 2.5 becomes 2, 3.02 becomes 3, and 4.9988 becomes 4)
    f = h - i;            //Obtain the fractional portion of h

    p = (float)(v * (1.0 - s));
    q = (float)(v * (1.0 - (s * f)));
    t = (float)(v * (1.0 - (s * (1.0 - f))));

    //Switch statement based on the 'i' value determined above to assign r,g and b values based on the p,q, and t calculations
    switch(i)
    {
      case 0: r=v; g=t; b=p; break;
      case 1: r=q; g=v; b=p; break;
      case 2: r=p; g=v; b=t; break;
      case 3: r=p; g=q; b=v; break;
      case 4: r=t; g=p; b=v; break;
      case 5: r=v; g=p; b=q; break;
      default: r = g = b = 0; break;
    }
  }
  //Assign the r, g, and b values
  colorRGB->r = (int)(r * 255.0);
  colorRGB->g = (int)(g * 255.0);
  colorRGB->b = (int)(b * 255.0);
}

//Create an empty CRGB variable
CRGB fire1;

//The origin of the LED Matrix(0,0) is the bottom left corner
//Neopixel library sets the origin at top of left corner of the Matrix
//This difference accounted for in setPixel() function below
void setPixel(unsigned char x, unsigned char y, unsigned char colorR, unsigned char colorG, unsigned char colorB) {
  //Shift origin and adjust white balance (add more green to show yellow)
  fire1 = CRGB(colorR/reduce, 5*colorG/reduce, colorB/reduce);
  strip->drawPixel(x, (HEIGHT - y - 1), fire1);
}


//Randomly generate the next line of fire for the LED Matrix
void generateLine() {
  for(unsigned char x=0;x<HEIGHT;x++) {
    line[x] = random(64, 255);
  }
}


//Shift all values in the matrixFire up one row
void shiftUp() {
  ColorRGB colorRGB;
  
  for (unsigned char y=WIDTH-1;y>0;y--) {
    for(unsigned char x=0;x<HEIGHT;x++) {
        matrixFire[x][y] = matrixFire[x][y-1];
    }
  }
  
  for(unsigned char x=0;x<HEIGHT;x++) {
      matrixFire[x][0] = line[x];
  }
}


//Draw a frame, interpolating between 2 "key frames"
//@param pcnt percentage of interpolation
void drawFrame(int pcnt) {
  ColorRGB colorRGB;
  ColorHSV colorHSV;
  int nextv;
  
  //Each row interpolates with the one before it
  for (unsigned char y=WIDTH-1;y>0;y--) {
    for (unsigned char x=0;x<HEIGHT;x++) {
      colorHSV.h = hueMask[y][x];
      colorHSV.s = 255;
      nextv = 
          (((100.0-pcnt)*matrixFire[x][y] 
        + pcnt*matrixFire[x][y-1])/100.0) 
        - valueMask[y][x];
      colorHSV.v = (unsigned char)max(0, nextv);
      
      HSVtoRGB(&colorRGB, &colorHSV);
      setPixel(x, y, colorRGB.r, colorRGB.g, colorRGB.b);
    }
  }
  
  //First row interpolates with the "next" line
  for(unsigned char x=0;x<HEIGHT;x++) {
    colorHSV.h = hueMask[0][x];
    colorHSV.s = 255;
    colorHSV.v = (unsigned char)(((100.0-pcnt)*matrixFire[x][0] + pcnt*line[x])/100.0);
    
    HSVtoRGB(&colorRGB, &colorHSV);
    setPixel(x, 0, colorRGB.r, colorRGB.g, colorRGB.b);
  }
  strip->show();    //Display the LEDs
}
////////////////////////////////////////////////////////////////////////////////////////////////




//Random Color Generator
////////////////////////////////////////////////////////////////////////////////////////////////
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
CRGB Wheel(byte WheelPos) {
  if(WheelPos < 85) {
    return CRGB(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if(WheelPos < 170) {
    WheelPos -= 85;
    return CRGB(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
    WheelPos -= 170;
    return CRGB(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}


//Create a function call that matches the format of the other functions present
CRGB randomColor() {
  return Wheel(random(256)); 
}
////////////////////////////////////////////////////////////////////////////////////////////////
