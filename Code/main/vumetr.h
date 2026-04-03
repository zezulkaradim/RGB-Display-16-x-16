#pragma once
#include <Arduino.h>

// Deklarujeme funkce (prototypy), aby o nich ostatní věděli
void drawRainbowBar(uint8_t band, uint8_t height);
void drawWhitePeak(uint8_t band);
void decayPeaks();
void vumetr();