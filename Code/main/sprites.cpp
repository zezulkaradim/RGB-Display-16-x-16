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

// ===================== HAD - animace na menu =====================

void telohada1() {
  int x=0;
  for(int i=0;i<=15;i++) if(i>10&&i<14) leds[XY(x,i)]=CRGB(0,80,0);
  x=1; for(int i=0;i<=15;i++) if(i>7&&i<12) leds[XY(x,i)]=CRGB(0,80,0);
  leds[XY(1,14)]=CRGB(0,80,0); leds[XY(1,15)]=CRGB(0,80,0);
  x=2; leds[XY(x,10)]=CRGB(0,80,0); leds[XY(x,11)]=CRGB(0,80,0); leds[XY(x,15)]=CRGB(0,80,0);
  x=3; leds[XY(x,10)]=CRGB(0,80,0); leds[XY(x,11)]=CRGB(0,80,0); leds[XY(x,15)]=CRGB(0,80,0);
  x=4; leds[XY(x,11)]=CRGB(0,80,0); leds[XY(x,15)]=CRGB(0,80,0);
  x=5; leds[XY(x,11)]=CRGB(0,80,0); leds[XY(x,15)]=CRGB(0,80,0);
  x=6; leds[XY(x,8)]=CRGB(0,80,0); leds[XY(x,11)]=CRGB(0,80,0); leds[XY(x,12)]=CRGB(0,80,0); leds[XY(x,15)]=CRGB(0,80,0);
  for(x=7;x<13;x++){leds[XY(x,8)]=CRGB(0,80,0);leds[XY(x,12)]=CRGB(0,80,0);leds[XY(x,15)]=CRGB(0,80,0);}
  x=13; leds[XY(x,8)]=CRGB(0,80,0);leds[XY(x,9)]=CRGB(0,80,0);leds[XY(x,12)]=CRGB(0,80,0);leds[XY(x,13)]=CRGB(0,80,0);leds[XY(x,15)]=CRGB(0,80,0);
  x=14; for(int i=0;i<=15;i++) if(i>10) leds[XY(x,i)]=CRGB(0,80,0);
  leds[XY(14,9)]=CRGB(0,80,0);
  x=15; for(int i=0;i<=15;i++) if((i>8&&i<15)&&(i!=12)) leds[XY(x,i)]=CRGB(0,80,0);

  x=2; leds[XY(x,13)]=CRGB(70,255,0);
  x=3; leds[XY(x,8)]=CRGB(70,255,0);leds[XY(x,9)]=CRGB(70,255,0);leds[XY(x,13)]=CRGB(70,255,0);leds[XY(x,14)]=CRGB(70,255,0);
  x=4; leds[XY(x,8)]=CRGB(70,255,0);leds[XY(x,9)]=CRGB(70,255,0);leds[XY(x,10)]=CRGB(70,255,0);leds[XY(x,13)]=CRGB(70,255,0);leds[XY(x,14)]=CRGB(70,255,0);
  x=5; leds[XY(x,9)]=CRGB(70,255,0);leds[XY(x,10)]=CRGB(70,255,0);leds[XY(x,13)]=CRGB(70,255,0);leds[XY(x,14)]=CRGB(70,255,0);
  x=6; leds[XY(x,9)]=CRGB(70,255,0);leds[XY(x,10)]=CRGB(70,255,0);leds[XY(x,13)]=CRGB(70,255,0);leds[XY(x,14)]=CRGB(70,255,0);
  x=7; leds[XY(x,9)]=CRGB(70,255,0);leds[XY(x,10)]=CRGB(70,255,0);leds[XY(x,14)]=CRGB(70,255,0);
  for(x=7;x<12;x++){leds[XY(x,9)]=CRGB(70,255,0);leds[XY(x,10)]=CRGB(70,255,0);leds[XY(x,11)]=CRGB(70,255,0);leds[XY(x,14)]=CRGB(70,255,0);}
  x=12; leds[XY(x,10)]=CRGB(70,255,0);leds[XY(x,11)]=CRGB(70,255,0);leds[XY(x,14)]=CRGB(70,255,0);
  x=13; leds[XY(x,10)]=CRGB(70,255,0);

  x=1; leds[XY(x,12)]=CRGB(0,255,0);leds[XY(x,13)]=CRGB(0,255,0);
  x=2; leds[XY(x,8)]=CRGB(0,255,0);leds[XY(x,9)]=CRGB(0,255,0);leds[XY(x,12)]=CRGB(0,255,0);leds[XY(x,14)]=CRGB(0,255,0);
  x=3; leds[XY(x,12)]=CRGB(0,255,0);
  x=4; leds[XY(x,12)]=CRGB(0,255,0);
  x=5; leds[XY(x,8)]=CRGB(0,255,0);leds[XY(x,12)]=CRGB(0,255,0);
  x=7; leds[XY(x,11)]=CRGB(0,255,0);leds[XY(x,13)]=CRGB(0,255,0);
  for(x=7;x<12;x++) leds[XY(x,13)]=CRGB(70,255,0);
  x=12; leds[XY(x,9)]=CRGB(0,255,0);leds[XY(x,13)]=CRGB(0,255,0);
  x=13; leds[XY(x,11)]=CRGB(0,255,0);leds[XY(x,14)]=CRGB(0,255,0);
  x=14; leds[XY(x,10)]=CRGB(0,255,0);
}

