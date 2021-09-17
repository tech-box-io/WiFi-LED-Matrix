//Start by including the required header for this file
#include "Effect.h"

//Define the following variables
#define SPRITE_WIDTH 8        //Sprite Width which is equal to LED Matrix width
#define SPRITE_HEIGHT 8       //Sprite Height which is equal to LED Matrix height
#define RGB_SPRITE_WIDTH 24   //RGB Sprite Width - 8 pixels wide with depth of 3 for the R(red), G(green), and B(blue) values of each pixel
#define RGB_SPRITE_HEIGHT 8   //RGB Sprite Height which is equal to LED Matrix height

//HEX Value array for the Heart Image
static uint8_t heart[SPRITE_WIDTH * SPRITE_HEIGHT] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
    0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

//HEX Value array for the PacMan Pink Ghost Image
static uint8_t ghostPink[SPRITE_WIDTH * SPRITE_HEIGHT] = {
    0x00, 0x00, 0xE6, 0xE6, 0xE6, 0xE6, 0x00, 0x00,
    0x00, 0xE6, 0xE6, 0xE6, 0xE6, 0xE6, 0xE6, 0x00,
    0x00, 0xE6, 0xE6, 0x00, 0xE6, 0xE6, 0x00, 0xE6,
    0x00, 0xE6, 0xE6, 0xB5, 0xE6, 0xE6, 0xB5, 0xE6,
    0xE6, 0xE6, 0xE6, 0xE6, 0xE6, 0xE6, 0xE6, 0xE6,
    0xE6, 0xE6, 0xE6, 0xE6, 0xE6, 0xE6, 0xE6, 0xE6,
    0xE6, 0xE6, 0xE6, 0xE6, 0xE6, 0xE6, 0xE6, 0xE6,
    0xE6, 0xE6, 0x00, 0xE6, 0xE6, 0x00, 0xE6, 0xE6
};

//HEX Value array for the PacMan Purple Ghost Image
static uint8_t ghostPurple[SPRITE_WIDTH * SPRITE_HEIGHT] = {
    0x00, 0x00, 0xD6, 0xD6, 0xD6, 0xD6, 0x00, 0x00,
    0x00, 0xD6, 0xD6, 0xD6, 0xD6, 0xD6, 0xD6, 0x00,
    0x00, 0xD6, 0xD6, 0x00, 0xD6, 0xD6, 0x00, 0xD6,
    0x00, 0xD6, 0xD6, 0xB5, 0xD6, 0xD6, 0xB5, 0xD6,
    0xD6, 0xD6, 0xD6, 0xD6, 0xD6, 0xD6, 0xD6, 0xD6,
    0xD6, 0xD6, 0xD6, 0xD6, 0xD6, 0xD6, 0xD6, 0xD6,
    0xD6, 0xD6, 0xD6, 0xD6, 0xD6, 0xD6, 0xD6, 0xD6,
    0xD6, 0xD6, 0x00, 0xD6, 0xD6, 0x00, 0xD6, 0xD6
};

//HEX Value array for the PacMan Green Ghost Image
static uint8_t ghostGreen[SPRITE_WIDTH * SPRITE_HEIGHT] = {
    0x00, 0x00, 0x64, 0x64, 0x64, 0x64, 0x00, 0x00,
    0x00, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x00,
    0x00, 0x64, 0x64, 0x00, 0x64, 0x64, 0x00, 0x64,
    0x00, 0x64, 0x64, 0xB5, 0x64, 0x64, 0xB5, 0x64,
    0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64,
    0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64,
    0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64,
    0x64, 0x64, 0x00, 0x64, 0x64, 0x00, 0x64, 0x64
};

//HEX Value array for the PacMan Blue Ghost Image
static uint8_t ghostBlue[SPRITE_WIDTH * SPRITE_HEIGHT] = {
    0x00, 0x00, 0x7E, 0x7E, 0x7E, 0x7E, 0x00, 0x00,
    0x00, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x00,
    0x00, 0x7E, 0x7E, 0x00, 0x7E, 0x7E, 0x00, 0x7E,
    0x00, 0x7E, 0x7E, 0xB5, 0x7E, 0x7E, 0xB5, 0x7E,
    0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E,
    0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E,
    0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E,
    0x7E, 0x7E, 0x00, 0x7E, 0x7E, 0x00, 0x7E, 0x7E
};

