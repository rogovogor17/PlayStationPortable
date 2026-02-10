#include "../include/game.h"

void game::start(void) {
  tft.fillScreen(TFT_BLACK); // Getting rid of numbers and words being displyed on the screen
  tft.setTextColor(TFT_WHITE);
  tft.drawString("Game Start!", X_CENTER, Y_CENTER, 4);
  //init music somewhere here
  delay(2000);
}

void CountDown(TFT_eSPI& tft) {
    int count_down = COUNT_DOWN;

    while (count_down > 0) {
    tft.drawString(String(count_down), X_CENTER, Y_CENTER + 20, 7);
    delay(1000);
    count_down--;
  }
}