void telohada2() {
  int x=0;
  for(int i=0;i<=15;i++) if(i>10&&i<14) leds[XY(x,i)]=CRGB(0,80,0);
  x=1; for(int i=0;i<=15;i++) if(i>7&&i<12) leds[XY(x,i)]=CRGB(0,80,0);
  leds[XY(1,14)]=CRGB(0,80,0); leds[XY(1,15)]=CRGB(0,80,0);
  x=2; leds[XY(x,8)]=CRGB(0,80,0);leds[XY(x,10)]=CRGB(0,80,0);leds[XY(x,11)]=CRGB(0,80,0);leds[XY(x,15)]=CRGB(0,80,0);
  x=3; leds[XY(x,10)]=CRGB(0,80,0);leds[XY(x,11)]=CRGB(0,80,0);leds[XY(x,15)]=CRGB(0,80,0);
  x=4; leds[XY(x,11)]=CRGB(0,80,0);leds[XY(x,15)]=CRGB(0,80,0);
  x=5; leds[XY(x,11)]=CRGB(0,80,0);leds[XY(x,15)]=CRGB(0,80,0);
  x=6; leds[XY(x,8)]=CRGB(0,80,0);leds[XY(x,11)]=CRGB(0,80,0);leds[XY(x,12)]=CRGB(0,80,0);leds[XY(x,15)]=CRGB(0,80,0);
  for(x=7;x<13;x++){leds[XY(x,8)]=CRGB(0,80,0);leds[XY(x,12)]=CRGB(0,80,0);leds[XY(x,15)]=CRGB(0,80,0);}
  x=13; leds[XY(x,8)]=CRGB(0,80,0);leds[XY(x,9)]=CRGB(0,80,0);leds[XY(x,12)]=CRGB(0,80,0);leds[XY(x,13)]=CRGB(0,80,0);leds[XY(x,15)]=CRGB(0,80,0);
  x=14; for(int i=0;i<=15;i++) if(i>10) leds[XY(x,i)]=CRGB(0,80,0);
  leds[XY(14,9)]=CRGB(0,80,0);
  x=15; for(int i=0;i<=15;i++) if((i>8&&i<15)&&(i!=12)) leds[XY(x,i)]=CRGB(0,80,0);

  x=2; leds[XY(x,13)]=CRGB(70,255,0);
  x=3; leds[XY(x,8)]=CRGB(70,255,0);leds[XY(x,9)]=CRGB(70,255,0);leds[XY(x,13)]=CRGB(70,255,0);leds[XY(x,14)]=CRGB(70,255,0);
  x=4; leds[XY(x,8)]=CRGB(70,255,0);leds[XY(x,9)]=CRGB(70,255,0);leds[XY(x,10)]=CRGB(70,255,0);leds[XY(x,13)]=CRGB(70,255,0);leds[XY(x,14)]=CRGB(70,255,0);
  x=5; leds[XY(x,9)]=CRGB(70,255,0);leds[XY(x,10)]=CRGB(70,255,0);leds[XY(x,13)]=CRGB(70,255,0);leds[XY(x,14)]=CRGB(70,255,0);
  x=6; leds[XY(x,9)]=CRGB(70,255,0);leds[XY(x,10)]=CRGB(70,255,0);leds[XY(x,13)]=CRGB(70,255,0);leds[XY(x,14)]=CRGB(70,255,0);
  x=7; leds[XY(x,9)]=CRGB(70,255,0);leds[XY(x,10)]=CRGB(70,255,0);leds[XY(x,14)]=CRGB(70,255,0);
  for(x=7;x<12;x++){leds[XY(x,9)]=CRGB(70,255,0);leds[XY(x,10)]=CRGB(70,255,0);leds[XY(x,11)]=CRGB(70,255,0);leds[XY(x,14)]=CRGB(70,255,0);}
  x=12; leds[XY(x,10)]=CRGB(70,255,0);leds[XY(x,11)]=CRGB(70,255,0);leds[XY(x,14)]=CRGB(70,255,0);
  x=13; leds[XY(x,10)]=CRGB(70,255,0);

  x=1; leds[XY(x,12)]=CRGB(0,255,0);leds[XY(x,13)]=CRGB(0,255,0);
  x=2; leds[XY(x,9)]=CRGB(0,255,0);leds[XY(x,12)]=CRGB(0,255,0);leds[XY(x,14)]=CRGB(0,255,0);
  x=3; leds[XY(x,12)]=CRGB(0,255,0);
  x=4; leds[XY(x,12)]=CRGB(0,255,0);
  x=5; leds[XY(x,8)]=CRGB(0,255,0);leds[XY(x,12)]=CRGB(0,255,0);
  x=7; leds[XY(x,11)]=CRGB(0,255,0);leds[XY(x,13)]=CRGB(0,255,0);
  for(x=7;x<12;x++) leds[XY(x,13)]=CRGB(70,255,0);
  x=12; leds[XY(x,9)]=CRGB(0,255,0);leds[XY(x,13)]=CRGB(0,255,0);
  x=13; leds[XY(x,11)]=CRGB(0,255,0);leds[XY(x,14)]=CRGB(0,255,0);
  x=14; leds[XY(x,10)]=CRGB(0,255,0);
}