//HEX Value array for the PacMan Image
static uint8_t pacMan[SPRITE_WIDTH * SPRITE_HEIGHT] = {
    0x00, 0x00, 0x2A, 0x2A, 0x2A, 0x2A, 0x00, 0x00,
    0x00, 0x2A, 0x2A, 0x00, 0x2A, 0x2A, 0x2A, 0x00,
    0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
    0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x00, 0x00, 0x00,
    0x2A, 0x2A, 0x2A, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x00, 0x00, 0x00,
    0x00, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x00,
    0x00, 0x00, 0x2A, 0x2A, 0x2A, 0x2A, 0x00, 0X00
};

//HEX Value array for the Charged Creeper Image
static uint8_t chargedCreeper[SPRITE_WIDTH * SPRITE_HEIGHT] = {
    0xA4, 0x87, 0xA4, 0xA4, 0xA4, 0xA4, 0xA4, 0x87,
    0x87, 0xA4, 0xA4, 0xA4, 0x87, 0xA4, 0xA4, 0xA4,
    0xA4, 0x00, 0x00, 0xA4, 0x87, 0x00, 0x00, 0xA4,
    0xA4, 0x00, 0x00, 0xA4, 0xA4, 0x00, 0x00, 0xA4,
    0xA4, 0xA4, 0xA4, 0x00, 0x00, 0xA4, 0xA4, 0xA4,
    0xA4, 0x87, 0x00, 0x00, 0x00, 0x00, 0xA4, 0xA4,
    0xA4, 0x87, 0x00, 0xA4, 0xA4, 0x00, 0xA4, 0x87,
    0xA4, 0xA4, 0x00, 0xA4, 0xA4, 0x00, 0x87, 0xA4
};

//RGB Value array for the Mario Image
static uint8_t mario[RGB_SPRITE_WIDTH * RGB_SPRITE_HEIGHT] = {   
        255, 255, 255,    249, 56, 1,       249, 56, 1,       249, 56, 1,       249, 56, 1,       249, 56, 1,       255, 255, 255,    255, 255, 255,    
        249, 56, 1,       249, 56, 1,       249, 56, 1,       249, 56, 1,       249, 56, 1,       249, 56, 1,       249, 56, 1,       249, 56, 1,
        50, 30, 1,        50, 30, 1,        50, 30, 1,        252, 161, 68,     50, 30, 1,        252, 161, 68,     252, 161, 68,     255, 255, 255,
        50, 30, 1,        252, 161, 68,     50, 30, 1,        252, 161, 68,     252, 161, 68,     50, 30, 1,        252, 161, 68,     252, 161, 68,
        255, 255, 255,    252, 161, 68,     252, 161, 68,     252, 161, 68,     50, 30, 1,        50, 30, 1,        50, 30, 1,        255, 255, 255,
        255, 255, 255,    50, 30, 1,        249, 56, 1,       50, 30, 1,        249, 56, 1,       50, 30, 1,        255, 255, 255,    255, 255, 255,
        252, 161, 68,     249, 56, 1,       252, 161, 68,     249, 56, 1,       252, 161, 68,     249, 56, 1,       252, 161, 68,     255, 255, 255,
        255, 255, 255,    249, 56, 1,       249, 56, 1,       255, 255, 255,    249, 56, 1,       249, 56, 1,       255, 255, 255,    255, 255, 255      
};

