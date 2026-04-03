#pragma once
#include <Arduino.h>

extern int fftResult[16];
extern int gain;
extern int squelch;

void setupAudio();
void FFTcode(void * parameter);
double fftAdd(int from, int to);