void krkhada1() {
  int x=2; leds[XY(x,6)]=CRGB(0,80,0);leds[XY(x,7)]=CRGB(0,80,0);
  x=3; leds[XY(x,4)]=CRGB(0,80,0);leds[XY(x,5)]=CRGB(0,80,0);
  x=4; leds[XY(x,3)]=CRGB(0,80,0);leds[XY(x,4)]=CRGB(0,80,0);
  x=5; leds[XY(x,3)]=CRGB(0,80,0);leds[XY(x,7)]=CRGB(0,80,0);
  x=6; leds[XY(x,2)]=CRGB(0,80,0);leds[XY(x,6)]=CRGB(0,80,0);leds[XY(x,7)]=CRGB(0,80,0);
  x=7; leds[XY(x,2)]=CRGB(0,80,0);leds[XY(x,5)]=CRGB(0,80,0);
  x=3; leds[XY(x,7)]=CRGB(70,255,0);
  x=7; leds[XY(x,3)]=CRGB(70,255,0);leds[XY(x,4)]=CRGB(70,255,0);
  x=3; leds[XY(x,6)]=CRGB(0,255,0);
  x=4; leds[XY(x,5)]=CRGB(0,255,0);leds[XY(x,6)]=CRGB(0,255,0);leds[XY(x,7)]=CRGB(0,255,0);
  x=5; leds[XY(x,4)]=CRGB(0,255,0);leds[XY(x,5)]=CRGB(0,255,0);leds[XY(x,6)]=CRGB(0,255,0);
  x=6; leds[XY(x,3)]=CRGB(0,255,0);leds[XY(x,4)]=CRGB(0,255,0);leds[XY(x,5)]=CRGB(0,255,0);
}

void krkhada2() {
  int x=2; leds[XY(x,6)]=CRGB(0,80,0);leds[XY(x,7)]=CRGB(0,80,0);
  x=3; leds[XY(x,5)]=CRGB(0,80,0);
  x=4; leds[XY(x,4)]=CRGB(0,80,0);
  x=6; leds[XY(x,7)]=CRGB(0,80,0);
  x=7; leds[XY(x,7)]=CRGB(0,80,0);
  x=3; leds[XY(x,7)]=CRGB(70,255,0);
  x=3; leds[XY(x,6)]=CRGB(0,255,0);
  x=4; leds[XY(x,5)]=CRGB(0,255,0);leds[XY(x,6)]=CRGB(0,255,0);leds[XY(x,7)]=CRGB(0,255,0);
  x=5; leds[XY(x,5)]=CRGB(0,255,0);leds[XY(x,6)]=CRGB(0,255,0);leds[XY(x,7)]=CRGB(0,255,0);
  x=6; leds[XY(x,5)]=CRGB(0,255,0);leds[XY(x,6)]=CRGB(0,255,0);
}

void hlavahada1() {
  int x=8; leds[XY(x,1)]=CRGB(0,80,0);leds[XY(x,5)]=CRGB(0,80,0);
  x=9; leds[XY(x,1)]=CRGB(0,80,0);leds[XY(x,6)]=CRGB(0,80,0);
  x=10; leds[XY(x,1)]=CRGB(0,80,0);leds[XY(x,6)]=CRGB(0,80,0);
  x=11; leds[XY(x,2)]=CRGB(0,80,0);leds[XY(x,4)]=CRGB(0,80,0);
  x=12; leds[XY(x,3)]=CRGB(0,80,0);leds[XY(x,4)]=CRGB(0,80,0);
  x=8; leds[XY(x,3)]=CRGB(70,255,0);leds[XY(x,4)]=CRGB(70,255,0);
  x=9; leds[XY(x,2)]=CRGB(70,255,0);leds[XY(x,3)]=CRGB(70,255,0);leds[XY(x,4)]=CRGB(70,255,0);
  x=10; leds[XY(x,2)]=CRGB(70,255,0);leds[XY(x,3)]=CRGB(70,255,0);leds[XY(x,4)]=CRGB(70,255,0);
  x=11; leds[XY(x,3)]=CRGB(70,255,0);
  x=9; leds[XY(x,5)]=CRGB(0,255,0);
  x=8; leds[XY(x,2)]=CRGB(162,255,196);
  x=12; leds[XY(x,2)]=CRGB(162,255,196);
  x=10; leds[XY(x,5)]=CRGB(133,0,0);
  x=11; leds[XY(x,6)]=CRGB(255,0,0);
}