//RGB Value array for the Mario Shy Guy Image
static uint8_t marioShyGuy[RGB_SPRITE_WIDTH * RGB_SPRITE_HEIGHT] = {
    0xF9, 0x38, 0x01,   0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,   0xF9, 0x38, 0x01,   0xF9, 0x38, 0x01,   0xF9, 0x38, 0x01,   0x00, 0x00, 0x00,
    0xFF, 0xFF, 0xFF,   0x00, 0x00, 0x00,   0xFF, 0xFF, 0xFF,   0x00, 0x00, 0x00,   0xFF, 0xFF, 0xFF,   0xF9, 0x38, 0x01,   0xF9, 0x38, 0x01,   0x00, 0x00, 0x00,
    0xFF, 0xFF, 0xFF,   0x00, 0x00, 0x00,   0xFF, 0xFF, 0xFF,   0x00, 0x00, 0x00,   0xFF, 0xFF, 0xFF,   0xF9, 0x38, 0x01,   0xF9, 0x38, 0x01,   0xF9, 0x38, 0x01,
    0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,
    0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,   0x00, 0x00, 0x00,   0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,   0xF9, 0x38, 0x01,   0xF9, 0x38, 0x01,   0xF9, 0x38, 0x01,
    0xF9, 0x38, 0x01,   0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,   0xF9, 0x38, 0x01,   0xF9, 0x38, 0x01,   0x00, 0x00, 0x00,   0xF9, 0x38, 0x01,
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0xF9, 0xB7, 0x01,   0xF9, 0xB7, 0x01,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,
    0xF9, 0x38, 0x01,   0xF9, 0x38, 0x01,   0xF9, 0x38, 0x01,   0xF9, 0x38, 0x01,   0xF9, 0x38, 0x01,   0xF9, 0x38, 0x01,   0xF9, 0x38, 0x01,   0x00, 0x00, 0x00
};

//RGB Value Array for the Kirby Image
static uint8_t kirby[RGB_SPRITE_WIDTH * RGB_SPRITE_HEIGHT] = {
    0x44, 0xA5, 0xF5,   0x44, 0xA5, 0xF5,   0x44, 0xA5, 0xF5,   0x44, 0xA5, 0xF5,   0x44, 0xA5, 0xF5,   0x44, 0xA5, 0xF5,   0x44, 0xA5, 0xF5,   0x44, 0xA5, 0xF5,
    0x44, 0xA5, 0xF5,   0x44, 0xA5, 0xF5,   0xF8, 0xBB, 0xD0,   0xF8, 0xBB, 0xD0,   0xF8, 0xBB, 0xD0,   0xF8, 0xBB, 0xD0,   0x44, 0xA5, 0xF5,   0x44, 0xA5, 0xF5,
    0x44, 0xA5, 0xF5,   0xF8, 0xBB, 0xD0,   0xF8, 0xBB, 0xD0,   0xF8, 0xBB, 0xD0,   0xF8, 0xBB, 0xD0,   0xF8, 0xBB, 0xD0,   0xF8, 0xBB, 0xD0,   0x44, 0xA5, 0xF5,
    0x44, 0xA5, 0xF5,   0xF4, 0x8F, 0xB1,   0xF8, 0xBB, 0xD0,   0x00, 0x00, 0x00,   0xF8, 0xBB, 0xD0,   0x00, 0x00, 0x00,   0xF8, 0xBB, 0xD0,   0x44, 0xA5, 0xF5,
    0xF4, 0x8F, 0xB1,   0xF8, 0xBB, 0xD0,   0xF8, 0xBB, 0xD0,   0x1A, 0x23, 0x7E,   0xF8, 0xBB, 0xD0,   0x1A, 0x23, 0x7E,   0xF8, 0xBB, 0xD0,   0xF4, 0x8F, 0xB1,
    0xF4, 0x8F, 0xB1,   0xF4, 0x8F, 0xB1,   0xF0, 0x62, 0x92,   0xF8, 0xBB, 0xD0,   0xF8, 0xBB, 0xD0,   0xF8, 0xBB, 0xD0,   0xF0, 0x62, 0x92,   0xF4, 0x8F, 0xB1,
    0x44, 0xA5, 0xF5,   0xAD, 0x14, 0x57,   0xF4, 0x8F, 0xB1,   0xF4, 0x8F, 0xB1,   0xF8, 0xBB, 0xD0,   0xF8, 0xBB, 0xD0,   0xAD, 0x14, 0x57,   0x44, 0xA5, 0xF5,
    0x00, 0xFF, 0x00,   0xAD, 0x14, 0x57,   0xC2, 0x18, 0x5B,   0xC2, 0x18, 0x5B,   0x00, 0xFF, 0x00,   0xC2, 0x18, 0x5B,   0xAD, 0x14, 0x57,   0x00, 0xFF, 0x00
};

