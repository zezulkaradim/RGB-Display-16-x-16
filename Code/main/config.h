#pragma once

#include <FastLED.h>
#include <OneButton.h>
#include <WiFi.h>
#include "time.h"
#include "DHT.h"
#include "RTClib.h"
#include <arduinoFFT.h>
#include <driver/i2s.h>
#include "audio_reactive.h"
#include <WebServer.h>
#include <ArduinoJson.h>

// --- Display ---
#define WIDTH 16
#define HEIGHT 16
#define NUM_LEDS (WIDTH * HEIGHT)
#define DATA_PIN 5
#define COLOR_ORDER GRB
#define CHIPSET WS2812B
#define BRIGHTNESS 200
#define VOLTS 5
#define AMPS 14000
extern CRGB leds[NUM_LEDS];

// --- Tlačítka ---
#define BUTTON_ARROW_UP    25
#define BUTTON_ARROW_RIGHT 26
#define BUTTON_ARROW_DOWN  27
#define BUTTON_ARROW_LEFT  14
#define BUTTON_HOME        33
#define BUTTON_PLAY        13

// --- DHT11 ---
#define DHTPIN 4
#define DHTTYPE DHT11
extern DHT dht;
extern int teplota, vlhkost;
extern int teplota1, vlhkost1;
extern int teplota2, vlhkost2;
extern uint8_t pocetsekundDHT;

// --- Wi-Fi ---
extern const char* ssid;
extern const char* password;
extern unsigned long lastTimePrint;
extern unsigned long lastWiFiCheck;
extern bool wifiConnecting;
extern unsigned long wifiConnectStart;

// --- Čas / RTC ---
extern const char* ntpServer;
extern const long gmtOffset_sec;
extern const int daylightOffset_sec;
extern int hodina, minuta;
extern int hodina1, hodina2, minuta1, minuta2;
extern bool dvojtecka;
extern RTC_DS3231 rtc;

// --- Navigace / módy ---
extern int mainmode;
extern int nummainmode;
extern bool setmainmode;
extern int poradianimace;
extern int modeanimace;
extern int nummodeanimace;
extern bool setmodeanimace;
extern bool prvnipruchodpacmana;

// --- Had ---
struct strukturahada { uint8_t x; uint8_t y; };
extern strukturahada had[256];
extern bool starthada, setmodehad;
extern uint8_t xjablko, yjablko;
extern bool nahoru, dolu, doprava, doleva;
extern uint8_t delkahada;
extern int pocetbodu;
extern int tisicehad, stovkyhad, desitkyhad, jednotkyhad;
extern bool setyoulosthad, dotekzdi, blikyoulost;
extern unsigned long hadpredchozimillis, hadmainlastmillis;
extern int animacehada;
extern bool prvnipruchodhada;

// --- Rachejtle ---
extern uint8_t raketaX, raketaY, hueraketa, pozicerakety;
extern bool raketaleti;
extern uint8_t fazeexploze;
extern unsigned long posledniexploze, casFadeStart, posledniRaketa, posledniPohyb;
extern bool fadeActive;
extern CRGB ledsohnastroj[NUM_LEDS];

// --- Oheň ---
extern uint8_t heat[WIDTH][HEIGHT];

// --- VU metr ---
extern uint8_t barHeights[16];
extern uint8_t peak[16];
extern uint8_t prevFFTValue[16];
extern bool setmodevumetr;

// --- Kreslení ---
extern bool setmodekresleni;
extern WebServer server;
extern CRGB ledBuffer[WIDTH][HEIGHT];
extern bool needsShow, kresleniActive;
