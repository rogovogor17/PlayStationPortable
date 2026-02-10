#ifndef TANKS_H
#define TANKS_H

#include <SPI.h>
#include <TFT_eSPI.h> 
#include "Board_properties.h"

const size_t COUNT_DOWN = 5;

class game {
    TFT_eSPI& tft;
    public:
        game(TFT_eSPI& tft) : tft(tft) {};
        void start(void);
};

void CountDown(TFT_eSPI& tft);

#endif