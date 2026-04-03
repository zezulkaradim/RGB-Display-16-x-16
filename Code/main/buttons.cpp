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

void clickright() {
  if (mainmode < nummainmode && setmainmode) mainmode++;
  if (modeanimace < nummodeanimace && setmodeanimace) modeanimace++;
  if (setmodehad && !starthada) { starthada = true; doprava = true; }
  if (setmodehad && starthada && !doleva) {
    dolu = false; nahoru = false; doprava = true; doleva = false;
  }
}

void clickleft() {
  if (mainmode > 0 && setmainmode) mainmode--;
  if (modeanimace > 0 && setmodeanimace) modeanimace--;
  if (setmodehad && !starthada) { starthada = true; doleva = true; }
  if (setmodehad && starthada && !doprava) {
    dolu = false; nahoru = false; doprava = false; doleva = true;
  }
}

void clickup() {
  if (setmodehad && !starthada) { starthada = true; nahoru = true; }
  if (setmodehad && starthada && !dolu) {
    dolu = false; nahoru = true; doprava = false; doleva = false;
  }
}

void clickdown() {
  if (setmodehad && !starthada) { starthada = true; dolu = true; }
  if (setmodehad && starthada && !nahoru) {
    dolu = true; nahoru = false; doprava = false; doleva = false;
  }
}

void clickplay() {
  if (mainmode == 1 && setmainmode) {
    setmainmode = false; setmodeanimace = true; modeanimace = 0;
  }
  if (mainmode == 2 && setmainmode) {
    setmainmode = false; setmodehad = true; starthada = false;
  }
  if (setyoulosthad && setmodehad) {
    setyoulosthad = false; starthada = false;
    xjablko = 0; yjablko = 0;
    nahoru = false; dolu = false; doprava = false; doleva = false;
    delkahada = 0; pocetbodu = 0;
    tisicehad = 0; stovkyhad = 0; desitkyhad = 0; jednotkyhad = 0;
    dotekzdi = false; blikyoulost = false;
    for (int i = 0; i < 255; i++) { had[i].x = 0; had[i].y = 0; }
    hadpredchozimillis = 0;
  }
  if (mainmode == 3 && setmainmode) {
    setmainmode = false; setmodevumetr = true;
  }
  if (mainmode == 4 && setmainmode) {
    setmainmode = false; setmodekresleni = true;
  }
}

void clickhome() {
  if (setmodeanimace) {
    setmodeanimace = false; setmainmode = true; modeanimace = 0; mainmode = 1;
  }
  if (setmodehad) {
    setmodehad = false; setmainmode = true; mainmode = 2;
    setyoulosthad = false; starthada = false;
    xjablko = 0; yjablko = 0;
    nahoru = false; dolu = false; doprava = false; doleva = false;
    delkahada = 0; pocetbodu = 0;
    tisicehad = 0; stovkyhad = 0; desitkyhad = 0; jednotkyhad = 0;
    dotekzdi = false; blikyoulost = false;
    for (int i = 0; i < 255; i++) { had[i].x = 0; had[i].y = 0; }
    hadpredchozimillis = 0;
  }
  if (setmodevumetr) {
    setmodevumetr = false; setmainmode = true; mainmode = 3;
  }
  if (setmodekresleni) {
    setmodekresleni = false; setmainmode = true; mainmode = 4;
    stopKresleni();
  }
}
