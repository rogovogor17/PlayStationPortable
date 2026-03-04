#ifndef BUTTON_CLASS
#define BUTTON_CLASS

#include <stdlib.h>
#include <vector>
#include <TFT_eSPI.h>

enum ButtonID {
  BTN_UP,
  BTN_DOWN,
  BTN_LEFT,
  BTN_RIGHT,
  BTN_X,
  BTN_Y,
  BTN_A,
  BTN_B,
  BTN_PAUSA,
  BTN_COUNT  // Number of buttons
};

constexpr int BTN_UP_PIN    = 17;  //35
constexpr int BTN_DOWN_PIN  = 32; 
constexpr int BTN_RIGHT_PIN = 19;  //34 
constexpr int BTN_LEFT_PIN  = 33; 

constexpr int BTN_X_PIN     = 26;  
constexpr int BTN_Y_PIN     = 27;  
constexpr int BTN_A_PIN     = 12;  
constexpr int BTN_B_PIN     = 14;  

constexpr int BTN_PAUSA_PIN = 25;  

class Button final {
    size_t pin_;
    
    public:
        int status_;
        
        Button(size_t pin) : pin_(pin), status_(false)  {
            pinMode(pin, INPUT_PULLUP);
        }

        void update() {
            status_ = digitalRead(pin_) == LOW; // true when button is pressed, false when button is not pressed
        }
};


#endif