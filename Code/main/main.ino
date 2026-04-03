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

// ===================== Globální proměnné =====================

CRGB leds[NUM_LEDS];

DHT dht(DHTPIN, DHTTYPE);
int teplota=0, vlhkost=0, teplota1=0, vlhkost1=0, teplota2=0, vlhkost2=0;
uint8_t pocetsekundDHT = 0;

const char* ssid     = "Xiaomi 13T Pro";
const char* password = "lupen_smrdi_123";
unsigned long lastTimePrint  = 0;
unsigned long lastWiFiCheck  = 0;
bool wifiConnecting          = false;
unsigned long wifiConnectStart = 0;

const char* ntpServer       = "pool.ntp.org";
const long  gmtOffset_sec   = 2 * 3600;
const int   daylightOffset_sec = 0;
int hodina=0, minuta=0, hodina1=0, hodina2=0, minuta1=0, minuta2=0;
bool dvojtecka = true;
RTC_DS3231 rtc;

int mainmode = 0, nummainmode = 4;
bool setmainmode = true;
int poradianimace = 0, modeanimace = 0, nummodeanimace = 3;
bool setmodeanimace = false, prvnipruchodpacmana = false;

strukturahada had[256];
bool starthada=false, setmodehad=false;
uint8_t xjablko=0, yjablko=0;
bool nahoru=false, dolu=false, doprava=false, doleva=false;
uint8_t delkahada=0;
int pocetbodu=0, tisicehad=0, stovkyhad=0, desitkyhad=0, jednotkyhad=0;
bool setyoulosthad=false, dotekzdi=false, blikyoulost=false;
unsigned long hadpredchozimillis=0, hadmainlastmillis=0;
int animacehada=0;
bool prvnipruchodhada=false;

uint8_t raketaX=0, raketaY=0, hueraketa=0, pozicerakety=HEIGHT;
bool raketaleti=false;
uint8_t fazeexploze=0;
unsigned long posledniexploze=0, casFadeStart=0, posledniRaketa=0, posledniPohyb=0;
bool fadeActive=false;
CRGB ledsohnastroj[NUM_LEDS];
uint8_t heat[WIDTH][HEIGHT];

uint8_t barHeights[16], peak[16], prevFFTValue[16];
bool setmodevumetr=false;

bool setmodekresleni=false;
WebServer server(80);
CRGB ledBuffer[WIDTH][HEIGHT];
bool needsShow=false, kresleniActive=false;

OneButton buttonUp(BUTTON_ARROW_UP,    false, false);
OneButton buttonRight(BUTTON_ARROW_RIGHT, false, false);
OneButton buttonDown(BUTTON_ARROW_DOWN,  false, false);
OneButton buttonLeft(BUTTON_ARROW_LEFT,  false, false);
OneButton buttonHome(BUTTON_HOME,        false, false);
OneButton buttonPlay(BUTTON_PLAY,        false, false);

// ===================== Navigace menu =====================

void mainmodes() {
  if (mainmode==1 && setmainmode) {
    if (!prvnipruchodpacmana) { FastLED.clear(); pacman(); FastLED.show(); prvnipruchodpacmana=true; }
    EVERY_N_MILLISECONDS(200) { FastLED.clear(); pacman(); FastLED.show(); }
  } else if (mainmode==2 && setmainmode) {
    if (!prvnipruchodhada) { FastLED.clear(); mainanimacehada(); FastLED.show(); prvnipruchodhada=true; }
    EVERY_N_MILLISECONDS(800) { FastLED.clear(); mainanimacehada(); FastLED.show(); }
  } else if (mainmode==3 && setmainmode) {
    FastLED.clear(); animacemikrofon(); FastLED.show();
  } else if (mainmode==4 && setmainmode) {
    FastLED.clear(); tuzka(); FastLED.show();
  }
}

// ===================== Setup =====================

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  FastLED.addLeds<CHIPSET, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, AMPS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear(); delay(200); FastLED.show(); delay(200);

  pinMode(BUTTON_ARROW_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_ARROW_LEFT,  INPUT_PULLUP);
  pinMode(BUTTON_ARROW_UP,    INPUT_PULLUP);
  pinMode(BUTTON_ARROW_DOWN,  INPUT_PULLUP);
  pinMode(BUTTON_HOME,        INPUT_PULLUP);
  pinMode(BUTTON_PLAY,        INPUT_PULLUP);

  buttonUp.attachClick(clickup);
  buttonRight.attachClick(clickright);
  buttonDown.attachClick(clickdown);
  buttonLeft.attachClick(clickleft);
  buttonPlay.attachClick(clickplay);
  buttonHome.attachClick(clickhome);

  memset(heat, 0, sizeof(heat));

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  wifiConnecting = true;
  wifiConnectStart = millis();
  lastWiFiCheck = millis();

  if (!rtc.begin()) Serial.println("RTC nenalezen.");
  if (rtc.lostPower()) Serial.println("RTC bez casu.");

  dht.begin();
  randomSeed(esp_random());
  setupAudio();
}

// ===================== Loop =====================

void loop() {
  buttonRight.tick(); buttonLeft.tick();
  buttonPlay.tick();  buttonHome.tick();
  buttonDown.tick();  buttonUp.tick();

  if      (mainmode==0 && setmainmode)            nacticaszwifi();
  else if (modeanimace==0 && setmodeanimace)      rainbowwave();
  else if (modeanimace==1 && setmodeanimace)      rainbowcycle();
  else if (modeanimace==2 && setmodeanimace)      rachejtle();
  else if (modeanimace==3 && setmodeanimace)      Fire2D(50, 90, 120);
  else if (setmodehad    && !setmainmode)         snake();
  else if (setmodevumetr && !setmainmode)         vumetr();
  else if (setmodekresleni && !setmainmode)       kresleni();
  else                                            mainmodes();
}
