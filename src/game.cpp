#include "../include/game.hpp"

void Game::start(void) {
  tft_.fillScreen(TFT_BLACK); // Getting rid of numbers and words being displyed on the screen
  tft_.setTextColor(TFT_WHITE);

  tft_.drawString("Game Start!", X_CENTER-50, Y_CENTER, 4);
  delay(1000);
  tft_.fillScreen(TFT_BLACK);
  tft_.setSwapBytes(true);

  std::vector<Rect> tank_rects = draw_map(); 
  create_tank(tank_rects.front().x, tank_rects.front().y, 50, 5, 5); // creating a tank in the center of the screen with 50 health and 5 ammunition
  if (tank_rects.size() >= 2) {
    create_bot(tank_rects[1].x, tank_rects[1].y, 30, 5, 3); // creating a bot in the second spawn point with 30 health and 5 ammunition
  }
  
  register_collidables(); 

  bool was_paused = false;
  while (is_running_) { //main loop
    
    check_updates_buttons();
  
    if (buttons_[BTN_PAUSA].status_ && !was_paused) {
        was_paused = true;
        
        if (status_ == GameStatus::IN_PROGRESS) {
          status_ = GameStatus::ON_HOLD;
          draw_pause_screen();
        } else if (status_ == GameStatus::ON_HOLD) {
          status_ = GameStatus::IN_PROGRESS;
          draw_map();
          print_tank_data_to_info_table(*tanks_[0], true);
        }
    } else if (!buttons_[BTN_PAUSA].status_) {
      was_paused = false;  // Сбрасываем флаг, когда кнопка отпущена
    }

    switch(status_) {
      case GameStatus::OVER: { 
        is_running_ = false;
        break;
      }

      case GameStatus::ON_HOLD: {  
        delay(100);  // Небольшая задержка, чтобы не нагружать CPU
        break;
      }

      case GameStatus::IN_PROGRESS: {
        for (auto& tank : tanks_)       tank->update();
        for (auto& bullet : bullets_) bullet->update();

        execute_updates();
        cleanup_dead_objects();

        delay(40);
      }

      default: break;
    }
    
  } 

  tft_.fillScreen(TFT_BLACK);
  tft_.drawString("Game Over", X_CENTER-50, Y_CENTER, 4);
  delay(2000);
}

void Game::check_updates_buttons(void) {
  for (size_t i = 0; i < BTN_COUNT; i++) {
    buttons_[i].update();
  }
}

void Game::execute_updates() {

  std::vector<Rect> dirty_rects;  

  if (buttons_[BTN_X].status_) {
    if (tanks_[0]->canShoot()) {
      tanks_[0]->shoot(); 
      create_flying_bullet(tanks_[0]);
    } 
  }

  for (size_t i = 1; i < tanks_.size(); ++i) {
    // Используем static_pointer_cast вместо dynamic_pointer_cast
    auto bot = std::static_pointer_cast<BotTank>(tanks_[i]);
    if (bot->fired_) {
      bot->fired_ = false;
      create_flying_bullet(tanks_[i]);
    }
  }

  move_player(dirty_rects);
  move_bots(dirty_rects);

  for (auto& tank : tanks_) {
    if (tank->is_exploding()) {
      if (tank->animation_finished()) {
        tank->mark_dead();
      }
      dirty_rects.push_back(tank->get_collision_rect());
      continue;
    }
  }
   
  for (auto& bullet : bullets_) {
    if (bullet->is_exploding()) {
      if (bullet->animation_finished()) {
        bullet->mark_dead();
      }
      dirty_rects.push_back(bullet->get_collision_rect());
      continue;
    }

    Rect rect = bullet->get_collision_rect();
    rect.x += bullet->get_dx();
    rect.y += bullet->get_dy();

    if (is_out_of_bounds(rect)) {
      bullet->mark_exploding();  // Взрыв при выходе за границы
      continue;
    }

    if (collision_mgr_.handle_collisions(bullet, rect)) {
      //all necessary operations are done in collision manager
    } 
    else {
      dirty_rects.push_back(bullet->get_collision_rect());
      bullet->move(bullet->get_dx(), bullet->get_dy());
      dirty_rects.push_back(bullet->get_collision_rect());
    }
  }

  for (const auto& area : dirty_rects) {
    draw_map_part(area);
  }

  for (auto& t : tanks_) {
    if (!t->is_dead()) t->draw();
  } 
  for (auto& b : bullets_) {
    if (!b->is_dead()) b->draw();
  }

  print_tank_data_to_info_table(*tanks_[0]);
}


