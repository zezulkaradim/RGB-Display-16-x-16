#pragma once
#include "config.h"
#include "display_utils.h"
#include "buttons.h"
#include "clock_wifi.h"
#include "animations.h"
#include "snake.h"
#include "vumetr.h"
#include "drawing.h"
#include "sprites.h"
#include "audio_reactive.h"

// Převod (x,y) → index pro had-uspořádanou matici
uint16_t XY(uint8_t x, uint8_t y) {
  if (y % 2 == 0) {
    return (y * WIDTH) + x;
  } else {
    return (y * WIDTH) + (WIDTH - 1 - x);
  }
}

// Vykreslení číslice 0–9 na pozici (X, Y) s danou barvou
void vykreslicislo(uint8_t X, uint8_t Y, uint8_t cislo, uint8_t R, uint8_t G, uint8_t B) {
  if (cislo == 0) {
    leds[XY(X,Y)]   = CRGB(R,G,B); leds[XY(X,Y-1)] = CRGB(R,G,B);
    leds[XY(X,Y-2)] = CRGB(R,G,B); leds[XY(X,Y-3)] = CRGB(R,G,B);
    leds[XY(X,Y-4)] = CRGB(R,G,B); leds[XY(X+1,Y-4)] = CRGB(R,G,B);
    leds[XY(X+1,Y)] = CRGB(R,G,B); leds[XY(X+2,Y)]   = CRGB(R,G,B);
    leds[XY(X+2,Y-1)] = CRGB(R,G,B); leds[XY(X+2,Y-2)] = CRGB(R,G,B);
    leds[XY(X+2,Y-3)] = CRGB(R,G,B); leds[XY(X+2,Y-4)] = CRGB(R,G,B);
  }
  if (cislo == 1) {
    leds[XY(X+2,Y)]   = CRGB(R,G,B); leds[XY(X+2,Y-1)] = CRGB(R,G,B);
    leds[XY(X+2,Y-2)] = CRGB(R,G,B); leds[XY(X+2,Y-3)] = CRGB(R,G,B);
    leds[XY(X+2,Y-4)] = CRGB(R,G,B);
  }
  if (cislo == 2) {
    leds[XY(X,Y)]   = CRGB(R,G,B); leds[XY(X,Y-1)] = CRGB(R,G,B);
    leds[XY(X,Y-2)] = CRGB(R,G,B); leds[XY(X,Y-4)] = CRGB(R,G,B);
    leds[XY(X+1,Y)] = CRGB(R,G,B); leds[XY(X+1,Y-2)] = CRGB(R,G,B);
    leds[XY(X+1,Y-4)] = CRGB(R,G,B); leds[XY(X+2,Y)] = CRGB(R,G,B);
    leds[XY(X+2,Y-2)] = CRGB(R,G,B); leds[XY(X+2,Y-3)] = CRGB(R,G,B);
    leds[XY(X+2,Y-4)] = CRGB(R,G,B);
  }
  if (cislo == 3) {
    leds[XY(X+2,Y)]   = CRGB(R,G,B); leds[XY(X+2,Y-1)] = CRGB(R,G,B);
    leds[XY(X+2,Y-2)] = CRGB(R,G,B); leds[XY(X+2,Y-3)] = CRGB(R,G,B);
    leds[XY(X+2,Y-4)] = CRGB(R,G,B); leds[XY(X,Y)]     = CRGB(R,G,B);
    leds[XY(X,Y-2)]   = CRGB(R,G,B); leds[XY(X,Y-4)]   = CRGB(R,G,B);
    leds[XY(X+1,Y)]   = CRGB(R,G,B); leds[XY(X+1,Y-2)] = CRGB(R,G,B);
    leds[XY(X+1,Y-4)] = CRGB(R,G,B);
  }
  if (cislo == 4) {
    leds[XY(X+2,Y)]   = CRGB(R,G,B); leds[XY(X+2,Y-1)] = CRGB(R,G,B);
    leds[XY(X+2,Y-2)] = CRGB(R,G,B); leds[XY(X+2,Y-3)] = CRGB(R,G,B);
    leds[XY(X+2,Y-4)] = CRGB(R,G,B); leds[XY(X,Y-2)]   = CRGB(R,G,B);
    leds[XY(X,Y-3)]   = CRGB(R,G,B); leds[XY(X,Y-4)]   = CRGB(R,G,B);
    leds[XY(X+1,Y-2)] = CRGB(R,G,B);
  }
  if (cislo == 5) {
    leds[XY(X,Y)]   = CRGB(R,G,B); leds[XY(X,Y-2)]   = CRGB(R,G,B);
    leds[XY(X,Y-3)] = CRGB(R,G,B); leds[XY(X,Y-4)]   = CRGB(R,G,B);
    leds[XY(X+1,Y)] = CRGB(R,G,B); leds[XY(X+1,Y-2)] = CRGB(R,G,B);
    leds[XY(X+1,Y-4)] = CRGB(R,G,B); leds[XY(X+2,Y)] = CRGB(R,G,B);
    leds[XY(X+2,Y-1)] = CRGB(R,G,B); leds[XY(X+2,Y-2)] = CRGB(R,G,B);
    leds[XY(X+2,Y-4)] = CRGB(R,G,B);
  }
  if (cislo == 6) {
    leds[XY(X,Y)]   = CRGB(R,G,B); leds[XY(X,Y-1)] = CRGB(R,G,B);
    leds[XY(X,Y-2)] = CRGB(R,G,B); leds[XY(X,Y-3)] = CRGB(R,G,B);
    leds[XY(X,Y-4)] = CRGB(R,G,B); leds[XY(X+1,Y)] = CRGB(R,G,B);
    leds[XY(X+1,Y-2)] = CRGB(R,G,B); leds[XY(X+1,Y-4)] = CRGB(R,G,B);
    leds[XY(X+2,Y)]   = CRGB(R,G,B); leds[XY(X+2,Y-1)] = CRGB(R,G,B);
    leds[XY(X+2,Y-2)] = CRGB(R,G,B); leds[XY(X+2,Y-4)] = CRGB(R,G,B);
  }
  if (cislo == 7) {
    leds[XY(X+2,Y)]   = CRGB(R,G,B); leds[XY(X+2,Y-1)] = CRGB(R,G,B);
    leds[XY(X+2,Y-2)] = CRGB(R,G,B); leds[XY(X+2,Y-3)] = CRGB(R,G,B);
    leds[XY(X+2,Y-4)] = CRGB(R,G,B); leds[XY(X,Y-4)]   = CRGB(R,G,B);
    leds[XY(X+1,Y-4)] = CRGB(R,G,B);
  }
  if (cislo == 8) {
    leds[XY(X,Y)]   = CRGB(R,G,B); leds[XY(X,Y-1)] = CRGB(R,G,B);
    leds[XY(X,Y-2)] = CRGB(R,G,B); leds[XY(X,Y-3)] = CRGB(R,G,B);
    leds[XY(X,Y-4)] = CRGB(R,G,B); leds[XY(X+1,Y)] = CRGB(R,G,B);
    leds[XY(X+1,Y-2)] = CRGB(R,G,B); leds[XY(X+1,Y-4)] = CRGB(R,G,B);
    leds[XY(X+2,Y)]   = CRGB(R,G,B); leds[XY(X+2,Y-1)] = CRGB(R,G,B);
    leds[XY(X+2,Y-2)] = CRGB(R,G,B); leds[XY(X+2,Y-3)] = CRGB(R,G,B);
    leds[XY(X+2,Y-4)] = CRGB(R,G,B);
  }
  if (cislo == 9) {
    leds[XY(X+2,Y)]   = CRGB(R,G,B); leds[XY(X+2,Y-1)] = CRGB(R,G,B);
    leds[XY(X+2,Y-2)] = CRGB(R,G,B); leds[XY(X+2,Y-3)] = CRGB(R,G,B);
    leds[XY(X+2,Y-4)] = CRGB(R,G,B); leds[XY(X,Y)]     = CRGB(R,G,B);
    leds[XY(X,Y-2)]   = CRGB(R,G,B); leds[XY(X,Y-3)]   = CRGB(R,G,B);
    leds[XY(X,Y-4)]   = CRGB(R,G,B); leds[XY(X+1,Y)]   = CRGB(R,G,B);
    leds[XY(X+1,Y-2)] = CRGB(R,G,B); leds[XY(X+1,Y-4)] = CRGB(R,G,B);
  }
}