//RGB Value Array for the Adventure Time Image
static uint8_t adventureTime[RGB_SPRITE_WIDTH * RGB_SPRITE_HEIGHT] = {
    0x81, 0xD4, 0xFA,   0x81, 0xD4, 0xFA,   0xFF, 0xFF, 0xFF,   0x81, 0xD4, 0xFA,   0x81, 0xD4, 0xFA,   0x81, 0xD4, 0xFA,   0xFF, 0xFF, 0xFF,   0x81, 0xD4, 0xFA,
    0x81, 0xD4, 0xFA,   0x81, 0xD4, 0xFA,   0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,   0x81, 0xD4, 0xFA,
    0x81, 0xD4, 0xFA,   0x81, 0xD4, 0xFA,   0xFF, 0xFF, 0xFF,   0x00, 0x00, 0x00,   0xFF, 0xE0, 0xB2,   0xFF, 0xE0, 0xB2,   0x00, 0x00, 0x00,   0x81, 0xD4, 0xFA,
    0x8B, 0xC3, 0x4A,   0x7C, 0xB3, 0x42,   0xFF, 0xFF, 0xFF,   0xFF, 0xE0, 0xB2,   0xFF, 0xE0, 0xB2,   0xFF, 0xE0, 0xB2,   0xFF, 0xE0, 0xB2,   0x81, 0xD4, 0xFA,
    0x1B, 0x5E, 0x20,   0xAE, 0xD5, 0x81,   0x81, 0xD4, 0xFA,   0x21, 0x96, 0xF3,   0x21, 0x96, 0xF3,   0x21, 0x96, 0xF3,   0x21, 0x96, 0xF3,   0x81, 0xD4, 0xFA,
    0xFF, 0xE0, 0xB2,   0x1B, 0x5E, 0x20,   0x21, 0x96, 0xF3,   0x21, 0x96, 0xF3,   0x21, 0x96, 0xF3,   0x21, 0x96, 0xF3,   0x21, 0x96, 0xF3,   0xFF, 0xE0, 0xB2,
    0x00, 0xFF, 0x00,   0x00, 0xFF, 0x00,   0x00, 0x00, 0xFF,   0x00, 0x00, 0xFF,   0x00, 0x00, 0xFF,   0x00, 0x00, 0xFF,   0x00, 0x00, 0xFF,   0x00, 0xFF, 0x00,
    0x00, 0xFF, 0x00,   0x00, 0xFF, 0x00,   0xFF, 0xE0, 0xB2,   0x00, 0xFF, 0x00,   0x00, 0xFF, 0x00,   0x00, 0xFF, 0x00,   0xFF, 0xE0, 0xB2,   0x00, 0xFF, 0x00
};

