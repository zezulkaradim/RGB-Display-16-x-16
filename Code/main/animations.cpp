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

void rainbowwave() {
  FastLED.clear();
  static uint16_t t = 0;
  EVERY_N_MILLISECONDS(5) {
    t++;
    for (uint8_t y = 0; y < HEIGHT; y++)
      for (uint8_t x = 0; x < WIDTH; x++)
        leds[XY(x,y)] = CHSV((x*6 + y*6 + t) % 255, 255, 255);
    FastLED.show();
  }
}

void rainbowcycle() {
  FastLED.clear();
  static uint16_t t = 0;
  EVERY_N_MILLISECONDS(5) {
    t++;
    float cx = (WIDTH - 1) / 2.0, cy = (HEIGHT - 1) / 2.0;
    for (uint8_t y = 0; y < HEIGHT; y++) {
      for (uint8_t x = 0; x < WIDTH; x++) {
        float dx = x - cx, dy = y - cy;
        uint8_t hue = (uint8_t)(atan2(dy,dx) * 40 + sqrt(dx*dx+dy*dy) * 15 + t);
        leds[XY(x,y)] = CHSV(hue, 255, 255);
      }
    }
    FastLED.show();
  }
}

// --- Rachejtle ---

void exploze1() {
  leds[XY(raketaX,   raketaY)]   = CHSV(hueraketa,255,255);
  leds[XY(raketaX+1, raketaY)]   = CHSV(hueraketa,255,255);
  leds[XY(raketaX+1, raketaY+1)] = CHSV(hueraketa,255,255);
  leds[XY(raketaX,   raketaY+1)] = CHSV(hueraketa,255,255);
  leds[XY(raketaX-1, raketaY+1)] = CHSV(hueraketa,255,255);
  leds[XY(raketaX-1, raketaY)]   = CHSV(hueraketa,255,255);
  leds[XY(raketaX-1, raketaY-1)] = CHSV(hueraketa,255,255);
  leds[XY(raketaX,   raketaY-1)] = CHSV(hueraketa,255,255);
  leds[XY(raketaX+1, raketaY-1)] = CHSV(hueraketa,255,255);
}

void exploze2() {
  exploze1();
  leds[XY(raketaX+2,raketaY)]   = CHSV(hueraketa,255,255);
  leds[XY(raketaX+3,raketaY)]   = CHSV(hueraketa,255,255);
  leds[XY(raketaX,  raketaY+2)] = CHSV(hueraketa,255,255);
  leds[XY(raketaX,  raketaY+3)] = CHSV(hueraketa,255,255);
  leds[XY(raketaX-2,raketaY)]   = CHSV(hueraketa,255,255);
  leds[XY(raketaX-3,raketaY)]   = CHSV(hueraketa,255,255);
  leds[XY(raketaX,  raketaY-2)] = CHSV(hueraketa,255,255);
  leds[XY(raketaX,  raketaY-3)] = CHSV(hueraketa,255,255);
  leds[XY(raketaX+2,raketaY+1)] = CHSV(hueraketa,255,255);
  leds[XY(raketaX+1,raketaY+2)] = CHSV(hueraketa,255,255);
  leds[XY(raketaX-1,raketaY+2)] = CHSV(hueraketa,255,255);
  leds[XY(raketaX-2,raketaY+1)] = CHSV(hueraketa,255,255);
  leds[XY(raketaX-1,raketaY-2)] = CHSV(hueraketa,255,255);
  leds[XY(raketaX+1,raketaY-2)] = CHSV(hueraketa,255,255);
  leds[XY(raketaX+2,raketaY-1)] = CHSV(hueraketa,255,255);
  leds[XY(raketaX-2,raketaY-1)] = CHSV(hueraketa,255,255);
}