void hlavahada2() {
  int x=8; leds[XY(x,5)]=CRGB(0,80,0);
  x=9; leds[XY(x,1)]=CRGB(0,80,0);leds[XY(x,4)]=CRGB(0,80,0);leds[XY(x,5)]=CRGB(0,80,0);leds[XY(x,6)]=CRGB(0,80,0);
  x=10; leds[XY(x,0)]=CRGB(0,80,0);leds[XY(x,3)]=CRGB(0,80,0);leds[XY(x,6)]=CRGB(0,80,0);
  x=11; leds[XY(x,0)]=CRGB(0,80,0);leds[XY(x,3)]=CRGB(0,80,0);leds[XY(x,6)]=CRGB(0,80,0);
  x=12; leds[XY(x,1)]=CRGB(0,80,0);leds[XY(x,2)]=CRGB(0,80,0);
  x=8; leds[XY(x,2)]=CRGB(70,255,0);leds[XY(x,3)]=CRGB(70,255,0);leds[XY(x,4)]=CRGB(70,255,0);
  x=9; leds[XY(x,2)]=CRGB(70,255,0);leds[XY(x,3)]=CRGB(70,255,0);
  x=10; leds[XY(x,1)]=CRGB(70,255,0);leds[XY(x,2)]=CRGB(70,255,0);
  x=11; leds[XY(x,1)]=CRGB(70,255,0);
  x=11; leds[XY(x,2)]=CRGB(0,255,0);
  x=8; leds[XY(x,1)]=CRGB(162,255,196);
  x=12; leds[XY(x,0)]=CRGB(162,255,196);
  x=10; leds[XY(x,4)]=CRGB(133,0,0);leds[XY(x,5)]=CRGB(133,0,0);
  x=11; leds[XY(x,5)]=CRGB(255,0,0);
  x=12; leds[XY(x,5)]=CRGB(255,0,0);
  x=13; leds[XY(x,6)]=CRGB(255,0,0);
}

void hlavahada3() {
  int x=5; leds[XY(x,3)]=CRGB(0,80,0);
  x=6; leds[XY(x,3)]=CRGB(0,80,0);
  x=7; leds[XY(x,3)]=CRGB(0,80,0);leds[XY(x,6)]=CRGB(0,80,0);
  for(x=8;x<=11;x++){leds[XY(x,2)]=CRGB(0,80,0);leds[XY(x,6)]=CRGB(0,80,0);}
  x=12; leds[XY(x,3)]=CRGB(0,80,0);leds[XY(x,6)]=CRGB(0,80,0);
  x=13; leds[XY(x,4)]=CRGB(0,80,0);leds[XY(x,5)]=CRGB(0,80,0);
  x=8; leds[XY(x,4)]=CRGB(70,255,0);leds[XY(x,5)]=CRGB(70,255,0);
  x=9; leds[XY(x,4)]=CRGB(70,255,0);leds[XY(x,5)]=CRGB(70,255,0);
  x=10; leds[XY(x,3)]=CRGB(70,255,0);leds[XY(x,4)]=CRGB(70,255,0);leds[XY(x,5)]=CRGB(70,255,0);
  x=11; leds[XY(x,3)]=CRGB(70,255,0);leds[XY(x,4)]=CRGB(70,255,0);leds[XY(x,5)]=CRGB(70,255,0);
  x=12; leds[XY(x,4)]=CRGB(70,255,0);leds[XY(x,5)]=CRGB(70,255,0);
  x=5; leds[XY(x,4)]=CRGB(0,255,0);
  x=6; leds[XY(x,4)]=CRGB(0,255,0);
  x=7; leds[XY(x,4)]=CRGB(0,255,0);leds[XY(x,5)]=CRGB(0,255,0);
  x=8; leds[XY(x,3)]=CRGB(0,255,0);
  x=9; leds[XY(x,3)]=CRGB(162,255,196);
  x=13; leds[XY(x,3)]=CRGB(162,255,196);
}

void hlavahada4() {
  int x=5; leds[XY(x,4)]=CRGB(0,80,0);
  x=6; leds[XY(x,3)]=CRGB(0,80,0);
  x=7; leds[XY(x,3)]=CRGB(0,80,0);
  for(x=8;x<=11;x++){leds[XY(x,3)]=CRGB(0,80,0);leds[XY(x,7)]=CRGB(0,80,0);}
  x=12; leds[XY(x,4)]=CRGB(0,80,0);leds[XY(x,7)]=CRGB(0,80,0);
  x=13; leds[XY(x,5)]=CRGB(0,80,0);leds[XY(x,6)]=CRGB(0,80,0);
  x=8; leds[XY(x,5)]=CRGB(70,255,0);
  x=9; leds[XY(x,5)]=CRGB(70,255,0);leds[XY(x,6)]=CRGB(70,255,0);
  x=10; leds[XY(x,4)]=CRGB(70,255,0);leds[XY(x,5)]=CRGB(70,255,0);leds[XY(x,6)]=CRGB(70,255,0);
  x=11; leds[XY(x,4)]=CRGB(70,255,0);leds[XY(x,5)]=CRGB(70,255,0);leds[XY(x,6)]=CRGB(70,255,0);
  x=12; leds[XY(x,5)]=CRGB(70,255,0);leds[XY(x,6)]=CRGB(70,255,0);
  x=6; leds[XY(x,4)]=CRGB(0,255,0);
  x=7; leds[XY(x,4)]=CRGB(0,255,0);leds[XY(x,5)]=CRGB(0,255,0);leds[XY(x,6)]=CRGB(0,255,0);
  x=8; leds[XY(x,4)]=CRGB(0,255,0);leds[XY(x,6)]=CRGB(0,255,0);
  x=9; leds[XY(x,4)]=CRGB(162,255,196);
  x=13; leds[XY(x,4)]=CRGB(162,255,196);
}