//RGB Value Array for the Creeper Image
static uint8_t creeper[RGB_SPRITE_WIDTH * RGB_SPRITE_HEIGHT] = {
    0x38, 0xFE, 0x3C,   0x00, 0xFF, 0x00,   0x4B, 0x8E, 0x4A,   0x00, 0xFF, 0x00,   0x00, 0xFF, 0x00,   0x00, 0xFF, 0x00,   0x00, 0xFF, 0x00,   0x00, 0xFF, 0x00,
    0x00, 0xFF, 0x00,   0x00, 0xFF, 0x00,   0x00, 0xFF, 0x00,   0x00, 0xFF, 0x00,   0x38, 0xFE, 0x3C,   0x4B, 0x8E, 0x4A,   0x4B, 0x8E, 0x4A,   0x00, 0xFF, 0x00,
    0x4B, 0x8E, 0x4A,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0xFF, 0x00,   0x00, 0xFF, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x4B, 0x8E, 0x4A,
    0x00, 0xFF, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x4B, 0x8E, 0x4A,   0x00, 0xFF, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0xFF, 0x00,
    0x00, 0xFF, 0x00,   0x38, 0xFE, 0x3C,   0x00, 0xFF, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x38, 0xFE, 0x3C,   0x00, 0xFF, 0x00,   0x00, 0xFF, 0x00,
    0x4B, 0x8E, 0x4A,   0x00, 0xFF, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x4B, 0x8E, 0x4A,   0x38, 0xFE, 0x3C,
    0x00, 0xFF, 0x00,   0x00, 0xFF, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0xFF, 0x00,   0x00, 0xFF, 0x00,
    0x00, 0xFF, 0x00,   0x4B, 0x8E, 0x4A,   0x00, 0x00, 0x00,   0x38, 0xFE, 0x3C,   0x00, 0xFF, 0x00,   0x00, 0x00, 0x00,   0x38, 0xFE, 0x3C,   0x4B, 0x8E, 0x4A
};

//RGB Value Array for the Yoshi Image
static uint8_t yoshi[RGB_SPRITE_WIDTH * RGB_SPRITE_HEIGHT] = {
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0xFF, 0x57, 0x22,   0x7C, 0xB3, 0x42,   0x68, 0x9F, 0x38,   0x7C, 0xB3, 0x42,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0xFF, 0x57, 0x22,   0x7C, 0xB3, 0x42,   0x00, 0x00, 0x00,   0x7C, 0xB3, 0x42,   0x7C, 0xB3, 0x42,   0x7C, 0xB3, 0x42,   0x7C, 0xB3, 0x42,
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,   0x7C, 0xB3, 0x42,   0x7C, 0xB3, 0x42,   0x7C, 0xB3, 0x42,   0x7C, 0xB3, 0x42,
    0x00, 0x00, 0x00,   0xFF, 0x57, 0x22,   0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,   0x7C, 0xB3, 0x42,   0x7C, 0xB3, 0x42,   0x7C, 0xB3, 0x42,   0x7C, 0xB3, 0x42,
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0xF4, 0x43, 0x36,   0x64, 0x64, 0x64,   0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,   0x00, 0x00, 0x00,
    0x68, 0x9F, 0x38,   0xFF, 0x57, 0x22,   0x68, 0x9F, 0x38,   0x68, 0x9F, 0x38,   0x64, 0x64, 0x64,   0x64, 0x64, 0x64,   0x68, 0x9F, 0x38,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0x68, 0x9F, 0x38,   0x7C, 0xB3, 0x42,   0x68, 0x9F, 0x38,   0x64, 0x64, 0x64,   0x64, 0x64, 0x64,   0x68, 0x9F, 0x38,   0x68, 0x9F, 0x38,
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0xB7, 0x1C, 0x1C,   0xB7, 0x1C, 0x1C,   0x00, 0x00, 0x00,   0x88, 0x0E, 0x4F,   0x88, 0x0E, 0x4F,   0x00, 0x00, 0x00
};

//RGB Value Array for the Blue Potion Image
static uint8_t potionBlue[RGB_SPRITE_WIDTH * RGB_SPRITE_HEIGHT] = {
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x96, 0x96, 0x96,   0x96, 0x96, 0x96,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0xFF, 0xFF, 0xFF,   0x21, 0x21, 0x21,   0x21, 0x21, 0x21,   0xFF, 0xFF, 0xFF,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0xFF, 0xFF, 0xFF,   0x29, 0xAD, 0xFF,   0x29, 0xAD, 0xFF,   0x29, 0xAD, 0xFF,   0x29, 0xAD, 0xFF,   0xFF, 0xFF, 0xFF,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0xFF, 0xFF, 0xFF,   0x29, 0xAD, 0xFF,   0x29, 0xAD, 0xFF,   0x29, 0xAD, 0xFF,   0x29, 0xAD, 0xFF,   0xFF, 0xFF, 0xFF,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0x96, 0x96, 0x96,   0x29, 0xAD, 0xFF,   0x29, 0xAD, 0xFF,   0x29, 0xAD, 0xFF,   0x29, 0xAD, 0xFF,   0x96, 0x96, 0x96,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x96, 0x96, 0x96,   0x96, 0x96, 0x96,   0x96, 0x96, 0x96,   0x96, 0x96, 0x96,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00
};

