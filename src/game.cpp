#include "../include/game.hpp"

void game::start(void) {
  tft_.fillScreen(TFT_BLACK); // Getting rid of numbers and words being displyed on the screen
  tft_.setTextColor(TFT_WHITE);

  tft_.drawString("Game Start!", X_CENTER-50, Y_CENTER, 4);
  delay(1000);
  tft_.fillScreen(TFT_BLACK);
  
 // v.front();
  create_tank(0, Y_CENTER, 100, 5); // creating a tank in the center of the screen with 100 health and 5 ammunition

  size_t x = 0;
  while (true) { //main loop
    //tft_.pushImage(0, 0, DEFAULT_TANK_WIDTH, DEFAULT_TANK_HEIGHT, default_tank); 
    screen_sprite_->fillSprite(TFT_BLACK); // clear the screen before drawing the tank 
    tanks_[0]->push_to_sprite(*screen_sprite_); // draw the tank sprite on the screen
     
    tanks_[0]->set_position(x, Y_CENTER); // move the tank to the right
    x += 5; // move the tank 5 pixels to the right
    if (x > X_MAX) { // if the tank goes off the screen, reset its position to the left
      x = 0;
    } 

    screen_sprite_->pushSprite(0, 0); 

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