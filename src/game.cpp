#include "../include/game.hpp"

void Game::start(void) {
  tft_.fillScreen(TFT_BLACK); // Getting rid of numbers and words being displyed on the screen
  tft_.setTextColor(TFT_WHITE);

  tft_.drawString("Game Start!", X_CENTER-50, Y_CENTER, 4);
  delay(1000);
  tft_.fillScreen(TFT_BLACK);
  tft_.setSwapBytes(true);

  draw_map(); 
  
  create_tank(X_CENTER, Y_CENTER, 100, 5, 5); // creating a tank in the center of the screen with 100 health and 5 ammunition
  create_tank(X_CENTER+100, Y_CENTER+50, 100, 5, 5); // creating a tank in the center of the screen with 100 health and 5 ammunition

  while (true) { //main loop
    check_updates_buttons();
    execute_updates();

    delay(50);
  } 
}

void Game::check_updates_buttons(void) {
  for (size_t i = 0; i < BTN_COUNT; i++) {
    buttons_[i].update();
  }
}

void Game::execute_updates() {
  if (tanks_.size() == 0) return; // if there are no tanks, do nothing

  for (const auto& bullet: bullets_) {
    bullet->move(bullet->get_dx(), bullet->get_dy());   
    bullet->draw();
  }

  if (buttons_[BTN_X].status_) create_flying_bullet();
  int dx = 0, dy = 0;
  int speed = tanks_[0]->get_speed();

  if (buttons_[BTN_UP].status_)    dy = -speed;
  else if (buttons_[BTN_DOWN].status_)  dy = speed;
  else if (buttons_[BTN_LEFT].status_)  dx = -speed;
  else if (buttons_[BTN_RIGHT].status_) dx = speed;

  if (dx != 0 || dy != 0) {
    if (!process_collisions(dx, dy)) {
      tanks_[0]->move(dx, dy);
      tanks_[0]->update_orientation(dx, dy);    
      tanks_[0]->draw();
    }
  }
}

bool Game::process_collisions(int dx, int dy) {
  int next_x = tanks_[0]->getX() + dx;
  int next_y = tanks_[0]->getY() + dy;

  bool collision = false;

  if (next_x < 0 || next_x + tanks_[0]->getWidth()  > X_MAX ||
      next_y < 0 || next_y + tanks_[0]->getHeight() > Y_MAX) collision = true;

  
  // if (!collision && map.is_blocked(next_x, next_y, tank->get_width(), tank->get_height())) collision = true;

  if (!collision) {
    for (auto& other : tanks_) {
      if (other == tanks_[0]) continue;
        if (tanks_[0]->collidesWith(dx, dy, other.get())) {
          collision = true;
          break;
        }
    }
  }
  return collision;
}

void Game::create_tank(size_t x_pos, size_t y_pos, size_t health, size_t ammunition, size_t speed) {
  auto tank = std::make_unique<Tank>(x_pos, y_pos, health, ammunition, speed,  tft_);
  if (tank->is_valid()) {
    tank->draw(); 
    tanks_.push_back(std::move(tank)); 
  }
}

void Game::delete_tank(size_t index) {
  tanks_.erase(tanks_.begin() + index);
}

void Game::delete_enemy_tanks(void) {
  if (tanks_.size() > 1) {
      tanks_.erase(tanks_.begin() + 1, tanks_.end());
  }
}

void Game::create_flying_bullet() {
  auto it = tanks_[0]->count_nose_of_the_tank(default_bullet_width, default_bullet_length);
  auto bullet = std::make_unique<Bullet>(it.first, it.second, tanks_[0]->getOrientation(), default_bullet_speed, tft_);
  bullets_.push_back(std::move(bullet));
}

void Game::draw_map() {
  for (int i = 0; i < MAP_HEIGHT; i++) {
    for (int j = 0; j < MAP_WIDTH; j++) {
      uint32_t color = 0;
      switch(game_map[i][j]) {
        case BLACK: break;
        case GRASS: color = TFT_OLIVE; break;
        case BRICKS_WALL: color =  TFT_BROWN; break;
        case SPECIAL: color = TFT_MAGENTA; break;
      }

      tft_.drawRect(j*TILE_SIZE, i*TILE_SIZE, TILE_SIZE, TILE_SIZE, color);
    }
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