//RGB Value Array for the Green Potion Image
static uint8_t potionGreen[RGB_SPRITE_WIDTH * RGB_SPRITE_HEIGHT] = {
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x96, 0x96, 0x96,   0x96, 0x96, 0x96,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0xFF, 0xFF, 0xFF,   0x21, 0x21, 0x21,   0x21, 0x21, 0x21,   0xFF, 0xFF, 0xFF,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0xFF, 0xFF, 0xFF,   0x00, 0xE4, 0x36,   0x00, 0xE4, 0x36,   0x00, 0xE4, 0x36,   0x00, 0xE4, 0x36,   0xFF, 0xFF, 0xFF,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0xFF, 0xFF, 0xFF,   0x00, 0xE4, 0x36,   0x00, 0xE4, 0x36,   0x00, 0xE4, 0x36,   0x00, 0xE4, 0x36,   0xFF, 0xFF, 0xFF,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0x96, 0x96, 0x96,   0x00, 0xE4, 0x36,   0x00, 0xE4, 0x36,   0x00, 0xE4, 0x36,   0x00, 0xE4, 0x36,   0x96, 0x96, 0x96,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x96, 0x96, 0x96,   0x96, 0x96, 0x96,   0x96, 0x96, 0x96,   0x96, 0x96, 0x96,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00
};

//RGB Value Array for the Orange Potion Image
static uint8_t potionOrange[RGB_SPRITE_WIDTH * RGB_SPRITE_HEIGHT] = {
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x96, 0x96, 0x96,   0x96, 0x96, 0x96,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0xFF, 0xFF, 0xFF,   0x21, 0x21, 0x21,   0x21, 0x21, 0x21,   0xFF, 0xFF, 0xFF,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0xFF, 0xFF, 0xFF,   0xFF, 0xA3, 0x00,   0xFF, 0xA3, 0x00,   0xFF, 0xA3, 0x00,   0xFF, 0xA3, 0x00,   0xFF, 0xFF, 0xFF,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0xFF, 0xFF, 0xFF,   0xFF, 0xA3, 0x00,   0xFF, 0xA3, 0x00,   0xFF, 0xA3, 0x00,   0xFF, 0xA3, 0x00,   0xFF, 0xFF, 0xFF,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0x96, 0x96, 0x96,   0xFF, 0xA3, 0x00,   0xFF, 0xA3, 0x00,   0xFF, 0xA3, 0x00,   0xFF, 0xA3, 0x00,   0x96, 0x96, 0x96,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x96, 0x96, 0x96,   0x96, 0x96, 0x96,   0x96, 0x96, 0x96,   0x96, 0x96, 0x96,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00
};

//RGB Value Array for the Red Potion Image
static uint8_t potionRed[RGB_SPRITE_WIDTH * RGB_SPRITE_HEIGHT] = {
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x96, 0x96, 0x96,   0x96, 0x96, 0x96,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0xFF, 0xFF, 0xFF,   0xFF, 0xFF, 0xFF,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0xFF, 0xFF, 0xFF,   0x21, 0x21, 0x21,   0x21, 0x21, 0x21,   0xFF, 0xFF, 0xFF,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0xFF, 0xFF, 0xFF,   0xFF, 0x00, 0x4D,   0xFF, 0x00, 0x4D,   0xFF, 0x00, 0x4D,   0xFF, 0x00, 0x4D,   0xFF, 0xFF, 0xFF,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0xFF, 0xFF, 0xFF,   0xFF, 0x00, 0x4D,   0xFF, 0x00, 0x4D,   0xFF, 0x00, 0x4D,   0xFF, 0x00, 0x4D,   0xFF, 0xFF, 0xFF,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0x96, 0x96, 0x96,   0xFF, 0x00, 0x4D,   0xFF, 0x00, 0x4D,   0xFF, 0x00, 0x4D,   0xFF, 0x00, 0x4D,   0x96, 0x96, 0x96,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x96, 0x96, 0x96,   0x96, 0x96, 0x96,   0x96, 0x96, 0x96,   0x96, 0x96, 0x96,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00
};

