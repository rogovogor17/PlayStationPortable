#include "../include/game.hpp"

void game::start(void) {
  tft_.fillScreen(TFT_BLACK); // Getting rid of numbers and words being displyed on the screen
  tft_.setTextColor(TFT_WHITE);

  tft_.drawString("Game Start!", X_CENTER-50, Y_CENTER, 4);
  delay(1000);
  tft_.fillScreen(TFT_BLACK);
  
  create_tank(X_CENTER, Y_CENTER, 100, 5); // creating a tank in the center of the screen with 100 health and 5 ammunition

  while (true) { //main loop
    
    
    tanks_[0]->move(5, 0); // move the tank to the right
    tanks_[0]->show(); // show the tank on the screen

    delay(50);
  }
  
}

void CountDown(TFT_eSPI& tft) {
    int count_down = COUNT_DOWN;

    while (count_down > 0) {
    tft.drawString(String(count_down), X_CENTER, Y_CENTER + 20, 7);
    delay(1000);
     tft.fillScreen(TFT_BLACK); 
    count_down--;
  }
}