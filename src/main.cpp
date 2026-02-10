

#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library

#include "../include/game.h"

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

void setup(void) {
  tft.init();
  tft.fillScreen(TFT_BLACK); // sets screen Black
  tft.setTextColor(TFT_WHITE);
  tft.drawString("World of Tanks", X_CENTER, Y_CENTER, 4); //Print string in the center
  tft.setRotation(1);            // Album orientation

  Serial.begin(115200); // setting velocity of communication with pins
  //inMode(BTN_PIN, INPUT_PULLUP);
  
  CountDown(tft);
  game TANKS(tft);
  TANKS.start();
}

void loop() {
  
  while (true) {};
}