void mainanimacehada() {
  for(int i=0;i<NUM_LEDS;i++) leds[i]=CRGB(0,15,15);
  if      (animacehada==0){telohada2();krkhada2();hlavahada4();animacehada++;}
  else if (animacehada==1){telohada2();krkhada2();hlavahada3();animacehada++;}
  else if (animacehada==2){telohada1();krkhada1();hlavahada1();animacehada++;}
  else if (animacehada==3){telohada1();krkhada1();hlavahada2();animacehada++;}
  else if (animacehada==4){telohada1();krkhada1();hlavahada1();animacehada++;}
  else if (animacehada==5){telohada2();krkhada2();hlavahada3();animacehada++;}
  else if (animacehada==6){telohada2();krkhada2();hlavahada4();animacehada++;}
  if(animacehada>=6) animacehada=0;
}

// ===================== MIKROFON ikona =====================

void animacemikrofon() {
  for(int i=0;i<NUM_LEDS;i++) leds[i]=CRGB(15,0,15);
  int x=0; leds[XY(x,7)]=CRGB(255,255,255);
  x=1; leds[XY(x,7)]=CRGB(255,255,255);leds[XY(x,6)]=CRGB(255,255,255);leds[XY(x,5)]=CRGB(255,255,255);leds[XY(x,4)]=CRGB(255,255,255);
  x=2; leds[XY(x,5)]=CRGB(255,255,255);
  x=4; leds[XY(x,4)]=CRGB(255,255,255);leds[XY(x,9)]=CRGB(255,255,255);leds[XY(x,10)]=CRGB(255,255,255);
  x=5; leds[XY(x,4)]=CRGB(255,255,255);leds[XY(x,3)]=CRGB(255,255,255);leds[XY(x,2)]=CRGB(255,255,255);leds[XY(x,1)]=CRGB(255,255,255);leds[XY(x,8)]=CRGB(255,255,255);leds[XY(x,9)]=CRGB(255,255,255);
  x=6; leds[XY(x,2)]=CRGB(255,255,255);leds[XY(x,7)]=CRGB(255,255,255);leds[XY(x,8)]=CRGB(255,255,255);
  x=7; leds[XY(x,6)]=CRGB(255,255,255);leds[XY(x,7)]=CRGB(255,255,255);
  x=10; leds[XY(x,2)]=CRGB(255,255,255);
  x=11; leds[XY(x,1)]=CRGB(255,255,255);leds[XY(x,2)]=CRGB(255,255,255);leds[XY(x,3)]=CRGB(255,255,255);
  x=12; leds[XY(x,2)]=CRGB(255,255,255);
  x=4; leds[XY(x,11)]=CRGB(130,130,130);
  x=5; leds[XY(x,11)]=CRGB(130,130,130);leds[XY(x,10)]=CRGB(130,130,130);
  x=6; leds[XY(x,11)]=CRGB(130,130,130);
  x=7; leds[XY(x,10)]=CRGB(130,130,130);
  x=8; for(int i=5;i<=9;i++) leds[XY(x,i)]=CRGB(130,130,130);
  x=9; leds[XY(x,8)]=CRGB(130,130,130);leds[XY(x,7)]=CRGB(130,130,130);leds[XY(x,6)]=CRGB(130,130,130);leds[XY(x,2)]=CRGB(130,130,130);
  x=10; leds[XY(x,7)]=CRGB(130,130,130);leds[XY(x,3)]=CRGB(130,130,130);leds[XY(x,1)]=CRGB(130,130,130);
  x=11; leds[XY(x,4)]=CRGB(130,130,130);leds[XY(x,0)]=CRGB(130,130,130);
  x=12; leds[XY(x,3)]=CRGB(130,130,130);leds[XY(x,1)]=CRGB(130,130,130);
  x=13; leds[XY(x,2)]=CRGB(130,130,130);
  x=3; for(int i=9;i<=13;i++) leds[XY(x,i)]=CRGB(80,80,80);
  x=4; leds[XY(x,14)]=CRGB(80,80,80);leds[XY(x,12)]=CRGB(80,80,80);leds[XY(x,8)]=CRGB(80,80,80);
  x=5; leds[XY(x,15)]=CRGB(80,80,80);leds[XY(x,12)]=CRGB(80,80,80);leds[XY(x,7)]=CRGB(80,80,80);
  x=6; leds[XY(x,15)]=CRGB(80,80,80);leds[XY(x,12)]=CRGB(80,80,80);leds[XY(x,10)]=CRGB(80,80,80);leds[XY(x,9)]=CRGB(80,80,80);leds[XY(x,6)]=CRGB(80,80,80);
  x=7; leds[XY(x,15)]=CRGB(80,80,80);leds[XY(x,11)]=CRGB(80,80,80);leds[XY(x,9)]=CRGB(80,80,80);leds[XY(x,8)]=CRGB(80,80,80);leds[XY(x,5)]=CRGB(80,80,80);
  x=8; leds[XY(x,14)]=CRGB(80,80,80);leds[XY(x,10)]=CRGB(80,80,80);for(int i=2;i<=4;i++) leds[XY(x,i)]=CRGB(80,80,80);
  x=9; leds[XY(x,13)]=CRGB(80,80,80);leds[XY(x,9)]=CRGB(80,80,80);leds[XY(x,5)]=CRGB(80,80,80);leds[XY(x,4)]=CRGB(80,80,80);leds[XY(x,3)]=CRGB(80,80,80);leds[XY(x,1)]=CRGB(80,80,80);
  x=10; leds[XY(x,12)]=CRGB(80,80,80);leds[XY(x,8)]=CRGB(80,80,80);leds[XY(x,6)]=CRGB(80,80,80);leds[XY(x,5)]=CRGB(80,80,80);leds[XY(x,4)]=CRGB(80,80,80);leds[XY(x,0)]=CRGB(80,80,80);
  x=11; leds[XY(x,11)]=CRGB(80,80,80);leds[XY(x,7)]=CRGB(80,80,80);leds[XY(x,6)]=CRGB(80,80,80);leds[XY(x,5)]=CRGB(80,80,80);
  x=12; leds[XY(x,11)]=CRGB(80,80,80);leds[XY(x,0)]=CRGB(80,80,80);for(int i=4;i<=7;i++) leds[XY(x,i)]=CRGB(80,80,80);
  x=13; leds[XY(x,11)]=CRGB(80,80,80);leds[XY(x,15)]=CRGB(80,80,80);leds[XY(x,1)]=CRGB(80,80,80);leds[XY(x,0)]=CRGB(80,80,80);for(int i=3;i<=7;i++) leds[XY(x,i)]=CRGB(80,80,80);
  x=14; for(int i=12;i<=14;i++) leds[XY(x,i)]=CRGB(80,80,80);for(int i=1;i<=6;i++) leds[XY(x,i)]=CRGB(80,80,80);
  x=15; for(int i=2;i<=5;i++) leds[XY(x,i)]=CRGB(80,80,80);
}

