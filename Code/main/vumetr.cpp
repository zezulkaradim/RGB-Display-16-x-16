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
#include "audio_reactive.h"// Kvůli funkci XY()

void drawRainbowBar(uint8_t band, uint8_t height) {
  for (uint8_t y = 0; y <= height; y++) {
    leds[XY(band, (HEIGHT - 1) - y)] = CHSV(map(band, 0, 15, 0, 255), 255, 255);
  }
}

void drawWhitePeak(uint8_t band) {
  leds[XY(band, (HEIGHT - 1) - peak[band])] = CRGB::White;
}

void decayPeaks() {
  static uint32_t lastDecay = 0;
  if (millis() - lastDecay > 60) {
    lastDecay = millis();
    for (uint8_t i = 0; i < 16; i++) {
      if (peak[i] > 0) peak[i]--;
    }
  }
}

void vumetr() {
  FastLED.clear();
  for (uint8_t band = 0; band < 16; band++) {
    uint8_t fftVal = ((prevFFTValue[band] * 3) + fftResult[band]) / 4;
    prevFFTValue[band] = fftVal;
    uint8_t height = map(fftVal, 0, 255, 0, HEIGHT - 1);
    barHeights[band] = height;
    
    if (height > peak[band]) peak[band] = height;
    
    drawRainbowBar(band, height);
    drawWhitePeak(band);
  }
  decayPeaks();
  FastLED.show();
}