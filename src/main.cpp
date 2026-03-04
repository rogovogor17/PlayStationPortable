#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library

#include "../include/game.hpp"

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

void setup(void) {
  delay(1000);
  tft.init();
  tft.fillScreen(TFT_BLACK); // sets screen Black
  tft.setTextColor(TFT_WHITE);
  tft.setRotation(1);            // Album orientation
  tft.setSwapBytes(true);
  tft.drawString("World of Tanks", X_CENTER-70, Y_CENTER, 4); //Print string in the center
  //tft.drawString("Ura", X_CENTER-70, Y_CENTER, 4); //Print string in the center
  delay(1000); // delay for 2 seconds to show the name of the game
  tft.fillScreen(TFT_BLACK);

  Serial.begin(115200); // setting velocity of communication with pins
  
  //CountDown(tft);
  Game TANKS(tft);
  TANKS.start();
}

void loop() {
  
  while (true) {};
}