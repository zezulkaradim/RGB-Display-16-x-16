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
void youlosthad() {
  starthada = false; setyoulosthad = true;
  unsigned long hadmillisted = millis();
  if (hadmillisted - hadpredchozimillis >= 400) {
    hadpredchozimillis = hadmillisted;
    blikyoulost = !blikyoulost;
  }

  FastLED.clear();
  if (blikyoulost) {
    // YOU LOST text pixel-art
    int x = 0;
    for (int i=0;i<=9;i++) if(i<2||i>4) leds[XY(x,i)]=CRGB(255,0,0);
    x=1; for(int i=0;i<=9;i++) if((i>0&&i<4)||i>8) leds[XY(x,i)]=CRGB(255,0,0);
    x=2; for(int i=0;i<=9;i++) if(i<2||i>8) leds[XY(x,i)]=CRGB(255,0,0);
    x=4; for(int i=0;i<=9;i++) if(i<4||i>4) leds[XY(x,i)]=CRGB(255,0,0);
    x=5;
    leds[XY(x,0)]=CRGB(255,0,0); leds[XY(x,3)]=CRGB(255,0,0);
    leds[XY(x,5)]=CRGB(255,0,0); leds[XY(x,9)]=CRGB(255,0,0);
    x=6; for(int i=0;i<=9;i++) if(i<4||i>4) leds[XY(x,i)]=CRGB(255,0,0);
    x=8; for(int i=0;i<=7;i++) if(i<4||i>4) leds[XY(x,i)]=CRGB(255,0,0);
    leds[XY(x,9)]=CRGB(255,0,0);
    x=9;
    leds[XY(x,9)]=CRGB(255,0,0); leds[XY(x,7)]=CRGB(255,0,0);
    leds[XY(x,5)]=CRGB(255,0,0); leds[XY(x,3)]=CRGB(255,0,0);
    x=10; for(int i=0;i<=9;i++) if(i<4||i>6) leds[XY(x,i)]=CRGB(255,0,0);
    leds[XY(x,5)]=CRGB(255,0,0);
    x=12; leds[XY(x,5)]=CRGB(255,0,0);
    x=13; for(int i=5;i<=9;i++) leds[XY(x,i)]=CRGB(255,0,0);
    x=14; leds[XY(x,5)]=CRGB(255,0,0);
    FastLED.show();
    blikyoulost = true;
  } else {
    for (int i=0;i<=159;i++) leds[i]=CRGB::Black;
  }

  tisicehad  = pocetbodu / 1000;
  stovkyhad  = (pocetbodu / 100) % 10;
  desitkyhad = (pocetbodu / 10) % 10;
  jednotkyhad = pocetbodu % 10;
  vykreslicislo(0,  15, tisicehad,  255,255,255);
  vykreslicislo(4,  15, stovkyhad,  255,255,255);
  vykreslicislo(8,  15, desitkyhad, 255,255,255);
  vykreslicislo(12, 15, jednotkyhad,255,255,255);
  FastLED.show();
}

void generujjablko() {
  bool chyba;
  do {
    chyba = false;
    xjablko = random(1,15); yjablko = random(1,15);
    for (int i=0; i<=delkahada; i++)
      if (xjablko==had[i].x && yjablko==had[i].y) { chyba=true; break; }
  } while (chyba);
}

void haddoprava() {
  if (had[0].x==xjablko && had[0].y==yjablko) {
    delkahada++; pocetbodu++;
    for(int i=delkahada;i>0;i--){had[i].x=had[i-1].x;had[i].y=had[i-1].y;}
    had[0].x++; generujjablko(); return;
  }
  for(int i=0;i<=delkahada;i++) if(had[0].x+1==had[i].x&&had[0].y==had[i].y){youlosthad();return;}
  if(had[0].x+1>15){if(dotekzdi)youlosthad();else dotekzdi=true;return;}
  dotekzdi=false;
  for(int i=delkahada;i>0;i--){had[i].x=had[i-1].x;had[i].y=had[i-1].y;}
  had[0].x++;
}

void haddoleva() {
  if (had[0].x==xjablko && had[0].y==yjablko) {
    delkahada++; pocetbodu++; generujjablko();
    for(int i=delkahada;i>0;i--){had[i].x=had[i-1].x;had[i].y=had[i-1].y;}
    had[0].x--; return;
  }
  for(int i=0;i<=delkahada;i++) if(had[0].x-1==had[i].x&&had[0].y==had[i].y){youlosthad();return;}
  if(had[0].x-1<0){if(dotekzdi)youlosthad();else dotekzdi=true;return;}
  dotekzdi=false;
  for(int i=delkahada;i>0;i--){had[i].x=had[i-1].x;had[i].y=had[i-1].y;}
  had[0].x--;
}

void hadnahoru() {
  if (had[0].x==xjablko && had[0].y==yjablko) {
    delkahada++; pocetbodu++; generujjablko();
    for(int i=delkahada;i>0;i--){had[i].x=had[i-1].x;had[i].y=had[i-1].y;}
    had[0].y--; return;
  }
  for(int i=0;i<=delkahada;i++) if(had[0].x==had[i].x&&had[0].y-1==had[i].y){youlosthad();return;}
  if(had[0].y-1<0){if(dotekzdi)youlosthad();else dotekzdi=true;return;}
  dotekzdi=false;
  for(int i=delkahada;i>0;i--){had[i].x=had[i-1].x;had[i].y=had[i-1].y;}
  had[0].y--;
}

void haddolu() {
  if (had[0].x==xjablko && had[0].y==yjablko) {
    delkahada++; pocetbodu++; generujjablko();
    for(int i=delkahada;i>0;i--){had[i].x=had[i-1].x;had[i].y=had[i-1].y;}
    had[0].y++; return;
  }
  for(int i=0;i<=delkahada;i++) if(had[0].x==had[i].x&&had[0].y+1==had[i].y){youlosthad();return;}
  if(had[0].y+1>15){if(dotekzdi)youlosthad();else dotekzdi=true;return;}
  dotekzdi=false;
  for(int i=delkahada;i>0;i--){had[i].x=had[i-1].x;had[i].y=had[i-1].y;}
  had[0].y++;
}

void snake() {
  if (setyoulosthad) { youlosthad(); return; }

  if (!starthada) {
    FastLED.clear();
    while (xjablko==had[0].x && yjablko==had[0].y) {
      xjablko=random(1,15); yjablko=random(1,15);
      had[0].x=random(3,13); had[0].y=random(3,13);
    }
    leds[XY(xjablko,yjablko)]=CRGB(255,0,0);
    leds[XY(had[0].x,had[0].y)]=CRGB(0,255,0);
    FastLED.show();
  }

  unsigned long now = millis();
  if (now - hadmainlastmillis < (unsigned long)(500-(5*pocetbodu))) return;
  hadmainlastmillis = now;

  if (starthada) {
    if (doprava) haddoprava();
    if (doleva)  haddoleva();
    if (dolu)    haddolu();
    if (nahoru)  hadnahoru();
    FastLED.clear();
    leds[XY(xjablko,yjablko)]=CRGB(255,0,0);
    for(int i=0;i<=delkahada;i++)
      leds[XY(had[i].x,had[i].y)] = (i%2==0) ? CRGB(0,125,0) : CRGB(0,255,0);
    FastLED.show();
  }
}