void exploze3() {
  exploze2();
  leds[XY(raketaX+3,raketaY+1)] = CHSV(hueraketa,255,255);
  leds[XY(raketaX+3,raketaY+2)] = CHSV(hueraketa,255,255);
  leds[XY(raketaX+3,raketaY-1)] = CHSV(hueraketa,255,255);
  leds[XY(raketaX+3,raketaY-2)] = CHSV(hueraketa,255,255);
  leds[XY(raketaX+2,raketaY+3)] = CHSV(hueraketa,255,255);
  leds[XY(raketaX+1,raketaY+3)] = CHSV(hueraketa,255,255);
  leds[XY(raketaX-1,raketaY+3)] = CHSV(hueraketa,255,255);
  leds[XY(raketaX-2,raketaY+3)] = CHSV(hueraketa,255,255);
  leds[XY(raketaX-3,raketaY+2)] = CHSV(hueraketa,255,255);
  leds[XY(raketaX-3,raketaY+1)] = CHSV(hueraketa,255,255);
  leds[XY(raketaX-3,raketaY-1)] = CHSV(hueraketa,255,255);
  leds[XY(raketaX-3,raketaY-2)] = CHSV(hueraketa,255,255);
  leds[XY(raketaX-2,raketaY-3)] = CHSV(hueraketa,255,255);
  leds[XY(raketaX-1,raketaY-3)] = CHSV(hueraketa,255,255);
  leds[XY(raketaX+1,raketaY-3)] = CHSV(hueraketa,255,255);
  leds[XY(raketaX+2,raketaY-3)] = CHSV(hueraketa,255,255);
  leds[XY(raketaX+2,raketaY-2)] = CHSV(hueraketa,255,255);
  leds[XY(raketaX+2,raketaY+2)] = CHSV(hueraketa,255,255);
  leds[XY(raketaX-2,raketaY+2)] = CHSV(hueraketa,255,255);
  leds[XY(raketaX-2,raketaY-2)] = CHSV(hueraketa,255,255);
}

void rachejtle() {
  unsigned long now = millis();

  if (!raketaleti && !fadeActive && (now - posledniRaketa >= 1500)) {
    raketaX = random(3, 13); raketaY = random(3, 13);
    pozicerakety = HEIGHT - 1;
    hueraketa += 15;
    raketaleti = true; fazeexploze = 0;
    posledniRaketa = now;
  }

  if (raketaleti && now - posledniPohyb > 20) {
    FastLED.clear();
    leds[XY(raketaX, pozicerakety)] = CRGB::White;
    FastLED.show();
    pozicerakety--;
    posledniPohyb = now;
    if (pozicerakety <= raketaY) {
      raketaleti = false; fazeexploze = 1; posledniexploze = now;
    }
  }

  if (fazeexploze >= 1 && fazeexploze <= 3 && !fadeActive) {
    if (now - posledniexploze >= 80) {
      FastLED.clear();
      if (fazeexploze == 1) exploze1();
      else if (fazeexploze == 2) exploze2();
      else if (fazeexploze == 3) {
        exploze3();
        for (int i = 0; i < NUM_LEDS; i++) ledsohnastroj[i] = leds[i];
        fadeActive = true; casFadeStart = now;
      }
      FastLED.show();
      fazeexploze++;
      posledniexploze = now;
    }
  }

  if (fadeActive) {
    unsigned long elapsed = now - casFadeStart;
    if (elapsed <= 1000) {
      uint8_t brightness = map(elapsed, 0, 1000, 255, 0);
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ledsohnastroj[i];
        leds[i].nscale8_video(brightness);
      }
      FastLED.show();
    } else {
      fadeActive = false;
      FastLED.clear(); FastLED.show();
    }
  }
}

// --- Oheň ---

void Fire2D(uint8_t cooling, uint8_t sparking, uint8_t speedDelay) {
  static unsigned long last = 0;
  unsigned long now = millis();
  if (now - last < speedDelay) return;
  last = now;

  for (uint8_t x = 0; x < WIDTH; x++)
    for (uint8_t y = 0; y < HEIGHT; y++)
      heat[x][y] = qsub8(heat[x][y], random8(0, (cooling * 10 / HEIGHT) + 2));

  uint8_t newHeat[WIDTH][HEIGHT];
  memset(newHeat, 0, sizeof(newHeat));

  for (uint8_t x = 0; x < WIDTH; x++) {
    for (uint8_t y = 0; y < HEIGHT; y++) {
      uint16_t sum = heat[x][y]; uint8_t count = 1;
      uint8_t yb = (y < HEIGHT-1) ? y+1 : y;
      uint8_t xl = (x == 0) ? WIDTH-1 : x-1;
      uint8_t xr = (x == WIDTH-1) ? 0 : x+1;
      sum += heat[x][yb];  count++;
      sum += heat[xl][yb]; count++;
      sum += heat[xr][yb]; count++;
      newHeat[x][y] = sum / count;
    }
  }
  memcpy(heat, newHeat, sizeof(heat));

  for (uint8_t x = 0; x < WIDTH; x++)
    if (random8() < sparking)
      heat[x][HEIGHT-1] = qadd8(heat[x][HEIGHT-1], random8(160,255));

  for (uint8_t x = 0; x < WIDTH; x++)
    for (uint8_t y = 0; y < HEIGHT; y++)
      leds[XY(x,y)] = ColorFromPalette(HeatColors_p, scale8(heat[x][y], 240));

  FastLED.show();
}