//RGB Value Array for the Pikachu Image
static uint8_t pikachu[RGB_SPRITE_WIDTH * RGB_SPRITE_HEIGHT] = {
    0x00, 0x00, 0x00,   0x21, 0x21, 0x21,   0x21, 0x21, 0x21,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x21, 0x21, 0x21,
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0xFF, 0xEB, 0x3B,   0xFF, 0x98, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0xFF, 0x98, 0x00,
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0xFF, 0xF1, 0x00,   0xFF, 0xF1, 0x00,   0xFF, 0xF1, 0x00,   0xFF, 0xF1, 0x00,   0xFF, 0x98, 0x00,
    0xFF, 0x96, 0x00,   0xFF, 0x96, 0x00,   0x00, 0x00, 0x00,   0xFF, 0xF1, 0x00,   0x00, 0x00, 0x00,   0xFF, 0xEB, 0x3B,   0xFF, 0xEB, 0x3B,   0x00, 0x00, 0x00,
    0xFF, 0x96, 0x00,   0xFF, 0x96, 0x00,   0x00, 0x00, 0x00,   0xFF, 0x21, 0x04,   0xFF, 0xEB, 0x3B,   0xFF, 0xEB, 0x3B,   0xFF, 0xEB, 0x3B,   0xFF, 0x98, 0x00,
    0x00, 0x00, 0x00,   0xC7, 0x6D, 0x00,   0x00, 0x00, 0x00,   0xFF, 0xF1, 0x00,   0xFF, 0x98, 0x00,   0xFF, 0x98, 0x00,   0xFF, 0x98, 0x00,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0xC7, 0x6D, 0x00,   0xFF, 0xF1, 0x00,   0xFF, 0x96, 0x00,   0xFF, 0xF1, 0x00,   0xFF, 0x98, 0x00,   0xFF, 0xF1, 0x00,   0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,   0xFF, 0xF1, 0x00,   0xFF, 0x96, 0x00,   0xC7, 0x6D, 0x00,   0xC7, 0x6D, 0x00,   0xFF, 0x96, 0x00,   0x00, 0x00, 0x00
};

//Empty RGB Frame for you to customize! Just remove the comments and add RGB values for your desired color
//static uint8_t emptyRGBFrame[RGB_SPRITE_WIDTH * RGB_SPRITE_HEIGHT] = {
//    0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,
//    0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,
//    0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,
//    0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,
//    0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,
//    0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,
//    0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,
//    0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x,   0x, 0x, 0x
//};

//Empty HSV Frame for you to customize! Just remove the comments and add HEX values for your desired color
//static uint8_t emptyHSVFrame[SPRITE_WIDTH * SPRITE_HEIGHT] = {
//    0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x,
//    0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x,
//    0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x,
//    0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x,
//    0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x,
//    0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x,
//    0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x,
//    0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x
//};

//Create the Sprite Object
class Sprite : public Effect {

private:
  int spriteWidth, spriteHeight;  //Create private variables for the sprite width and height
    

public:
  //Create the Sprite constructor with the appropriate variables
  Sprite(CRGB *leds, int width, int height) : Effect(leds, width, height), spriteWidth(SPRITE_WIDTH), spriteHeight(SPRITE_HEIGHT) {}