// ===================== TUŽKA ikona =====================

void tuzka() {
  for(int i=0;i<NUM_LEDS;i++) leds[i]=CRGB(0,15,15);
  int x=4; leds[XY(x,8)]=CRGB(255,180,120);leds[XY(x,9)]=CRGB(255,180,120);
  x=5; leds[XY(x,9)]=CRGB(255,180,120);leds[XY(x,10)]=CRGB(255,180,120);
  x=6; leds[XY(x,10)]=CRGB(255,180,120);
  leds[XY(4,10)]=CRGB(0,0,0);
  leds[XY(12,11)]=CRGB(0,0,0);leds[XY(13,11)]=CRGB(0,0,0);
  leds[XY(4,12)]=CRGB(0,0,0);leds[XY(5,12)]=CRGB(0,0,0);leds[XY(6,12)]=CRGB(0,0,0);leds[XY(7,12)]=CRGB(0,0,0);leds[XY(10,12)]=CRGB(0,0,0);leds[XY(11,12)]=CRGB(0,0,0);leds[XY(14,12)]=CRGB(0,0,0);
  leds[XY(8,13)]=CRGB(0,0,0);leds[XY(9,13)]=CRGB(0,0,0);
  x=4; leds[XY(x,7)]=CRGB(100,100,0);
  x=5; leds[XY(x,7)]=CRGB(100,100,0);leds[XY(x,6)]=CRGB(100,100,0);
  x=6; leds[XY(x,6)]=CRGB(100,100,0);leds[XY(x,5)]=CRGB(100,100,0);
  x=7; leds[XY(x,10)]=CRGB(100,100,0);leds[XY(x,9)]=CRGB(100,100,0);leds[XY(x,5)]=CRGB(100,100,0);leds[XY(x,4)]=CRGB(100,100,0);
  x=8; leds[XY(x,9)]=CRGB(100,100,0);leds[XY(x,8)]=CRGB(100,100,0);leds[XY(x,4)]=CRGB(100,100,0);leds[XY(x,3)]=CRGB(100,100,0);
  x=9; leds[XY(x,8)]=CRGB(100,100,0);leds[XY(x,7)]=CRGB(100,100,0);leds[XY(x,3)]=CRGB(100,100,0);
  x=10; leds[XY(x,6)]=CRGB(100,100,0);leds[XY(x,7)]=CRGB(100,100,0);
  x=11; leds[XY(x,6)]=CRGB(100,100,0);leds[XY(x,5)]=CRGB(100,100,0);
  x=5; leds[XY(x,8)]=CRGB(255,255,0);
  x=6; leds[XY(x,7)]=CRGB(255,255,0);leds[XY(x,8)]=CRGB(255,255,0);leds[XY(x,9)]=CRGB(255,255,0);
  x=7; leds[XY(x,6)]=CRGB(255,255,0);leds[XY(x,7)]=CRGB(255,255,0);leds[XY(x,8)]=CRGB(255,255,0);
  x=8; leds[XY(x,5)]=CRGB(255,255,0);leds[XY(x,6)]=CRGB(255,255,0);leds[XY(x,7)]=CRGB(255,255,0);
  x=9; leds[XY(x,4)]=CRGB(255,255,0);leds[XY(x,5)]=CRGB(255,255,0);leds[XY(x,6)]=CRGB(255,255,0);
  x=10; leds[XY(x,4)]=CRGB(255,255,0);leds[XY(x,5)]=CRGB(255,255,0);
  x=10; leds[XY(x,3)]=CRGB(255,255,255);
  x=11; leds[XY(x,2)]=CRGB(255,255,255);leds[XY(x,3)]=CRGB(255,255,255);leds[XY(x,4)]=CRGB(255,255,255);
  x=12; leds[XY(x,2)]=CRGB(255,255,255);leds[XY(x,3)]=CRGB(255,255,255);
  x=9; leds[XY(x,2)]=CRGB(80,80,80);
  x=10; leds[XY(x,1)]=CRGB(80,80,80);leds[XY(x,2)]=CRGB(80,80,80);
  x=11; leds[XY(x,1)]=CRGB(80,80,80);
  x=12; leds[XY(x,4)]=CRGB(80,80,80);leds[XY(x,5)]=CRGB(80,80,80);
  x=13; leds[XY(x,3)]=CRGB(80,80,80);leds[XY(x,4)]=CRGB(80,80,80);
  x=12; leds[XY(x,1)]=CRGB(255,0,0);
  x=13; leds[XY(x,1)]=CRGB(255,0,0);leds[XY(x,2)]=CRGB(255,0,0);
  x=11; leds[XY(x,0)]=CRGB(100,0,0);
  x=12; leds[XY(x,0)]=CRGB(100,0,0);
  x=14; leds[XY(x,2)]=CRGB(100,0,0);leds[XY(x,3)]=CRGB(100,0,0);
}