void Game::register_collidables() {
  for (auto& tank: tanks_) {
    collision_mgr_.register_object(tank);
  }
  
  for (auto& bullet: bullets_) {
    collision_mgr_.register_object(bullet);
  }
}

void Game::create_tank(size_t x_pos, size_t y_pos, size_t health, size_t ammunition, size_t speed) {
  auto tank = std::make_shared<Tank>(x_pos, y_pos, health, ammunition, speed,  tft_);
  if (tank->is_valid()) {
    tank->draw(); 
    tanks_.push_back(std::move(tank)); 
  }
}

void Game::create_bot(size_t x_pos, size_t y_pos, size_t health, size_t ammunition, size_t speed) {
  auto bot = std::make_shared<BotTank>(x_pos, y_pos, health, ammunition, speed,  tft_);
  bot->set_type(BotType::normal); // Устанавливаем тип бота (можно менять на easy или hard)
  bot->set_valid_dir_callback([this](int speed, Rect current_rect) -> std::vector<Direction> {
    return collision_mgr_.get_valid_directions(speed, current_rect);
  });
  if (bot->is_valid()) {
    bot->draw(); 
    tanks_.push_back(std::move(bot)); 
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

void Game::create_flying_bullet(std::shared_ptr<Tank> tank) {
  auto it = tank->count_nose_of_the_tank(default_bullet_width, default_bullet_length);
  auto bullet = std::make_shared<Bullet>(it.first, it.second, tank, default_bullet_speed, tft_);
  collision_mgr_.register_object(bullet);
  bullets_.push_back(std::move(bullet));
}

void Game::draw_pause_screen() {
  tft_.fillRect(X_CENTER-10, Y_CENTER, TILE_SIZE/2, TILE_SIZE*4, TFT_WHITE);
  tft_.fillRect(X_CENTER+10, Y_CENTER, TILE_SIZE/2, TILE_SIZE*4, TFT_WHITE);
}

std::vector<Rect> Game::draw_map() {
  std::vector<Rect> tank_rects = {Rect{0, 0, 0, 0}};
  for (int i = 0; i < MAP_HEIGHT; i++) {
    for (int j = 0; j < MAP_WIDTH; j++) {
      uint32_t color = 0;
      switch(game_map_[i][j]) {
        case BLACK: break;
        case GRASS:       color = TFT_OLIVE;   break;
        case BRICKS_WALL: color = TFT_BROWN;   break;
        case SPECIAL:     color = TFT_MAGENTA; break;
        case BEDROCK:     color = TFT_DARKGREY;break;
        case SPAWN_P1: {
          color = TFT_SILVER;
          Rect tank_rect = {j*TILE_SIZE, i*TILE_SIZE, TILE_SIZE, TILE_SIZE};
          tank_rects[0] = tank_rect;
          break;
        } 

        case SPAWN_POINTS: {
          color = TFT_SILVER;
          Rect tank_rect = {j*TILE_SIZE, i*TILE_SIZE, TILE_SIZE, TILE_SIZE};
          tank_rects.push_back(tank_rect);
          break;
        }
      }

      tft_.fillRect(j*TILE_SIZE, i*TILE_SIZE, TILE_SIZE, TILE_SIZE, color);
    }
  }

  draw_info_table();
  return tank_rects;
}

void Game::draw_info_table() {
  // Полурамочка в правом верхнем углу
  int frame_x = (MAP_WIDTH - 4) * TILE_SIZE;
  int frame_y = 0;
  int frame_width = 4 * TILE_SIZE;
  int frame_height = 6 * TILE_SIZE;  // Уменьшаем до 6 строк (убрали AMMO и HP надписи)

  // Внешняя рамка (объемный эффект)
  tft_.drawRect(frame_x, frame_y, frame_width, frame_height, TFT_WHITE);
  tft_.drawLine(frame_x + 1, frame_y + 1, frame_x + frame_width - 2, frame_y + 1, TFT_LIGHTGREY);
  tft_.drawLine(frame_x + 1, frame_y + 1, frame_x + 1, frame_y + frame_height - 2, TFT_LIGHTGREY);
  
  // Внутренняя рамка
  tft_.drawRect(frame_x + 2, frame_y + 2, frame_width - 4, frame_height - 4, TFT_LIGHTGREY);
  
  // Внутренняя область (черная)
  tft_.fillRect(frame_x + 3, frame_y + 3, frame_width - 6, frame_height - 6, TFT_BLACK);
  
  // Устанавливаем цвет текста
  tft_.setTextColor(TFT_WHITE, TFT_BLACK);
  tft_.setTextSize(1);
  
  tft_.drawString("IP 1", frame_x + 10, frame_y + 10, 2);
  tft_.drawLine(frame_x + 5, frame_y + 25, frame_x + frame_width - 5, frame_y + 25, TFT_WHITE);
  
  // Убираем надписи HP и AMMO, оставляем только X и Y
  // Вместо этого рисуем иконки
  tft_.drawString("X:",    frame_x + 10, frame_y + 78, 1);
  tft_.drawString("Y:",    frame_x + 10, frame_y + 98, 1);
}
void draw_heart(TFT_eSPI& tft, int x, int y, int size = 10) {
  if (size == 10) {
    for (int row = 0; row < 10; row++) {
      for (int col = 0; col < 10; col++) {
        if (heart_10x10[row][col]) {
          tft.drawPixel(x + col, y + row, TFT_RED);
        }
      }
    }
  } else if (size == 8) {
    for (int row = 0; row < 8; row++) {
      for (int col = 0; col < 8; col++) {
        if (heart_8x8[row][col]) {
          tft.drawPixel(x + col, y + row, TFT_RED);
        }
      }
    }
  }
}


void Game::print_tank_data_to_info_table(const Tank& tank, bool force_update) {  
  // Если force_update == true, сбрасываем все статические переменные
  if (force_update) {
    last_health_ = -1;
    last_ammo_ = -1;
    last_x_ = -1;
    last_y_ = -1;
  }
  
  int current_health = tank.get_health();
  int current_ammo   = tank.get_ammunition();
  Rect rect          = tank.get_collision_rect();
  int current_x = rect.x;
  int current_y = rect.y;
  
  int frame_x = (MAP_WIDTH - 4) * TILE_SIZE;
  int frame_y = 0;
  
  // Обновляем HP (сердечки)
  int current_hearts = current_health / 10;
  int last_hearts = (last_health_ == -1) ? -1 : last_health_ / 10;
  
  if (current_hearts != last_hearts) {
    last_health_= current_health;
    
    // Очищаем область для сердечек (теперь на месте бывшей надписи HP)
    tft_.fillRect(frame_x + 10, frame_y + 38, 60, 16, TFT_BLACK);
    
    // Рисуем сердечки в ряд
    int max_hearts = 5; // Максимум 5 сердечек (50 HP)
    for (int i = 0; i < max_hearts; i++) {
      int heart_x = frame_x + 10 + (i * 12);
      if (i < current_hearts) {
        draw_heart(tft_, heart_x, frame_y + 40, 10);
      } else {
        // Рисуем пустое сердечко (контур)
        tft_.drawRect(heart_x, frame_y + 40, 10, 10, TFT_DARKGREY);
      }
    }
  }
  
  // Обновляем Ammo (желтые прямоугольники)
  if (current_ammo != last_ammo_) {
    last_ammo_ = current_ammo;
    
    // Очищаем область для патронов
    tft_.fillRect(frame_x + 10, frame_y + 58, 60, 16, TFT_BLACK);
    
    // Рисуем желтые прямоугольники (патроны)
    int max_ammo = tanks_[0]->get_max_ammunition();
    int ammo_to_show = min(current_ammo, max_ammo);
    
    for (int i = 0; i < max_ammo; i++) {
      int ammo_x = frame_x + 10 + (i * 7);
      if (i < ammo_to_show) {
        tft_.fillRect(ammo_x, frame_y + 60, 5, 10, TFT_YELLOW);
        tft_.fillRect(ammo_x, frame_y + 60, 5, 4,  TFT_BROWN);
        // Добавляем блик для объема
        tft_.drawLine(ammo_x + 1, frame_y + 61, ammo_x + 1, frame_y + 66, TFT_WHITE);
      } else {
        // Рисуем пустой контур патрона
        tft_.drawRect(ammo_x, frame_y + 60, 5, 10, TFT_DARKGREY);
      }
    }
    
    // Если патронов больше чем помещается, показываем "+"
    if (current_ammo > max_ammo) {
      tft_.setTextColor(TFT_YELLOW, TFT_BLACK);
      tft_.drawString("+", frame_x + 10 + (max_ammo * 7), frame_y + 60, 1);
    }
  }
  
  // Обновляем X 
  if (current_x != last_x_) {
    last_x_ = current_x;
    
    tft_.fillRect(frame_x + 30, frame_y + 78, 30, 16, TFT_BLACK);
    tft_.drawString(String(current_x), frame_x + 30, frame_y + 78, 1);
  }
  
  // Обновляем Y
  if (current_y != last_y_) {
    last_y_ = current_y;
    
    tft_.fillRect(frame_x + 30, frame_y + 98, 30, 16, TFT_BLACK);
    tft_.drawString(String(current_y), frame_x + 30, frame_y + 98, 1);
  }
}

void Game::draw_map_part(Rect r) {
  int start_col = r.x / TILE_SIZE;
  int end_col   = (r.x + r.w - 1) / TILE_SIZE;
  int start_row = r.y / TILE_SIZE;
  int end_row   = (r.y + r.h - 1) / TILE_SIZE;

  start_col = std::max(0, start_col);
  end_col   = std::min((int)MAP_WIDTH - 1, end_col);
  start_row = std::max(0, start_row);
  end_row   = std::min((int)MAP_HEIGHT - 1, end_row);

  // Определяем область info table
  int info_x = (MAP_WIDTH - 4) * TILE_SIZE;
  int info_y = 0;
  int info_w = 4 * TILE_SIZE;
  int info_h = 6 * TILE_SIZE;
  
  // Проверяем, пересекается ли перерисовываемая область с info table
  bool intersects_info = (r.x < info_x + info_w && r.x + r.w > info_x &&
                          r.y < info_y + info_h && r.y + r.h > info_y);
  
  for (int i = start_row; i <= end_row; i++) {
    for (int j = start_col; j <= end_col; j++) {
      uint16_t color = TFT_BLACK;
      switch(game_map_[i][j]) {
        case GRASS:       color = TFT_OLIVE;   break;
        case BRICKS_WALL: color = TFT_BROWN;   break;
        case SPECIAL:     color = TFT_MAGENTA; break;
        case BEDROCK:     color = TFT_DARKGREY;break;
        default:          color = TFT_BLACK;   break;
      }
      
      tft_.fillRect(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE, color);
    }
  }
  
  // Если была затронута info table, перерисовываем её полностью
  if (intersects_info && !tanks_.empty()) {
    draw_info_table();
    print_tank_data_to_info_table(*tanks_[0], true);
  }
}


void Game::cleanup_dead_objects() {

  collision_mgr_.cleanup();

  bullets_.erase(
    std::remove_if(bullets_.begin(), bullets_.end(), 
      [](const auto& b) {
        return b->is_dead();  
      }), 
    bullets_.end()
  );

  tanks_.erase(
    std::remove_if(tanks_.begin(), tanks_.end(), 
      [](const auto& t) {
        return t->is_dead(); 
      }), 
    tanks_.end()
  );
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

bool Game::is_out_of_bounds (Rect next) {
  bool out_of_bounds = (next.x < 0 || next.x + next.w > X_MAX ||
                          next.y < 0 || next.y + next.h > Y_MAX);
  return out_of_bounds;
}

void Game::move_player(std::vector<Rect>& dirty_rects) {
   int dx = 0, dy = 0;
  int speed = tanks_[0]->get_speed();

  if (buttons_[BTN_UP].status_)       dy = -speed;
  else if (buttons_[BTN_DOWN].status_)  dy =  speed;
  else if (buttons_[BTN_LEFT].status_)  dx = -speed;
  else if (buttons_[BTN_RIGHT].status_) dx =  speed;

  if (dx != 0 || dy != 0) {
    Rect next_r = tanks_[0]->get_collision_rect();
    next_r.x += dx;
    next_r.y += dy;

    dirty_rects.push_back(tanks_[0]->get_collision_rect());
    tanks_[0]->update_orientation(dx, dy);

    if (!is_out_of_bounds(next_r) && !collision_mgr_.handle_collisions(tanks_[0], next_r)) {
      tanks_[0]->move(dx, dy);
    }
    dirty_rects.push_back(tanks_[0]->get_collision_rect());
  }
}
void Game::move_bots(std::vector<Rect>& dirty_rects) {
  for (size_t i = 1; i < tanks_.size(); i++) {
    auto& tank = tanks_[i];
    
    // Пропускаем взрывающиеся танки
    if (tank->is_exploding()) continue;
    
    dirty_rects.push_back(tank->get_collision_rect());
    
    // Рассчитываем dx, dy на основе ориентации бота
    int dx = 0, dy = 0;
    int speed = tank->get_speed();
    
    // Определяем направление движения на основе текущей ориентации
    switch(tank->getOrientation()) {
      case DIR_UP:    dy = -speed; break;
      case DIR_DOWN:  dy =  speed; break;
      case DIR_LEFT:  dx = -speed; break;
      case DIR_RIGHT: dx =  speed; break;
    }
    
    if (dx != 0 || dy != 0) {
      Rect next_r = tank->get_collision_rect();
      next_r.x += dx;
      next_r.y += dy;
      
      if (!is_out_of_bounds(next_r) && !collision_mgr_.handle_collisions(tank, next_r)) {
        tank->move(dx, dy);
      }
    }
    
    dirty_rects.push_back(tank->get_collision_rect());
  }
}