  //Create the 'start' public function
  void start(int imgNum, int spriteInc) {
    //If the image number from the webpage is set to 0, then rotate through the sprite images based on the sprite increment value
    if(imgNum == 0) {
      if(spriteInc == 0) { blitRGB(kirby); }
      else if(spriteInc == 1) { blitRGB(creeper); }
      else if(spriteInc == 2) { blitRGB(pikachu); }
      else if(spriteInc == 3) { blitHSV(ghostPink); }
      else if(spriteInc == 4) { blitHSV(heart); }
      else if(spriteInc == 5) { blitRGB(potionOrange); }
      else if(spriteInc == 6) { blitHSV(pacMan); }
      else if(spriteInc == 7) { blitRGB(potionRed); }
      else if(spriteInc == 8) { blitHSV(chargedCreeper); }
      else if(spriteInc == 9) { blitHSV(ghostBlue); }
      else if(spriteInc == 10) { blitRGB(mario); }
      else if(spriteInc == 11) { blitRGB(potionBlue); }
      else if(spriteInc == 12) { blitRGB(marioShyGuy); }
      else if(spriteInc == 13) { blitHSV(ghostPurple); }
      else if(spriteInc == 14) { blitRGB(adventureTime); }
      else if(spriteInc == 15) { blitRGB(potionGreen); }
      else if(spriteInc == 16) { blitRGB(yoshi); }
      else if(spriteInc == 17) { blitHSV(ghostGreen); }
    }
    //Else, if the image number from the webpage is not 0, then display the selected sprite
    else if(imgNum == 1) { blitHSV(heart); }
    else if(imgNum == 2) { blitHSV(ghostPink); }
    else if(imgNum == 3) { blitHSV(ghostPurple); }
    else if(imgNum == 4) { blitHSV(ghostGreen); }
    else if(imgNum == 5) { blitHSV(ghostBlue); }
    else if(imgNum == 6) { blitHSV(pacMan); }
    else if(imgNum == 7) { blitHSV(chargedCreeper); }
    else if(imgNum == 8) { blitRGB(mario); }
    else if(imgNum == 9) { blitRGB(marioShyGuy); }
    else if(imgNum == 10) { blitRGB(kirby); }
    else if(imgNum == 11){ blitRGB(adventureTime); }
    else if(imgNum == 12){ blitRGB(creeper); }
    else if(imgNum == 13){ blitRGB(yoshi); }
    else if(imgNum == 14){ blitRGB(potionBlue); }
    else if(imgNum == 15){ blitRGB(potionGreen); }
    else if(imgNum == 16){ blitRGB(potionOrange); }
    else if(imgNum == 17){ blitRGB(potionRed); }
    else if(imgNum == 18){ blitRGB(pikachu); }
    else {
      FastLED.clear();    //Clear LED array
      FastLED.show();     //Show the cleared array to clear the LED Matrix
    }
  }

  //Create the 'blitHSV' public function to display the chosen HSV sprite
  void blitHSV(uint8_t *sprite) {
    FastLED.clear();      //Clear the LED array
    uint8_t saturation = random(8, 16) * 16;    
    //For each pixel in the sprite, assign the HSV value to the leds array
    for (int spx = 0; spx < spriteWidth; spx++) { 
      for (int spy = 0; spy < spriteHeight; spy++) {
        if(sprite[spy + (8 * spx)] > 0) {
          leds[spy + (8 * spx)] = CHSV(sprite[spy + (8 * spx)], 255, 200);
        }
      }
    }
    LEDS.show();    //Show the LED Array
  }

  //Create the 'blitRGB' public function to display the chosen RGB sprite
  void blitRGB(uint8_t *sprite) {
    FastLED.clear();    //Clear the LED array
    //For each pixel in the sprite, assign the RGB value to the leds array
    for (int spx = 0; spx < spriteWidth; spx++) {
      for (int spy = 0; spy < spriteHeight; spy++) {
        leds[spy + (8 * spx)] = CRGB(sprite[(spy * 3) + (24 * spx)], sprite[(spy * 3) + 1 + (24 * spx)], sprite[(spy * 3) + 2 + (24 * spx)]);
      }
    }
    LEDS.show();    //Show the LED Array
  }
};