// ===================== PACMAN =====================

void pacman1() {
  for(int i=5;i<=10;i++){leds[XY(1,i)]=CRGB(255,255,0);leds[XY(14,i)]=CRGB(255,255,0);}
  for(int i=3;i<=12;i++){leds[XY(2,i)]=CRGB(255,255,0);leds[XY(13,i)]=CRGB(255,255,0);}
  for(int i=2;i<=13;i++){leds[XY(3,i)]=CRGB(255,255,0);leds[XY(4,i)]=CRGB(255,255,0);leds[XY(11,i)]=CRGB(255,255,0);leds[XY(12,i)]=CRGB(255,255,0);}
  for(int i=1;i<=14;i++){leds[XY(5,i)]=CRGB(255,255,0);leds[XY(6,i)]=CRGB(255,255,0);leds[XY(7,i)]=CRGB(255,255,0);leds[XY(8,i)]=CRGB(255,255,0);leds[XY(9,i)]=CRGB(255,255,0);leds[XY(10,i)]=CRGB(255,255,0);}
}

void pacman2() {
  for(int i=5;i<=10;i++) leds[XY(1,i)]=CRGB(255,255,0);
  for(int i=3;i<=12;i++) leds[XY(2,i)]=CRGB(255,255,0);
  for(int i=2;i<=13;i++){leds[XY(3,i)]=CRGB(255,255,0);leds[XY(4,i)]=CRGB(255,255,0);}
  for(int i=1;i<=14;i++) leds[XY(5,i)]=CRGB(255,255,0);
  for(int x=6;x<=7;x++){for(int i=1;i<=6;i++) leds[XY(x,i)]=CRGB(255,255,0);for(int i=8;i<=14;i++) leds[XY(x,i)]=CRGB(255,255,0);}
  int p=8; for(int i=1;i<=5;i++) leds[XY(p,i)]=CRGB(255,255,0);for(int i=9;i<=14;i++) leds[XY(p,i)]=CRGB(255,255,0);
  p=9;  for(int i=1;i<=5;i++) leds[XY(p,i)]=CRGB(255,255,0);for(int i=9;i<=14;i++) leds[XY(p,i)]=CRGB(255,255,0);
  p=10; for(int i=1;i<=5;i++) leds[XY(p,i)]=CRGB(255,255,0);for(int i=9;i<=14;i++) leds[XY(p,i)]=CRGB(255,255,0);
  p=11; for(int i=2;i<=5;i++) leds[XY(p,i)]=CRGB(255,255,0);for(int i=9;i<=13;i++) leds[XY(p,i)]=CRGB(255,255,0);
  p=12; for(int i=2;i<=5;i++) leds[XY(p,i)]=CRGB(255,255,0);for(int i=9;i<=13;i++) leds[XY(p,i)]=CRGB(255,255,0);
  p=13; for(int i=3;i<=4;i++) leds[XY(p,i)]=CRGB(255,255,0);for(int i=10;i<=12;i++) leds[XY(p,i)]=CRGB(255,255,0);
  p=14; leds[XY(p,4)]=CRGB(255,255,0);leds[XY(p,10)]=CRGB(255,255,0);
}

