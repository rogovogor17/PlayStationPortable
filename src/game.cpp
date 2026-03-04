#include "../include/game.hpp"

void game::start(void) {
  tft_.fillScreen(TFT_BLACK); // Getting rid of numbers and words being displyed on the screen
  tft_.setTextColor(TFT_WHITE);

  tft_.drawString("Game Start!", X_CENTER-50, Y_CENTER, 4);
  delay(1000);
  tft_.fillScreen(TFT_BLACK);
  
  create_tank(X_CENTER, Y_CENTER, 100, 5, 5); // creating a tank in the center of the screen with 100 health and 5 ammunition

  while (true) { //main loop
    
    //tft_.pushImage(X_CENTER, Y_CENTER, 40, 40, default_tank_new_up); // redraw the background to erase the previous position of the tank
    check_updates_buttons();
    execute_updates();

    // tanks_[0]->move(5, 0); // move the tank to the right
    // tanks_[0]->show(); // show the tank on the screen

    delay(50);
  } 
}

void game::check_updates_buttons(void) {
  for (size_t i = 0; i < BTN_COUNT; i++) {
    buttons_[i].update();
  }
}

void game::execute_updates() {
  if (tanks_.size() == 0) return; // if there are no tanks, do nothing

  int dx = 0, dy = 0;
  int speed = tanks_[0]->get_speed();

  if (buttons_[BTN_UP].status_)    dy = -speed;
  else if (buttons_[BTN_DOWN].status_)  dy = speed;
  else if (buttons_[BTN_LEFT].status_)  dx = -speed;
  else if (buttons_[BTN_RIGHT].status_) dx = speed;

  if (dx != 0 || dy != 0) {
    tanks_[0]->move(dx, dy);
  }

  tanks_[0]->update_orientation(dx, dy);    

  tanks_[0]->draw();
}

void game::create_tank(size_t x_pos, size_t y_pos, size_t health, size_t ammunition, size_t speed) {
  auto tank = std::make_unique<Tank_d>(x_pos, y_pos, health, ammunition, speed,  tft_);
  if (tank->is_valid()) {
    tank->draw(); 
    tanks_.push_back(std::move(tank)); 
  }
}

void game::delete_tank(size_t index) {
  tanks_.erase(tanks_.begin() + index);
}

void game::delete_enemy_tanks(void) {
  if (tanks_.size() > 1) {
      tanks_.erase(tanks_.begin() + 1, tanks_.end());
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