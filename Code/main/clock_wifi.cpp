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

void setRTCfromNTP() {
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    DateTime nowRTC(1900 + timeinfo.tm_year, 1 + timeinfo.tm_mon,
                    timeinfo.tm_mday, timeinfo.tm_hour,
                    timeinfo.tm_min, timeinfo.tm_sec);
    rtc.adjust(nowRTC);
  }
}

void manageWiFiNonBlocking() {
  unsigned long now = millis();
  const unsigned long wifiCheckInterval   = 10000;
  const unsigned long wifiConnectTimeout  = 10000;

  if (WiFi.status() != WL_CONNECTED) {
    if (!wifiConnecting && (now - lastWiFiCheck >= wifiCheckInterval)) {
      WiFi.begin(ssid, password);
      wifiConnecting = true;
      wifiConnectStart = now;
      lastWiFiCheck = now;
    }
    if (wifiConnecting && (now - wifiConnectStart) > wifiConnectTimeout) {
      WiFi.disconnect();
      delay(50);
      WiFi.begin(ssid, password);
      wifiConnectStart = now;
    }
  } else {
    if (wifiConnecting) {
      wifiConnecting = false;
      configTzTime("CET-1CEST,M3.5.0/2,M10.5.0/3", ntpServer);
      setRTCfromNTP();
    }
  }
}

void nacticaszwifi() {
  const unsigned long timeInterval = 1000;
  unsigned long now = millis();
  uint8_t R = 255, G = 255, B = 255;

  manageWiFiNonBlocking();

  if (now - lastTimePrint < timeInterval) return;
  lastTimePrint = now;

  bool casOK = false;
  if (WiFi.status() == WL_CONNECTED) {
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      hodina = timeinfo.tm_hour; minuta = timeinfo.tm_min;
      hodina1 = hodina / 10; hodina2 = hodina % 10;
      minuta1 = minuta / 10; minuta2 = minuta % 10;
      DateTime nowRTC(1900 + timeinfo.tm_year, 1 + timeinfo.tm_mon,
                      timeinfo.tm_mday, timeinfo.tm_hour,
                      timeinfo.tm_min, timeinfo.tm_sec);
      rtc.adjust(nowRTC);
      casOK = true;
    }
  }
  if (!casOK) {
    DateTime nowRTC = rtc.now();
    hodina = nowRTC.hour(); minuta = nowRTC.minute();
    hodina1 = hodina / 10; hodina2 = hodina % 10;
    minuta1 = minuta / 10; minuta2 = minuta % 10;
  }

  FastLED.clear();

  EVERY_N_MILLISECONDS(1000) {
    vykreslicislo(0,  6, hodina1, R, G, B);
    vykreslicislo(4,  6, hodina2, R, G, B);
    vykreslicislo(8,  6, minuta1, R, G, B);
    vykreslicislo(12, 6, minuta2, R, G, B);

    if (dvojtecka) {
      leds[XY(7,3)] = CHSV(0,0,128);
      leds[XY(7,5)] = CHSV(0,0,128);
      dvojtecka = false;
    } else {
      leds[XY(7,3)] = CRGB::Black;
      leds[XY(7,5)] = CRGB::Black;
      dvojtecka = true;
    }

    vlhkost = dht.readHumidity();
    teplota = dht.readTemperature();
    if (!isnan(vlhkost) && !isnan(teplota)) {
      vlhkost1 = vlhkost / 10; vlhkost2 = vlhkost % 10;
      teplota1 = teplota / 10; teplota2 = teplota % 10;
    }

    if (pocetsekundDHT < 5) {
      for (int i = 128; i <= 255; i++) leds[i] = CRGB::Black;
      vykreslicislo(0, 13, teplota1, R, G, B);
      vykreslicislo(4, 13, teplota2, R, G, B);
      leds[XY(8,9)]  = CRGB(R,G,B); leds[XY(10,9)]  = CRGB(R,G,B);
      leds[XY(11,9)] = CRGB(R,G,B); leds[XY(10,10)] = CRGB(R,G,B);
      leds[XY(10,11)]= CRGB(R,G,B); leds[XY(11,11)] = CRGB(R,G,B);
    }
    if (pocetsekundDHT >= 5 && pocetsekundDHT < 10) {
      for (int i = 128; i <= 255; i++) leds[i] = CRGB::Black;
      vykreslicislo(0, 13, vlhkost1, R, G, B);
      vykreslicislo(4, 13, vlhkost2, R, G, B);
      leds[XY(8,13)] = CRGB(R,G,B); leds[XY(9,12)]  = CRGB(R,G,B);
      leds[XY(10,11)]= CRGB(R,G,B); leds[XY(11,10)] = CRGB(R,G,B);
      leds[XY(12,9)] = CRGB(R,G,B); leds[XY(8,10)]  = CRGB(R,G,B);
      leds[XY(8,9)]  = CRGB(R,G,B); leds[XY(9,9)]   = CRGB(R,G,B);
      leds[XY(12,12)]= CRGB(R,G,B); leds[XY(12,13)] = CRGB(R,G,B);
      leds[XY(11,13)]= CRGB(R,G,B); leds[XY(9,10)]  = CRGB(R,G,B);
      leds[XY(11,12)]= CRGB(R,G,B);
    }

    pocetsekundDHT++;
    if (pocetsekundDHT >= 10) pocetsekundDHT = 0;

    leds[XY(0,0)] = (WiFi.status() == WL_CONNECTED) ? CRGB(0,255,0) : CRGB(255,0,0);

    FastLED.show();
  }
}