void pacman3() {
  for(int i=5;i<=10;i++) leds[XY(1,i)]=CRGB(255,255,0);
  for(int i=3;i<=12;i++) leds[XY(2,i)]=CRGB(255,255,0);
  for(int i=2;i<=13;i++){leds[XY(3,i)]=CRGB(255,255,0);leds[XY(4,i)]=CRGB(255,255,0);}
  for(int i=1;i<=14;i++) leds[XY(5,i)]=CRGB(255,255,0);
  for(int x=6;x<=7;x++){for(int i=1;i<=6;i++) leds[XY(x,i)]=CRGB(255,255,0);for(int i=9;i<=14;i++) leds[XY(x,i)]=CRGB(255,255,0);}
  int p=8;  for(int i=1;i<=5;i++) leds[XY(p,i)]=CRGB(255,255,0);for(int i=10;i<=14;i++) leds[XY(p,i)]=CRGB(255,255,0);
  p=9;  for(int i=1;i<=5;i++) leds[XY(p,i)]=CRGB(255,255,0);for(int i=10;i<=14;i++) leds[XY(p,i)]=CRGB(255,255,0);
  p=10; for(int i=1;i<=4;i++) leds[XY(p,i)]=CRGB(255,255,0);for(int i=11;i<=14;i++) leds[XY(p,i)]=CRGB(255,255,0);
  p=11; for(int i=2;i<=4;i++) leds[XY(p,i)]=CRGB(255,255,0);for(int i=11;i<=13;i++) leds[XY(p,i)]=CRGB(255,255,0);
  p=12; for(int i=2;i<=3;i++) leds[XY(p,i)]=CRGB(255,255,0);for(int i=12;i<=13;i++) leds[XY(p,i)]=CRGB(255,255,0);
  p=13; leds[XY(p,3)]=CRGB(255,255,0);leds[XY(p,12)]=CRGB(255,255,0);
}

void pacman() {
  for(int i=0;i<NUM_LEDS;i++) leds[i]=CRGB(0,0,15);
  if      (poradianimace==0){pacman1();poradianimace++;}
  else if (poradianimace==1){pacman2();poradianimace++;}
  else if (poradianimace==2){pacman3();poradianimace++;}
  else if (poradianimace==3){pacman2();poradianimace++;}
  if(poradianimace>3) poradianimace=0;
}

// ===================== WIFI JOIN obrazovka =====================

void joinwifi() {
  vykreslicislo(1,5,1,255,255,255); vykreslicislo(5,5,0,255,255,255);
  vykreslicislo(7,5,1,255,255,255); vykreslicislo(9,5,1,255,255,255);
  vykreslicislo(12,5,1,255,255,255); vykreslicislo(5,13,1,255,255,255);
  vykreslicislo(7,13,1,255,255,255); vykreslicislo(11,13,1,255,255,255);
  int x=1;
  for(int i=4;i<=5;i++) leds[XY(x,i)]=CRGB(255,255,255);
  for(int i=9;i<=11;i++) leds[XY(x,i)]=CRGB(255,255,255);
  x=2; for(int i=12;i<=13;i++) leds[XY(x,i)]=CRGB(255,255,255);leds[XY(x,5)]=CRGB(255,255,255);
  x=3; for(int i=10;i<=11;i++) leds[XY(x,i)]=CRGB(255,255,255);
  x=4; for(int i=12;i<=13;i++) leds[XY(x,i)]=CRGB(255,255,255);
  x=5; for(int i=9;i<=11;i++) leds[XY(x,i)]=CRGB(255,255,255);
  x=10; leds[XY(x,9)]=CRGB(255,255,255);leds[XY(x,11)]=CRGB(255,255,255);
  x=11; leds[XY(x,9)]=CRGB(255,255,255);leds[XY(x,11)]=CRGB(255,255,255);
  x=12; leds[XY(x,2)]=CRGB(255,255,255);
  x=13; leds[XY(x,3)]=CRGB(255,255,255);leds[XY(x,4)]=CRGB(255,255,255);
}
