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

// definice displaye

#define WIDTH 16
#define HEIGHT 16
#define NUM_LEDS (WIDTH * HEIGHT)
#define DATA_PIN 5
#define COLOR_ORDER GRB
#define CHIPSET WS2812B
#define BRIGHTNESS 200
#define VOLTS 5
#define AMPS 14000
 CRGB leds[NUM_LEDS];



// --- Definice všech tlačítek ---
#define BUTTON_ARROW_UP    25//1// hnedobila
#define BUTTON_ARROW_RIGHT 26//2// hneda
#define BUTTON_ARROW_DOWN  27//42// orandzovobila
#define BUTTON_ARROW_LEFT  14//41// modrobila 
#define BUTTON_HOME        33//12//7// zelenobila
#define BUTTON_PLAY        13//6// zelena

// definice teploty a vlhkosti dht11
#define DHTPIN 4  
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE); 
int teplota = 0;
int vlhkost = 0;
int teplota1 = 0;
int vlhkost1 = 0;
int teplota2 = 0;
int vlhkost2 = 0;
uint8_t pocetsekundDHT=0;



// definice onebutton tlacitek
OneButton buttonUp(BUTTON_ARROW_UP, false, false);
OneButton buttonRight(BUTTON_ARROW_RIGHT, false, false);
OneButton buttonDown(BUTTON_ARROW_DOWN, false, false);
OneButton buttonLeft(BUTTON_ARROW_LEFT, false, false);
OneButton buttonHome(BUTTON_HOME, false, false);
OneButton buttonPlay(BUTTON_PLAY, false, false);




// definice mainmodu
int mainmode = 0;   
int nummainmode = 4;
bool setmainmode = true;

// definice modu animace
int poradianimace = 0;
int modeanimace = 0;   
int nummodeanimace = 3;
bool setmodeanimace = false;
bool prvnipruchodpacmana = false;


// promenne pro rachejtle
uint8_t raketaX = 0;
uint8_t raketaY = 0;
uint8_t hueraketa = 0;
uint8_t pozicerakety = HEIGHT;
bool raketaleti = false;
uint8_t fazeexploze = 0;
unsigned long posledniexploze= 0;
unsigned long casFadeStart = 0;
//unsigned long lastFadeStep = 0;  
bool fadeActive = false;
unsigned long posledniRaketa = 0;  
unsigned long posledniPohyb = 0;   
CRGB ledsohnastroj[NUM_LEDS];

// promenne pro ohen
uint8_t heat[WIDTH][HEIGHT];

//promenne pro wifi
const char* ssid = "Xiaomi 13T Pro";
const char* password = "lupen_smrdi_123";

unsigned long lastTimePrint = 0; // pro non-blocking tisk času
const unsigned long timeInterval = 1000; // interval tisku v ms
unsigned long lastWiFiCheck = 0;      // pro non-blocking kontrolu Wi-Fi
const unsigned long wifiCheckInterval = 10000; // interval opětovného připojení v ms
bool wifiConnecting = false;
unsigned long wifiConnectStart = 0;
const unsigned long wifiConnectTimeout = 10000;

//promenne pro cas
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 2 * 3600;  // +2 hodinainy pro ČR
const int daylightOffset_sec = 0;   //  0 zimní čas , 3600 letníčas 
int hodina = 0;
int minuta = 0;
int hodina1 = 0;// první číslice hodinain → 1
int hodina2 = 0;  // druhá číslice hodinain → 5
int minuta1 = 0; // první číslice minutaut → 3
int minuta2 = 0;  // druhá číslice minutaut → 0
bool dvojtecka = true;
//rtc
RTC_DS3231 rtc;

//promenne pro hada
struct  strukturahada{
  uint8_t x;
  uint8_t y;
};
strukturahada had[256];
bool starthada = false;
bool setmodehad = false;
uint8_t xjablko = 0;
uint8_t yjablko = 0;
bool nahoru= false;
bool dolu= false;
bool doprava= false;
bool doleva= false;
uint8_t delkahada =0;
int pocetbodu = 0;
int tisicehad = 0;
int stovkyhad = 0;
int desitkyhad =0;
int jednotkyhad =0;
bool setyoulosthad = false;
bool dotekzdi = false;
bool blikyoulost= false;
static unsigned long hadpredchozimillis= 0;
static unsigned long hadmainlastmillis = 0;
int animacehada =0;
bool prvnipruchodhada = false;

//FFT VU metr
uint8_t barHeights[16];
uint8_t peak[16];
uint8_t prevFFTValue[16];
bool setmodevumetr=false;

//kresleni
bool setmodekresleni=false;
WebServer server(80);
CRGB ledBuffer[WIDTH][HEIGHT]; // Buffer pro všechny LED
bool needsShow = false;
bool kresleniActive = false;

//===========================================================================================================================XY
// Převod (x,y) → index
uint16_t XY(uint8_t x, uint8_t y) {
  if (y % 2 == 0) {
    return (y * WIDTH) + x;  
  } 
  else {
    return (y * WIDTH) + (WIDTH - 1 - x);  
  }
}
//===========================================================================================================================vykresli cislo
void vykreslicislo(uint8_t X, uint8_t Y, uint8_t cislo, uint8_t R,uint8_t G, uint8_t B){
  if(cislo == 0){
    leds[XY(X,Y)] = CRGB(R, G, B);
    leds[XY(X,Y-1)] = CRGB(R, G, B);
    leds[XY(X,Y-2)] = CRGB(R, G, B);
    leds[XY(X,Y-3)] = CRGB(R, G, B);
    leds[XY(X,Y-4)] = CRGB(R, G, B);
    leds[XY(X+1,Y-4)] = CRGB(R, G, B);
    leds[XY(X+1,Y)] = CRGB(R, G, B);
    leds[XY(X+2,Y)] = CRGB(R, G, B);
    leds[XY(X+2,Y-1)] = CRGB(R, G, B);
    leds[XY(X+2,Y-2)] = CRGB(R, G, B);
    leds[XY(X+2,Y-3)] = CRGB(R, G, B);
    leds[XY(X+2,Y-4)] = CRGB(R, G, B);
  }
  if(cislo == 1){
    leds[XY(X+2,Y)] = CRGB(R, G, B);
    leds[XY(X+2,Y-1)] = CRGB(R, G, B);
    leds[XY(X+2,Y-2)] = CRGB(R, G, B);
    leds[XY(X+2,Y-3)] = CRGB(R, G, B);
    leds[XY(X+2,Y-4)] = CRGB(R, G, B);
  }
  if(cislo == 2){
    leds[XY(X,Y)] = CRGB(R, G, B);
    leds[XY(X,Y-1)] = CRGB(R, G, B);
    leds[XY(X,Y-2)] = CRGB(R, G, B);
    leds[XY(X,Y-4)] = CRGB(R, G, B);
    leds[XY(X+1,Y)] = CRGB(R, G, B);
    leds[XY(X+1,Y-2)] = CRGB(R, G, B);
    leds[XY(X+1,Y-4)] = CRGB(R, G, B);
    leds[XY(X+2,Y)] = CRGB(R, G, B);
    leds[XY(X+2,Y-2)] = CRGB(R, G, B);
    leds[XY(X+2,Y-3)] = CRGB(R, G, B);
    leds[XY(X+2,Y-4)] = CRGB(R, G, B);
  }
  if(cislo == 3){
    leds[XY(X+2,Y)] = CRGB(R, G, B);
    leds[XY(X+2,Y-1)] = CRGB(R, G, B);
    leds[XY(X+2,Y-2)] = CRGB(R, G, B);
    leds[XY(X+2,Y-3)] = CRGB(R, G, B);
    leds[XY(X+2,Y-4)] = CRGB(R, G, B);
    leds[XY(X,Y)] = CRGB(R, G, B);
    leds[XY(X,Y-2)] = CRGB(R, G, B);
    leds[XY(X,Y-4)] = CRGB(R, G, B);
    leds[XY(X+1,Y)] = CRGB(R, G, B);
    leds[XY(X+1,Y-2)] = CRGB(R, G, B);
    leds[XY(X+1,Y-4)] = CRGB(R, G, B);
  }
  if(cislo == 4){
    leds[XY(X+2,Y)] = CRGB(R, G, B);
    leds[XY(X+2,Y-1)] = CRGB(R, G, B);
    leds[XY(X+2,Y-2)] = CRGB(R, G, B);
    leds[XY(X+2,Y-3)] = CRGB(R, G, B);
    leds[XY(X+2,Y-4)] = CRGB(R, G, B);
    leds[XY(X,Y-2)] = CRGB(R, G, B);
    leds[XY(X,Y-3)] = CRGB(R, G, B);
    leds[XY(X,Y-4)] = CRGB(R, G, B);
    leds[XY(X+1,Y-2)] = CRGB(R, G, B);
  }
  if(cislo == 5){
    leds[XY(X,Y)] = CRGB(R, G, B);
    leds[XY(X,Y-2)] = CRGB(R, G, B);
    leds[XY(X,Y-3)] = CRGB(R, G, B);
    leds[XY(X,Y-4)] = CRGB(R, G, B);
    leds[XY(X+1,Y)] = CRGB(R, G, B);
    leds[XY(X+1,Y-2)] = CRGB(R, G, B);
    leds[XY(X+1,Y-4)] = CRGB(R, G, B);
    leds[XY(X+2,Y)] = CRGB(R, G, B);
    leds[XY(X+2,Y-1)] = CRGB(R, G, B);
    leds[XY(X+2,Y-2)] = CRGB(R, G, B);
    leds[XY(X+2,Y-4)] = CRGB(R, G, B);
  }
  if(cislo == 6){
    leds[XY(X,Y)] = CRGB(R, G, B);
    leds[XY(X,Y-1)] = CRGB(R, G, B);
    leds[XY(X,Y-2)] = CRGB(R, G, B);
    leds[XY(X,Y-3)] = CRGB(R, G, B);
    leds[XY(X,Y-4)] = CRGB(R, G, B);
    leds[XY(X+1,Y)] = CRGB(R, G, B);
    leds[XY(X+1,Y-2)] = CRGB(R, G, B);
    leds[XY(X+1,Y-4)] = CRGB(R, G, B);
    leds[XY(X+2,Y)] = CRGB(R, G, B);
    leds[XY(X+2,Y-1)] = CRGB(R, G, B);
    leds[XY(X+2,Y-2)] = CRGB(R, G, B);
    leds[XY(X+2,Y-4)] = CRGB(R, G, B);
  }
  if(cislo == 7){
    leds[XY(X+2,Y)] = CRGB(R, G, B);
    leds[XY(X+2,Y-1)] = CRGB(R, G, B);
    leds[XY(X+2,Y-2)] = CRGB(R, G, B);
    leds[XY(X+2,Y-3)] = CRGB(R, G, B);
    leds[XY(X+2,Y-4)] = CRGB(R, G, B);
    leds[XY(X,Y-4)] = CRGB(R, G, B);
    leds[XY(X+1,Y-4)] = CRGB(R, G, B);
  }
  if(cislo == 8){
    leds[XY(X,Y)] = CRGB(R, G, B);
    leds[XY(X,Y-1)] = CRGB(R, G, B);
    leds[XY(X,Y-2)] = CRGB(R, G, B);
    leds[XY(X,Y-3)] = CRGB(R, G, B);
    leds[XY(X,Y-4)] = CRGB(R, G, B);
    leds[XY(X+1,Y)] = CRGB(R, G, B);
    leds[XY(X+1,Y-2)] = CRGB(R, G, B);
    leds[XY(X+1,Y-4)] = CRGB(R, G, B);
    leds[XY(X+2,Y)] = CRGB(R, G, B);
    leds[XY(X+2,Y-1)] = CRGB(R, G, B);
    leds[XY(X+2,Y-2)] = CRGB(R, G, B);
    leds[XY(X+2,Y-3)] = CRGB(R, G, B);
    leds[XY(X+2,Y-4)] = CRGB(R, G, B);
  }
  if(cislo == 9){
    leds[XY(X+2,Y)] = CRGB(R, G, B);
    leds[XY(X+2,Y-1)] = CRGB(R, G, B);
    leds[XY(X+2,Y-2)] = CRGB(R, G, B);
    leds[XY(X+2,Y-3)] = CRGB(R, G, B);
    leds[XY(X+2,Y-4)] = CRGB(R, G, B);
    leds[XY(X,Y)] = CRGB(R, G, B);
    leds[XY(X,Y-2)] = CRGB(R, G, B);
    leds[XY(X,Y-3)] = CRGB(R, G, B);
    leds[XY(X,Y-4)] = CRGB(R, G, B);
    leds[XY(X+1,Y)] = CRGB(R, G, B);
    leds[XY(X+1,Y-2)] = CRGB(R, G, B);
    leds[XY(X+1,Y-4)] = CRGB(R, G, B);
  }
}
//===========================================================================================================================clickright
// funkce pro kliknuti praveho tlacitka
void clickright(){
  //jak se ma chovat v mainmodu
  if(mainmode >= nummainmode && setmainmode== true){
  }
  else{
    mainmode++;
  }
  //jak se ma chovat v modu animace
  if(modeanimace >= nummodeanimace && setmodeanimace == true){
  }
  else{
    modeanimace++;
  }
  //jak se ma chovat v modu hada
  if(setmodehad == true && starthada== false){
    starthada=true;
    doprava=true;
  }
  if(setmodehad == true && starthada== true && doleva== false ){
    dolu= false;
    nahoru= false;
    doprava= true;
    doleva= false;
  }
}
//===========================================================================================================================clickleft
// funkce pro kliknuti leveho tlacitka
void clickleft(){
  //jak se ma chovat v mainmodu
  if(mainmode <= 0  && setmainmode==true){
  }
  else{
    mainmode--;
  }
  //jak se ma chovat v modu animace
   if(modeanimace <= 0  && setmodeanimace==true){
  }
  else{
    modeanimace--;
  }
  //jak se ma chovat v modu hada
  if(setmodehad == true && starthada== false){
    starthada=true;
    doleva = true;
  }
  if(setmodehad == true && starthada== true && doprava== false){
    dolu= false;
    nahoru= false;
    doprava= false;
    doleva= true;
  }
}
//===========================================================================================================================clickplay
// funkce pro kliknuti play tlacitka
void clickplay(){
  if(mainmode==1 && setmainmode == true){
  setmainmode = false;
  mainmode=1;
  setmodeanimace = true;
  modeanimace = 0;
  }
  if(mainmode==2 && setmainmode == true){
  setmainmode = false;
  mainmode=2;
  setmodehad = true;
  starthada=false;
  }
  if(setyoulosthad==true && setmodehad== true){
    setmodehad =true;
    setyoulosthad = false;
    starthada = false;
    xjablko = 0;
    yjablko = 0;
    nahoru= false;
    dolu= false;
    doprava= false;
    doleva= false;
    delkahada =0;
    pocetbodu = 0;
    tisicehad = 0;
    stovkyhad = 0;
    desitkyhad =0;
    jednotkyhad =0;
    dotekzdi = false;
    blikyoulost= false;
    for (int i = 0; i < 255; i++) {
        had[i].x = 0;
        had[i].y = 0;
    }
    hadpredchozimillis= 0;
  }
  if(mainmode==3 && setmainmode == true){
  setmainmode = false;
  mainmode=3;
  setmodevumetr = true;
  }
  if(mainmode==4 && setmainmode == true){
  setmainmode = false;
  mainmode=4;
  setmodekresleni= true;
  }
}
//===========================================================================================================================clickhome
// funkce pro kliknuti home tlacitka
void clickhome(){
  if(setmodeanimace == true){
    setmodeanimace= false;
    setmainmode=true;
    modeanimace = 0;
    mainmode=1;
  }
  if(setmodehad == true){
    setmodehad= false;
    setmainmode=true;
    mainmode=2;
    setyoulosthad = false;
    starthada = false;
    xjablko = 0;
    yjablko = 0;
    nahoru= false;
    dolu= false;
    doprava= false;
    doleva= false;
    delkahada =0;
    pocetbodu = 0;
    tisicehad = 0;
    stovkyhad = 0;
    desitkyhad =0;
    jednotkyhad =0;
    dotekzdi = false;
    blikyoulost= false;
    for (int i = 0; i < 255; i++) {
        had[i].x = 0;
        had[i].y = 0;
    }
    hadpredchozimillis= 0;
  }
  if(setmodevumetr == true){
    setmodevumetr= false;
    setmainmode=true;
    mainmode=3;
  }
  if(setmodekresleni == true){
    setmodekresleni= false;
    setmainmode=true;
    mainmode=4;
    stopKresleni();
  }
}
//===========================================================================================================================clickup
void clickup(){
  //jak se ma chovat v modu hada
  if(setmodehad == true && starthada== false ){
    starthada=true;
    nahoru = true;
  }
  if(setmodehad == true && starthada== true && dolu== false){
    dolu= false;
    nahoru= true;
    doprava= false;
    doleva= false;
  }
}
//===========================================================================================================================clickdown
void clickdown(){
  //jak se ma chovat v modu hada
  if(setmodehad == true && starthada== false){
    starthada=true;
    dolu= true;
  }
  if(setmodehad == true && starthada== true && nahoru== false ){
    dolu= true;
    nahoru= false;
    doprava= false;
    doleva= false;
  }
}
//===========================================================================================================================MAIN MODY
void mainmodes(){

    if(mainmode==1 && setmainmode== true){
    if(prvnipruchodpacmana == false){
        FastLED.clear();
        pacman();
        FastLED.show();
        prvnipruchodpacmana = true;
      }
      EVERY_N_MILLISECONDS(200) {
        FastLED.clear();
        pacman();
        FastLED.show();
      }
    }
    else if(mainmode==2 && setmainmode== true){
      if(prvnipruchodhada == false){
        FastLED.clear();
        mainanimacehada();
        FastLED.show();
        prvnipruchodhada = true;
      }
      EVERY_N_MILLISECONDS(800) {
        FastLED.clear();
        mainanimacehada();
        FastLED.show();
      }
    }
    else if(mainmode==3 && setmainmode== true){
    FastLED.clear();
    animacemikrofon();
    FastLED.show();
    }
    else if(mainmode==4 && setmainmode== true){
    FastLED.clear();
    tuzka();
    FastLED.show();
    }
}
//===========================================================================================================================rainbow vlna
void rainbowwave(){
  FastLED.clear();
    static uint16_t t = 0; // časový posun pro animaci
    EVERY_N_MILLISECONDS(5){
    t++;
    
    for (uint8_t y = 0; y < HEIGHT; y++) {
      for (uint8_t x = 0; x < WIDTH; x++) {
      // diagonální vlna: čím dál na "jihu a východinaě", tím větší posun
      uint16_t index = XY(x, y);
      uint8_t hue = (x * 6 + y * 6 + t) % 255;  // 8 = rychlost šíření vlny
      leds[index] = CHSV(hue, 255, 255);        // plná saturace, plný jas
     }
    }
    FastLED.show();
    }
}
//===========================================================================================================================rainbow kruh
void rainbowcycle(){
    FastLED.clear();
    static uint16_t t = 0;        // časový posun

    EVERY_N_MILLISECONDS(5) {    // animace se obnoví každých 30 ms
      t++;

      float cx = (WIDTH - 1) / 2.0;   // střed X
      float cy = (HEIGHT - 1) / 2.0;  // střed Y

     for (uint8_t y = 0; y < HEIGHT; y++) {
       for (uint8_t x = 0; x < WIDTH; x++) {
          uint16_t index = XY(x, y);

          float dx = x - cx;
          float dy = y - cy;
          float angle = atan2(dy, dx);          // úhel v radiánech
          float dist = sqrt(dx * dx + dy * dy); // vzdálenost od středu

          // Hue = úhel * 40 + vzdálenost * 15 + čas
          uint8_t hue = (uint8_t)(angle * 40 + dist * 15 + t);

          leds[index] = CHSV(hue, 255, 255);
        }
      }
      FastLED.show();
    }
}

//===========================================================================================================================rachejtle
void exploze1(){
  leds[XY(raketaX, raketaY)] = CHSV(hueraketa, 255, 255);
  leds[XY(raketaX+1, raketaY)] = CHSV(hueraketa, 255, 255);
  leds[XY(raketaX+1, raketaY+1)] = CHSV(hueraketa, 255, 255);
  leds[XY(raketaX, raketaY+1)] = CHSV(hueraketa, 255, 255);
  leds[XY(raketaX-1, raketaY+1)] = CHSV(hueraketa, 255, 255);
  leds[XY(raketaX-1, raketaY)] = CHSV(hueraketa, 255, 255);
  leds[XY(raketaX-1, raketaY-1)] = CHSV(hueraketa, 255, 255);
  leds[XY(raketaX, raketaY-1)] = CHSV(hueraketa, 255, 255);
  leds[XY(raketaX+1, raketaY-1)] = CHSV(hueraketa, 255, 255);
}
void exploze2(){
          leds[XY(raketaX, raketaY)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX+1, raketaY)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX+1, raketaY+1)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX, raketaY+1)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX-1, raketaY+1)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX-1, raketaY)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX-1, raketaY-1)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX, raketaY-1)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX+1, raketaY-1)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX+2, raketaY)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX+3, raketaY)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX, raketaY+2)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX, raketaY+3)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX-2, raketaY)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX-3, raketaY)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX, raketaY-2)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX, raketaY-3)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX+2, raketaY+1)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX+1, raketaY+2)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX-1, raketaY+2)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX-2, raketaY+1)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX-1, raketaY-2)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX-1, raketaY-2)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX+1, raketaY-2)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX+2, raketaY-1)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX-2, raketaY-1)] = CHSV(hueraketa, 255, 255);
}
void exploze3(){
          leds[XY(raketaX, raketaY)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX+1, raketaY)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX+1, raketaY+1)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX, raketaY+1)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX-1, raketaY+1)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX-1, raketaY)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX-1, raketaY-1)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX, raketaY-1)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX+1, raketaY-1)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX+2, raketaY)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX+3, raketaY)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX, raketaY+2)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX, raketaY+3)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX-2, raketaY)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX-3, raketaY)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX, raketaY-2)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX, raketaY-3)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX+2, raketaY+1)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX+1, raketaY+2)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX-1, raketaY+2)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX-2, raketaY+1)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX-1, raketaY-2)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX-1, raketaY-2)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX+1, raketaY-2)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX+2, raketaY-1)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX-2, raketaY-1)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX+3, raketaY+1)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX+3, raketaY+2)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX+3, raketaY-1)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX+3, raketaY-2)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX+2, raketaY+3)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX+1, raketaY+3)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX-1, raketaY+3)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX-2, raketaY+3)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX-3, raketaY+2)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX-3, raketaY+1)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX-3, raketaY-1)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX-3, raketaY-2)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX-2, raketaY-3)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX-1, raketaY-3)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX+1, raketaY-3)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX+2, raketaY-3)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX+2, raketaY-2)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX+2, raketaY+2)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX-2, raketaY+2)] = CHSV(hueraketa, 255, 255);
          leds[XY(raketaX-2, raketaY-2)] = CHSV(hueraketa, 255, 255);
}


void rachejtle(){
    unsigned long now = millis();

    // --- Spouštění nové rakety ---
    if(!raketaleti && !fadeActive && (now - posledniRaketa >= 1500)){
        raketaX = random(3, 13);
        raketaY = random(3, 13);
        pozicerakety = HEIGHT - 1;
        hueraketa += 15;
        raketaleti = true;
        fazeexploze = 0;
        posledniRaketa = now;
    }

    // --- Pohyb rakety ---
    if(raketaleti && now - posledniPohyb > 20){
        FastLED.clear();
        leds[XY(raketaX, pozicerakety)] = CRGB::White;
        FastLED.show();
        pozicerakety--;
        posledniPohyb = now;

        if(pozicerakety <= raketaY){
            raketaleti = false;
            fazeexploze = 1;
            posledniexploze = now;
        }
    }

    // --- Exploze ---
    if(fazeexploze >= 1 && fazeexploze <= 3 && !fadeActive){
        if(now - posledniexploze >= 80){
            FastLED.clear();
            if(fazeexploze == 1) exploze1();
            else if(fazeexploze == 2) exploze2();
            else if(fazeexploze == 3){
                exploze3();
                for (int i = 0; i < NUM_LEDS; i++) {
                  ledsohnastroj[i] = leds[i];   // uložíme původní barvy
                }
                fadeActive = true;         // spouštíme fade
                casFadeStart = now;
            }
            FastLED.show();
            fazeexploze++;
            posledniexploze = now;
        }
    }

    // --- Fade třetí exploze ---
    if (fadeActive) {
      unsigned long elapsed = now - casFadeStart;
      if (elapsed <= 1000) {   // 3 sekundy fade
        uint8_t brightness = map(elapsed, 0, 1000, 255, 0);
        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = ledsohnastroj[i];
            leds[i].nscale8_video(brightness); // spočítáme jas z původní barvy
        }
        FastLED.show();
      } 
      else {
        fadeActive = false;
        FastLED.clear();
        FastLED.show();
      }
    } 
}

//===========================================================================================================================ohen

void Fire2D(uint8_t cooling = 55, uint8_t sparking = 120, uint8_t speedDelay = 50) {
  static unsigned long last = 0;
  unsigned long now = millis();
  if (now - last < speedDelay) return;
  last = now;

  // 1) ochlazení každého pixelu
  for (uint8_t x = 0; x < WIDTH; x++) {
    for (uint8_t y = 0; y < HEIGHT; y++) {
      // qsub8 bezpečně odečítá bez podtečení
      heat[x][y] = qsub8(heat[x][y], random8(0, (cooling * 10 / HEIGHT) + 2));
    }
  }

  // 2) difúze / šíření nahoru (vytvoří dojem, že plamen stoupá)
  uint8_t newHeat[WIDTH][HEIGHT];
  // nejprve nulujeme nový buffer
  for (uint8_t x = 0; x < WIDTH; x++) {
    for (uint8_t y = 0; y < HEIGHT; y++) {
      newHeat[x][y] = 0;
    }
  }

  for (uint8_t x = 0; x < WIDTH; x++) {
    for (uint8_t y = 0; y < HEIGHT; y++) {
      // bereme hodinanoty z aktuální buňky a ze "spodních sousedů" (y+1)
      uint16_t sum = heat[x][y];
      uint8_t count = 1;

      // spodní buňka
      if (y < HEIGHT - 1) {
        sum += heat[x][y + 1];
        count++;
      } else {
        // spodní okraj: použijeme samotnou buňku (ne index 16!)
        sum += heat[x][y];
        count++;
      }

      // spodní-levý
      uint8_t xl = (x == 0) ? (WIDTH - 1) : (x - 1);
      if (y < HEIGHT - 1) {
        sum += heat[xl][y + 1];
        count++;
      } else {
        sum += heat[xl][y];
        count++;
      }

      // spodní-pravý
      uint8_t xr = (x == WIDTH - 1) ? 0 : (x + 1);
      if (y < HEIGHT - 1) {
        sum += heat[xr][y + 1];
        count++;
      } else {
        sum += heat[xr][y];
        count++;
      }

      newHeat[x][y] = (uint8_t)(sum / count);
    }
  }

  // přepíšu heat novými hodinanotami
  memcpy(heat, newHeat, sizeof(heat));

  // 3) náhodinané jiskry na spodním řádku (y = HEIGHT-1)
  for (uint8_t x = 0; x < WIDTH; x++) {
    if (random8() < sparking) {
      // přidat náhlý nárůst teploty dole
      heat[x][HEIGHT - 1] = qadd8(heat[x][HEIGHT - 1], random8(160, 255));
    }
  }

  // 4) převod heat -> barva a vykreslení
  for (uint8_t x = 0; x < WIDTH; x++) {
    for (uint8_t y = 0; y < HEIGHT; y++) {
      // přemapování teploty na index palety
      uint8_t colorIndex = scale8(heat[x][y], 240);
      CRGB color = ColorFromPalette(HeatColors_p, colorIndex);
      leds[XY(x, y)] = color;
    }
  }
  FastLED.show();
}


//===========================================================================================================================nacti cas z wifi
void setRTCfromNTP() {
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    DateTime nowRTC(
      1900 + timeinfo.tm_year,
      1 + timeinfo.tm_mon,
      timeinfo.tm_mday,
      timeinfo.tm_hour,
      timeinfo.tm_min,
      timeinfo.tm_sec
    );
    rtc.adjust(nowRTC);
    Serial.println("RTC nastaveno z NTP.");
  } else {
    Serial.println("NTP: nelze získat čas.");
  }
}

void manageWiFiNonBlocking() {
  unsigned long now = millis();

  if (WiFi.status() != WL_CONNECTED) {
    if (!wifiConnecting && (now - lastWiFiCheck >= wifiCheckInterval)) {
      Serial.println("Wi-Fi odpojena -> start reconnect...");
      WiFi.begin(ssid, password);
      wifiConnecting = true;
      wifiConnectStart = now;
      lastWiFiCheck = now;
    }

    if (wifiConnecting && (now - wifiConnectStart) > wifiConnectTimeout) {
      Serial.println("Wi-Fi reconnect timeout, restartuju pokus...");
      WiFi.disconnect();
      delay(50);
      WiFi.begin(ssid, password);
      wifiConnectStart = now;
    }
  } else {
    if (wifiConnecting) {
      wifiConnecting = false;
      Serial.print("Wi-Fi připojena, IP: ");
      Serial.println(WiFi.localIP());

      // ====== LETNÍ / ZIMNÍ ČAS – JEDINÁ ZMĚNA ======
      configTzTime("CET-1CEST,M3.5.0/2,M10.5.0/3",ntpServer);

      // =============================================

      setRTCfromNTP();
    }
  }
}

void nacticaszwifi() {

  unsigned long now = millis();
  uint8_t R = 255;
  uint8_t G = 255;
  uint8_t B = 255;

  manageWiFiNonBlocking();

  if (now - lastTimePrint >= timeInterval) {
    lastTimePrint = now;
    bool casOK = false;

    if (WiFi.status() == WL_CONNECTED) {
      struct tm timeinfo;
      if (getLocalTime(&timeinfo)) {
        hodina = timeinfo.tm_hour;
        minuta = timeinfo.tm_min;

        hodina1 = hodina / 10;
        hodina2 = hodina % 10;
        minuta1 = minuta / 10;
        minuta2 = minuta % 10;

        Serial.printf("z wifi: hodiny: %d%d, minuty: %d%d\n",
                      hodina1, hodina2, minuta1, minuta2);

        DateTime nowRTC(
          1900 + timeinfo.tm_year,
          1 + timeinfo.tm_mon,
          timeinfo.tm_mday,
          timeinfo.tm_hour,
          timeinfo.tm_min,
          timeinfo.tm_sec
        );
        rtc.adjust(nowRTC);
        casOK = true;
      }
    }

    if (!casOK) {
      DateTime nowRTC = rtc.now();
      hodina = nowRTC.hour();
      minuta = nowRTC.minute();

      hodina1 = hodina / 10;
      hodina2 = hodina % 10;
      minuta1 = minuta / 10;
      minuta2 = minuta % 10;

      Serial.printf("RTC čas: %d%d:%d%d\n",
                    hodina1, hodina2, minuta1, minuta2);
    }

    FastLED.clear();

    EVERY_N_MILLISECONDS(1000) {
      vykreslicislo(0, 6, hodina1, R, G, B);
      vykreslicislo(4, 6, hodina2, R, G, B);
      vykreslicislo(8, 6, minuta1, R, G, B);
      vykreslicislo(12, 6, minuta2, R, G, B);

      if (dvojtecka) {
        leds[XY(7, 3)] = CHSV(0, 0, 128);
        leds[XY(7, 5)] = CHSV(0, 0, 128);
        dvojtecka = false;
      } else {
        leds[XY(7, 3)] = CRGB::Black;
        leds[XY(7, 5)] = CRGB::Black;
        dvojtecka = true;
      }

      vlhkost = dht.readHumidity();
      teplota = dht.readTemperature();

      if (isnan(vlhkost) || isnan(teplota)) {
        Serial.println("Chyba při čtení z DHT11!");
      }

      vlhkost1 = vlhkost / 10;
      vlhkost2 = vlhkost % 10;
      teplota1 = teplota / 10;
      teplota2 = teplota % 10;

      if (pocetsekundDHT < 5) {
        for (int i = 128; i <= 255; i++) leds[i] = CRGB::Black;
        vykreslicislo(0, 13, teplota1, R, G, B);
        vykreslicislo(4, 13, teplota2, R, G, B);
        //kresleni stupnu celsia
        leds[XY(8,9)] = CRGB(R, G, B);
        leds[XY(10,9)] = CRGB(R, G, B);
        leds[XY( 11,9 )] = CRGB(R, G, B);
        leds[XY( 10,10 )] = CRGB(R, G, B);
        leds[XY(10 ,11 )] = CRGB(R, G, B);
        leds[XY( 11,11 )] = CRGB(R, G, B);
      }

      if (pocetsekundDHT >= 5 && pocetsekundDHT < 10) {
        for (int i = 128; i <= 255; i++) leds[i] = CRGB::Black;
        vykreslicislo(0, 13, vlhkost1, R, G, B);
        vykreslicislo(4, 13, vlhkost2, R, G, B);
        //kresleni procenta
        leds[XY( 8,13 )] = CRGB(R, G, B);
        leds[XY( 9,12 )] = CRGB(R, G, B);
        leds[XY( 10,11 )] = CRGB(R, G, B);
        leds[XY( 11,10 )] = CRGB(R, G, B);
        leds[XY( 12, 9)] = CRGB(R, G, B);
        leds[XY( 8, 10)] = CRGB(R, G, B);
        leds[XY( 8, 9)] = CRGB(R, G, B);
        leds[XY( 9, 9)] = CRGB(R, G, B);
        leds[XY( 12, 12)] = CRGB(R, G, B);
        leds[XY( 12, 13)] = CRGB(R, G, B);
        leds[XY( 11, 13)] = CRGB(R, G, B);
        leds[XY( 9,10 )] = CRGB(R, G, B);
        leds[XY( 11,12 )] = CRGB(R, G, B);
      }

      pocetsekundDHT++;
      if (pocetsekundDHT >= 10) pocetsekundDHT = 0;

      if (WiFi.status() == WL_CONNECTED)
        leds[XY(0, 0)] = CRGB(0, 255, 0);
      else
        leds[XY(0, 0)] = CRGB(255, 0, 0);

      FastLED.show();
    }
  }
}

//===========================================================================================================================had
void youlosthad(){
  starthada = false;
  setyoulosthad = true;
  
  
  int xyoulost=0;
    unsigned long hadmillisted = millis();
  if(hadmillisted-hadpredchozimillis>= 400){
    hadpredchozimillis=hadmillisted;
    blikyoulost= !blikyoulost;
  }
  FastLED.clear();
  if(blikyoulost){
  
  
   xyoulost=0;
  for(int i=0; i<=9;i++){
    if(i<2 || i>4){
      leds[XY(xyoulost,i )] = CRGB(255, 0, 0);
    }
  }
  xyoulost=1;
  for(int i=0; i<=9;i++){
    if((i>0 && i<4) || i>8){
      leds[XY(xyoulost,i )] = CRGB(255, 0, 0);
    }
  }
  xyoulost=2;
  for(int i=0; i<=9;i++){
    if(i<2 || i>8){
      leds[XY(xyoulost,i )] = CRGB(255, 0, 0);
    }
  }
  xyoulost=4;
  for(int i=0; i<=9;i++){
    if(i<4 || i>4){
      leds[XY(xyoulost,i )] = CRGB(255, 0, 0);
    }
  }
  xyoulost=5;
  leds[XY(xyoulost,0 )] = CRGB(255, 0, 0);
  leds[XY(xyoulost,3 )] = CRGB(255, 0, 0);
  leds[XY(xyoulost,5 )] = CRGB(255, 0, 0);
  leds[XY(xyoulost,9 )] = CRGB(255, 0, 0);

  xyoulost=6;
  for(int i=0; i<=9;i++){
    if(i<4 || i>4){
      leds[XY(xyoulost,i )] = CRGB(255, 0, 0);
    }
  }
  xyoulost=8;
  for(int i=0; i<=7;i++){
    if(i<4 || i>4){
      leds[XY(xyoulost,i )] = CRGB(255, 0, 0);
    }
  }
  leds[XY(xyoulost,9 )] = CRGB(255, 0, 0);

  xyoulost=9;
  leds[XY(xyoulost,9 )] = CRGB(255, 0, 0);
  leds[XY(xyoulost,7 )] = CRGB(255, 0, 0);
  leds[XY(xyoulost,5 )] = CRGB(255, 0, 0);
  leds[XY(xyoulost,3 )] = CRGB(255, 0, 0);

  xyoulost=10;
  for(int i=0; i<=9;i++){
    if(i<4 || i>6){
      leds[XY(xyoulost,i )] = CRGB(255, 0, 0);
    }
  }
  leds[XY(xyoulost,5 )] = CRGB(255, 0, 0);

  xyoulost=12;
  leds[XY(xyoulost,5 )] = CRGB(255, 0, 0);

  xyoulost=13;
  for(int i=5; i<=9;i++){
      leds[XY(xyoulost,i )] = CRGB(255, 0, 0);
  }

  xyoulost=14;
  leds[XY(xyoulost,5 )] = CRGB(255, 0, 0);
  FastLED.show();
  blikyoulost= true;
  }
  else{
    for (int i = 0; i <= 159; i++) {
      leds[i] = CRGB::Black;
    }
  }
  tisicehad= pocetbodu/1000;
  stovkyhad= (pocetbodu/100)%10;
  desitkyhad = (pocetbodu/10)%10;
  jednotkyhad= pocetbodu%10;

  vykreslicislo(0,15, tisicehad, 255,255, 255);
  vykreslicislo(4,15, stovkyhad, 255,255, 255);
  vykreslicislo(8,15, desitkyhad, 255,255, 255);
  vykreslicislo(12,15, jednotkyhad, 255,255, 255);

  FastLED.show();

  
}

void generujjablko(){
  bool chybajablka;
  do {
  chybajablka = false;
  xjablko = random(1, 15);
  yjablko = random(1, 15);

  for (int i = 0; i <= delkahada; i++) {
    if (xjablko == had[i].x && yjablko == had[i].y) {
      chybajablka = true;
      break;
    }
  }

  } 
  while (chybajablka);
}

  void haddoprava(){
    if(!(had[0].x== xjablko && had[0].y== yjablko)){
      for(int i=0;i<=delkahada;i++){
        if(had[0].x+1== had[i].x && had[0].y== had[i].y){
            youlosthad();
        }
      }
      if(had[0].x+1 > 15){
        if(dotekzdi){
          youlosthad();
        }
        else{
          dotekzdi= true;
        }
      }
      else{
      dotekzdi = false;
      
      for (int i = delkahada; i > 0; i--) {
        had[i].x = had[i-1].x;
        had[i].y = had[i-1].y;
      }
      had[0].x++;
      }
    }
    else if(had[0].x== xjablko && had[0].y== yjablko){
      delkahada++;
      pocetbodu++;
      for (int i = delkahada; i > 0; i--) {
        had[i].x = had[i-1].x;
        had[i].y = had[i-1].y;
      }
      had[0].x++;
      generujjablko();
      }
  }
void haddoleva(){
  if(!(had[0].x== xjablko && had[0].y== yjablko)){
      for(int i=0;i<=delkahada;i++){
        if(had[0].x-1== had[i].x && had[0].y== had[i].y){
            youlosthad();
        }
      }
    if(had[0].x-1 < 0){
      if(dotekzdi){
          youlosthad();
        }
        else{
          dotekzdi= true;
      }
    }
    else{
      dotekzdi= false;
      for (int i = delkahada; i > 0; i--) {
        had[i].x = had[i-1].x;
        had[i].y = had[i-1].y;
      }
      had[0].x--;
    }
  }
  else if(had[0].x== xjablko && had[0].y== yjablko){
    delkahada++;
    pocetbodu++;
    generujjablko();
      for (int i = delkahada; i > 0; i--) {
        had[i].x = had[i-1].x;
        had[i].y = had[i-1].y;
      }
      had[0].x--;
  }
}
void hadnahoru(){
  if(!(had[0].x== xjablko && had[0].y== yjablko)){
    for(int i=0;i<=delkahada;i++){
      if(had[0].x== had[i].x && had[0].y-1== had[i].y){
        youlosthad();
      }
    }
    if(had[0].y-1 < 0){
      if(dotekzdi){
          youlosthad();
        }
        else{
          dotekzdi= true;
      }
    }
    else{
      dotekzdi= false;
      for (int i = delkahada; i > 0; i--) {
        had[i].x = had[i-1].x;
        had[i].y = had[i-1].y;
      }
      had[0].y--;
      
    }
  }
  else if(had[0].x== xjablko && had[0].y== yjablko){
    delkahada++;
    pocetbodu++;
    generujjablko();
      for (int i = delkahada; i > 0; i--) {
        had[i].x = had[i-1].x;
        had[i].y = had[i-1].y;
      }
      had[0].y--;
  }
}
void haddolu(){
  if(!(had[0].x== xjablko && had[0].y== yjablko)){
    for(int i=0;i<=delkahada;i++){
      if(had[0].x== had[i].x && had[0].y+1== had[i].y){
        youlosthad();
      }
    }
    if(had[0].y+1 > 15){
      if(dotekzdi){
          youlosthad();
        }
        else{
          dotekzdi= true;
      }
    }
    else{
      dotekzdi= false;
      for (int i = delkahada; i > 0; i--) {
        had[i].x = had[i-1].x;
        had[i].y = had[i-1].y;
      }
      had[0].y++;
    }
  }
  else if(had[0].x== xjablko && had[0].y== yjablko){
    delkahada++;
    pocetbodu++;
    generujjablko();
      for (int i = delkahada; i > 0; i--) {
        had[i].x = had[i-1].x;
        had[i].y = had[i-1].y;
      }
      had[0].y++;
  }
}
void snake(){
  if(setyoulosthad == true){
  youlosthad();
  }
  else{
  if(starthada== false){
    FastLED.clear();
    while(xjablko == had[0].x && yjablko == had[0].y){
    xjablko = random(1,15);
    yjablko = random(1,15);
    had[0].x= random(3,13);
    had[0].y= random(3,13);
    leds[XY( xjablko,yjablko )] = CRGB(255, 0, 0);
    leds[XY(had[0].x, had[0].y)]=CRGB(0, 255, 0);
    FastLED.show();
  }
  }
  unsigned long hadmainnowmillis = millis();
  if(hadmainnowmillis-hadmainlastmillis  >= (500-(5*pocetbodu))){
  hadmainlastmillis = hadmainnowmillis;
  if(starthada == true){
    if(doprava == true){
    haddoprava();
    }
    if(doleva == true){
    haddoleva();
    }
    if(dolu == true){
    haddolu();
    }
    if(nahoru == true){
    hadnahoru();
    }
    FastLED.clear();
    leds[XY( xjablko,yjablko )] = CRGB(255, 0, 0);
    for(int i=0; i<= delkahada; i++){
      if (i % 2 == 0) {
        leds[XY(had[i].x, had[i].y)]=CRGB(0, 125, 0);
      }
      else{
        leds[XY(had[i].x, had[i].y)]=CRGB(0, 255, 0);
      }
    }
    FastLED.show();
  }
  }
}

}
//===========================================================================================================================vumetr
void vumetr(){
    FastLED.clear();

  for (uint8_t band = 0; band < 16; band++) {

    // jemné vyhlazení
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

  // ===== BAR =====
void drawRainbowBar(uint8_t band, uint8_t height) {
  for (uint8_t y = 0; y <= height; y++) {
    uint8_t hue = map(band, 0, 15, 0, 255);
    leds[XY(band, (HEIGHT - 1) - y)] = CHSV(hue, 255, 255);
  }
} 
// ===== PEAK DOT =====
void drawWhitePeak(uint8_t band) {
  leds[XY(band, (HEIGHT - 1) - peak[band])] = CRGB::White;
}


// ===== PEAK DECAY =====
void decayPeaks() {
  static uint32_t lastDecay = 0;
  if (millis() - lastDecay > 60) {
    lastDecay = millis();
    for (uint8_t i = 0; i < 16; i++) {
      if (peak[i] > 0) peak[i]--;
    }
  }
}

//===========================================================================================================================drawing 
/* ================= HTML ================= */
const char MAIN_page[] PROGMEM = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
  body { background:#000; color:#fff; font-family:sans-serif; text-align:center; touch-action:none; }
  canvas { border:2px solid #444; width:320px; height:320px; image-rendering:pixelated; touch-action:none; }
  .controls { margin-top:10px; }
  button, input { background:#222; color:#fff; border:1px solid #555; padding:8px; margin:4px; font-size:16px; }
  button.active { background:#555; }
  </style>
  </head>
  <body>

  <h2>RGB DRAW</h2>
  <canvas id="c" width="16" height="16"></canvas>

  <div class="controls">
  <input type="color" id="color" value="#ff0000"><br>
  <button onclick="setBrush(1)">1x1</button>
  <button onclick="setBrush(3)">3x3</button>
  <button onclick="setBrush(5)">5x5</button><br>
  <button onclick="eraser()">Guma</button>
  <button onclick="clearPanel()">Clear</button>
  </div>

  <script>
    const canvas = document.getElementById("c");
  const ctx = canvas.getContext("2d");
  ctx.imageSmoothingEnabled = false;

  let drawing = false;
  let brush = 1;
  let erase = false;

  function setBrush(size){ brush=size; erase=false; }
  function eraser(){ erase=true; }

  function hexToRgb(hex){
  const n=parseInt(hex.slice(1),16);
  return [(n>>16)&255,(n>>8)&255,n&255];
  }

  function draw(clientX, clientY){
  const r=canvas.getBoundingClientRect();
  const cx=Math.floor((clientX-r.left)/r.width*16);
  const cy=Math.floor((clientY-r.top)/r.height*16);

  const half=Math.floor(brush/2);
  const col=erase?[0,0,0]:hexToRgb(document.getElementById("color").value);

  let pixels=[];
  for(let y=-half;y<=half;y++){
    for(let x=-half;x<=half;x++){
      const px=cx+x;
      const py=cy+y;
      if(px<0||py<0||px>=16||py>=16) continue;

      ctx.fillStyle=erase?"#000":document.getElementById("color").value;
      ctx.fillRect(px,py,1,1);

      pixels.push({x:px,y:py,r:col[0],g:col[1],b:col[2]});
    }
  }

  if(pixels.length){
    fetch("/update",{method:"POST",headers:{"Content-Type":"application/json"},body:JSON.stringify({pixels})});
  }
  }

  /* TOUCH + MOUSE */
  ["mousedown","touchstart"].forEach(e=>{
  canvas.addEventListener(e,ev=>{
    drawing=true;
    const p=ev.touches?ev.touches[0]:ev;
    draw(p.clientX,p.clientY);
    ev.preventDefault();
  });
  });
  ["mousemove","touchmove"].forEach(e=>{
  canvas.addEventListener(e,ev=>{
    if(!drawing) return;
    const p=ev.touches?ev.touches[0]:ev;
    draw(p.clientX,p.clientY);
    ev.preventDefault();
  });
  });
  ["mouseup","mouseleave","touchend"].forEach(e=>{
  canvas.addEventListener(e,()=>drawing=false);
  });

  function clearPanel(){
  ctx.fillStyle="#000";
  ctx.fillRect(0,0,16,16);
  fetch("/clear",{method:"POST"});
  }
  </script>
  </body>
  </html>
  )rawliteral";

void handleRoot(){ server.send_P(200,"text/html",MAIN_page); }

void handleUpdate(){
  if(!server.hasArg("plain")){ server.send(400,"text/plain","NO DATA"); return; }
  DynamicJsonDocument doc(2048);
  if(deserializeJson(doc,server.arg("plain"))){
    server.send(400,"text/plain","BAD JSON"); 
    return;
  }

  JsonArray arr = doc["pixels"].as<JsonArray>();
  for(JsonObject p : arr){
    uint8_t x = p["x"];
    uint8_t y = p["y"];
    uint8_t r = p["r"];
    uint8_t g = p["g"];
    uint8_t b = p["b"];
    if(x<WIDTH && y<HEIGHT){
      ledBuffer[x][y] = CRGB(r,g,b);
      needsShow = true;
    }
  }
  server.send(200,"text/plain","OK");
}

void handleClear(){
  for(uint8_t y=0;y<HEIGHT;y++)
    for(uint8_t x=0;x<WIDTH;x++)
      ledBuffer[x][y]=CRGB::Black;
  needsShow=true;
  server.send(200,"text/plain","CLEARED");
}

void kresleni(){
  if(!kresleniActive){
    kresleniActive = true;
    FastLED.clear(true); // clear + show
    delay(5);
    

    // Odpojit STA a spustit AP
    WiFi.disconnect(true);
    delay(100);
    WiFi.mode(WIFI_AP);
    WiFi.softAP("RGB-DISPLAY","12345678");
    Serial.print("RGB MODE: AP IP = "); Serial.println(WiFi.softAPIP());

    // Nastavit server
    server.on("/", handleRoot);
    server.on("/update", HTTP_POST, handleUpdate);
    server.on("/clear", HTTP_POST, handleClear);
    server.begin();

    
    joinwifi();
    FastLED.show();
    
  }
  
  

  // Obsluha klientů a vykreslení bufferu
  server.handleClient();
  static unsigned long lastShow = 0;
  if(millis()-lastShow>50 && needsShow){
    for(uint8_t y=0;y<HEIGHT;y++){
      for(uint8_t x=0;x<WIDTH;x++){
        leds[XY(x,y)] = ledBuffer[x][y];
      }
    }
    FastLED.show();
    needsShow=false;
    lastShow=millis();
  }
}

void stopKresleni(){
  if(!kresleniActive) return;
  kresleniActive = false;

  server.stop();
  WiFi.softAPdisconnect(true);
  delay(100);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Kresleni ukonceno, pokus o pripojeni k WiFi STA...");
}

 
//===========================================================================================================================setup
void setup() {
  Serial.begin(115200);
  //I2C
  Wire.begin(21,22);  //38,39
// display
  FastLED.addLeds<CHIPSET, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, AMPS);
  FastLED.setBrightness(BRIGHTNESS);
  
  FastLED.clear();
  delay(200);
  FastLED.show();
  delay(200);
 //tlacitka
  pinMode(BUTTON_ARROW_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_ARROW_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_ARROW_UP, INPUT_PULLUP);
  pinMode(BUTTON_ARROW_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_HOME, INPUT_PULLUP);
  pinMode(BUTTON_PLAY, INPUT_PULLUP);

  buttonUp.attachClick(clickup);
  buttonRight.attachClick(clickright);
  buttonDown.attachClick(clickdown);
  buttonLeft.attachClick(clickleft);
  buttonPlay.attachClick(clickplay);
  buttonHome.attachClick(clickhome);
//ohen
  memset(heat, 0, sizeof(heat));
//cas
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  wifiConnecting = true;
  wifiConnectStart = millis();
  lastWiFiCheck = millis();
  Serial.println("Startuji Wi-Fi (non-blocking) ...");
  //rtc start
  if (!rtc.begin()) {
    Serial.println("Nenalezen RTC modul DS3231! Pokračuji jen s WiFi časem.");
  }
  if (rtc.lostPower()) {
    Serial.println("RTC nemá čas -> nastavím později z internetu.");
  }
  //dht11 start
  dht.begin();

  // setup random seedu
  randomSeed(esp_random());

  // setup VU metru
  setupAudio();

  
}

//===========================================================================================================================loop

void loop() {
  buttonRight.tick();
  buttonLeft.tick();
  buttonPlay.tick();
  buttonHome.tick();
  buttonDown.tick();
  buttonUp.tick();

  if(mainmode==0 && setmainmode== true){
    nacticaszwifi();
  }
  else if(modeanimace == 0 && setmodeanimace == true){
    rainbowwave();
  }
  else if(modeanimace == 1 && setmodeanimace == true){
    rainbowcycle();
  }
  else if(modeanimace == 2 && setmodeanimace == true){
    rachejtle();
  }
  else if(modeanimace == 3 && setmodeanimace == true){
    Fire2D(50, 90, 120);
  }
  else if(setmodehad == true && setmainmode== false){
    snake();
  }
  else if(setmodevumetr == true && setmainmode== false){
    vumetr();
  }
  else if(setmodekresleni==true  && setmainmode== false){
    kresleni();
    }
  else{
    mainmodes();
  }
}

//======================================================================================================================================================================================================================================================
//======================================================================================================================================================================================================================================================
//======================================================================================================================================================================================================================================================
//======================================================================================================================================================================================================================================================
//======================================================================================================================================================================================================================================================
//===========================================================================================================================telohada1
void telohada1(){
  //tmave zelena 0,80, 0
  //svetle zelena 70,255,0
  //bile zelena 162,255, 196
  //zelena 0,255,0
  //tmave cervena 133,0,0
  //cervena 255,0,0

  //tmave zelena====================================
  int xhad=0;
  for(int i=0; i<=15;i++){
    if(i>10 && i<14){
      leds[XY(xhad,i )] = CRGB(0,80, 0);
    }
  }

  xhad=1;
  for(int i=0; i<=15;i++){
    if(i>7 && i<12){
      leds[XY(xhad,i )] = CRGB(0,80, 0);
    }
  }
  leds[XY(xhad,14 )] = CRGB(0,80, 0);
  leds[XY(xhad,15 )] = CRGB(0,80, 0);

  xhad=2;
  leds[XY(xhad,10 )] = CRGB(0,80, 0);
  leds[XY(xhad,11 )] = CRGB(0,80, 0);
  leds[XY(xhad,15 )] = CRGB(0,80, 0);

  xhad=3;
  leds[XY(xhad,10 )] = CRGB(0,80, 0);
  leds[XY(xhad,11 )] = CRGB(0,80, 0);
  leds[XY(xhad,15 )] = CRGB(0,80, 0);
  xhad=4;
  leds[XY(xhad,11 )] = CRGB(0,80, 0);
  leds[XY(xhad,15 )] = CRGB(0,80, 0);
  xhad=5;
  leds[XY(xhad,11 )] = CRGB(0,80, 0);
  leds[XY(xhad,15 )] = CRGB(0,80, 0);
  xhad=6;
  leds[XY(xhad,8 )] = CRGB(0,80, 0);
  leds[XY(xhad,11 )] = CRGB(0,80, 0);
  leds[XY(xhad,12 )] = CRGB(0,80, 0);
  leds[XY(xhad,15 )] = CRGB(0,80, 0);

  for(xhad = 7;xhad<13;xhad++){
    leds[XY(xhad,8 )] = CRGB(0,80, 0);
    leds[XY(xhad,12 )] = CRGB(0,80, 0);
    leds[XY(xhad,15 )] = CRGB(0,80, 0);
  }
  xhad=13;
  leds[XY(xhad,8 )] = CRGB(0,80, 0);
  leds[XY(xhad,9)] = CRGB(0,80, 0);
  leds[XY(xhad,12 )] = CRGB(0,80, 0);
  leds[XY(xhad,13 )] = CRGB(0,80, 0);
  leds[XY(xhad,15 )] = CRGB(0,80, 0);

  xhad=14;
  for(int i=0; i<=15;i++){
    if(i>10 ){
      leds[XY(xhad,i )] = CRGB(0,80, 0);
    }
  }
  leds[XY(xhad,9)] = CRGB(0,80, 0);  

  xhad=15;
  for(int i=0; i<=15;i++){
    if((i>8 && i<15)&&(i!=12)){
      leds[XY(xhad,i )] = CRGB(0,80, 0);
    }
  }
  //svetle zelena====================================
  xhad=2;
  leds[XY(xhad,13 )] = CRGB(70,255,0);

  xhad=3;
  leds[XY(xhad,8 )] = CRGB(70,255,0);
  leds[XY(xhad,9 )] = CRGB(70,255,0);
  leds[XY(xhad,13 )] = CRGB(70,255,0);
  leds[XY(xhad,14 )] = CRGB(70,255,0);

  xhad=4;
  leds[XY(xhad,8 )] = CRGB(70,255,0);
  leds[XY(xhad,9 )] = CRGB(70,255,0);
  leds[XY(xhad,10 )] = CRGB(70,255,0);
  leds[XY(xhad,13 )] = CRGB(70,255,0);
  leds[XY(xhad,14 )] = CRGB(70,255,0);

  xhad=5;
  leds[XY(xhad,9 )] = CRGB(70,255,0);
  leds[XY(xhad,10 )] = CRGB(70,255,0);
  leds[XY(xhad,13 )] = CRGB(70,255,0);
  leds[XY(xhad,14 )] = CRGB(70,255,0);

  xhad=6;
  leds[XY(xhad,9 )] = CRGB(70,255,0);
  leds[XY(xhad,10 )] = CRGB(70,255,0);
  leds[XY(xhad,13 )] = CRGB(70,255,0);
  leds[XY(xhad,14 )] = CRGB(70,255,0);

  xhad=7;
  leds[XY(xhad,9 )] = CRGB(70,255,0);
  leds[XY(xhad,10 )] = CRGB(70,255,0);
  leds[XY(xhad,14 )] = CRGB(70,255,0);

  for(xhad = 7;xhad<12;xhad++){
    leds[XY(xhad,9 )] = CRGB(70,255,0);
    leds[XY(xhad,10 )] = CRGB(70,255,0);
    leds[XY(xhad,11 )] = CRGB(70,255,0);
    leds[XY(xhad,14 )] = CRGB(70,255,0);
  }

  xhad=12;
  leds[XY(xhad,10 )] = CRGB(70,255,0);
  leds[XY(xhad,11 )] = CRGB(70,255,0);
  leds[XY(xhad,14 )] = CRGB(70,255,0);
  xhad=13;
  leds[XY(xhad,10 )] = CRGB(70,255,0);

  // zelena====================================
  xhad=1;
  leds[XY(xhad,12 )] = CRGB(0,255,0);
  leds[XY(xhad,13 )] = CRGB(0,255,0);

  xhad=2;
  leds[XY(xhad,8 )] = CRGB(0,255,0);
  leds[XY(xhad,9 )] = CRGB(0,255,0);
  leds[XY(xhad,12 )] = CRGB(0,255,0);
  leds[XY(xhad,14 )] = CRGB(0,255,0);

  xhad=3;
  leds[XY(xhad,12 )] = CRGB(0,255,0);

  xhad=4;
  leds[XY(xhad,12 )] = CRGB(0,255,0);

  xhad=5;
  leds[XY(xhad,8 )] = CRGB(0,255,0);
  leds[XY(xhad,12 )] = CRGB(0,255,0);

  xhad=7;
  leds[XY(xhad,11 )] = CRGB(0,255,0);
  leds[XY(xhad,13 )] = CRGB(0,255,0);

  for(xhad = 7;xhad<12;xhad++){
    leds[XY(xhad,13 )] = CRGB(70,255,0);
  }

  xhad=12;
  leds[XY(xhad,9 )] = CRGB(0,255,0);
  leds[XY(xhad,13 )] = CRGB(0,255,0);

  xhad=13;
  leds[XY(xhad,11 )] = CRGB(0,255,0);
  leds[XY(xhad,14 )] = CRGB(0,255,0);

  xhad=14;
  leds[XY(xhad,10 )] = CRGB(0,255,0);
}
//===========================================================================================================================telohada2
void telohada2(){
  //tmave zelena 0,80, 0
  //svetle zelena 70,255,0
  //bile zelena 162,255, 196
  //zelena 0,255,0
  //tmave cervena 133,0,0
  //cervena 255,0,0

  //tmave zelena====================================
  int xhad=0;
  for(int i=0; i<=15;i++){
    if(i>10 && i<14){
      leds[XY(xhad,i )] = CRGB(0,80, 0);
    }
  }

  xhad=1;
  for(int i=0; i<=15;i++){
    if(i>7 && i<12){
      leds[XY(xhad,i )] = CRGB(0,80, 0);
    }
  }
  leds[XY(xhad,14 )] = CRGB(0,80, 0);
  leds[XY(xhad,15 )] = CRGB(0,80, 0);

  xhad=2;
  leds[XY(xhad,8 )] = CRGB(0,80, 0);
  leds[XY(xhad,10 )] = CRGB(0,80, 0);
  leds[XY(xhad,11 )] = CRGB(0,80, 0);
  leds[XY(xhad,15 )] = CRGB(0,80, 0);

  xhad=3;
  leds[XY(xhad,10 )] = CRGB(0,80, 0);
  leds[XY(xhad,11 )] = CRGB(0,80, 0);
  leds[XY(xhad,15 )] = CRGB(0,80, 0);
  xhad=4;
  leds[XY(xhad,11 )] = CRGB(0,80, 0);
  leds[XY(xhad,15 )] = CRGB(0,80, 0);
  xhad=5;
  leds[XY(xhad,11 )] = CRGB(0,80, 0);
  leds[XY(xhad,15 )] = CRGB(0,80, 0);
  xhad=6;
  leds[XY(xhad,8 )] = CRGB(0,80, 0);
  leds[XY(xhad,11 )] = CRGB(0,80, 0);
  leds[XY(xhad,12 )] = CRGB(0,80, 0);
  leds[XY(xhad,15 )] = CRGB(0,80, 0);

  for(xhad = 7;xhad<13;xhad++){
    leds[XY(xhad,8 )] = CRGB(0,80, 0);
    leds[XY(xhad,12 )] = CRGB(0,80, 0);
    leds[XY(xhad,15 )] = CRGB(0,80, 0);
  }
  xhad=13;
  leds[XY(xhad,8 )] = CRGB(0,80, 0);
  leds[XY(xhad,9)] = CRGB(0,80, 0);
  leds[XY(xhad,12 )] = CRGB(0,80, 0);
  leds[XY(xhad,13 )] = CRGB(0,80, 0);
  leds[XY(xhad,15 )] = CRGB(0,80, 0);

  xhad=14;
  for(int i=0; i<=15;i++){
    if(i>10 ){
      leds[XY(xhad,i )] = CRGB(0,80, 0);
    }
  }
  leds[XY(xhad,9)] = CRGB(0,80, 0);  

  xhad=15;
  for(int i=0; i<=15;i++){
  if((i>8 && i<15)&&(i!=12)){
      leds[XY(xhad,i )] = CRGB(0,80, 0);
    }
  }
  //svetle zelena====================================
  xhad=2;
  leds[XY(xhad,13 )] = CRGB(70,255,0);

  xhad=3;
  leds[XY(xhad,8 )] = CRGB(70,255,0);
  leds[XY(xhad,9 )] = CRGB(70,255,0);
  leds[XY(xhad,13 )] = CRGB(70,255,0);
  leds[XY(xhad,14 )] = CRGB(70,255,0);

  xhad=4;
  leds[XY(xhad,8 )] = CRGB(70,255,0);
  leds[XY(xhad,9 )] = CRGB(70,255,0);
  leds[XY(xhad,10 )] = CRGB(70,255,0);
  leds[XY(xhad,13 )] = CRGB(70,255,0);
  leds[XY(xhad,14 )] = CRGB(70,255,0);

  xhad=5;
  leds[XY(xhad,9 )] = CRGB(70,255,0);
  leds[XY(xhad,10 )] = CRGB(70,255,0);
  leds[XY(xhad,13 )] = CRGB(70,255,0);
  leds[XY(xhad,14 )] = CRGB(70,255,0);

  xhad=6;
  leds[XY(xhad,9 )] = CRGB(70,255,0);
  leds[XY(xhad,10 )] = CRGB(70,255,0);
  leds[XY(xhad,13 )] = CRGB(70,255,0);
  leds[XY(xhad,14 )] = CRGB(70,255,0);

  xhad=7;
  leds[XY(xhad,9 )] = CRGB(70,255,0);
  leds[XY(xhad,10 )] = CRGB(70,255,0);
  leds[XY(xhad,14 )] = CRGB(70,255,0);

  for(xhad = 7;xhad<12;xhad++){
    leds[XY(xhad,9 )] = CRGB(70,255,0);
    leds[XY(xhad,10 )] = CRGB(70,255,0);
    leds[XY(xhad,11 )] = CRGB(70,255,0);
    leds[XY(xhad,14 )] = CRGB(70,255,0);
  }

  xhad=12;
  leds[XY(xhad,10 )] = CRGB(70,255,0);
  leds[XY(xhad,11 )] = CRGB(70,255,0);
  leds[XY(xhad,14 )] = CRGB(70,255,0);
  xhad=13;
  leds[XY(xhad,10 )] = CRGB(70,255,0);

  // zelena====================================
  xhad=1;
  leds[XY(xhad,12 )] = CRGB(0,255,0);
  leds[XY(xhad,13 )] = CRGB(0,255,0);

  xhad=2;
  leds[XY(xhad,9 )] = CRGB(0,255,0);
  leds[XY(xhad,12 )] = CRGB(0,255,0);
  leds[XY(xhad,14 )] = CRGB(0,255,0);

  xhad=3;
  leds[XY(xhad,12 )] = CRGB(0,255,0);

  xhad=4;
  leds[XY(xhad,12 )] = CRGB(0,255,0);

  xhad=5;
  leds[XY(xhad,8 )] = CRGB(0,255,0);
  leds[XY(xhad,12 )] = CRGB(0,255,0);

  xhad=7;
  leds[XY(xhad,11 )] = CRGB(0,255,0);
  leds[XY(xhad,13 )] = CRGB(0,255,0);

  for(xhad = 7;xhad<12;xhad++){
    leds[XY(xhad,13 )] = CRGB(70,255,0);
  }

  xhad=12;
  leds[XY(xhad,9 )] = CRGB(0,255,0);
  leds[XY(xhad,13 )] = CRGB(0,255,0);

  xhad=13;
  leds[XY(xhad,11 )] = CRGB(0,255,0);
  leds[XY(xhad,14 )] = CRGB(0,255,0);

  xhad=14;
  leds[XY(xhad,10 )] = CRGB(0,255,0);

}
//===========================================================================================================================krkhada1
void krkhada1(){
  //tmave zelena 0,80, 0
  //svetle zelena 70,255,0
  //bile zelena 162,255, 196
  //zelena 0,255,0
  //tmave cervena 133,0,0
  //cervena 255,0,0

  //tmave zelena====================================
  int xhad=2;
  leds[XY(xhad,6 )] = CRGB(0,80, 0);
  leds[XY(xhad,7 )] = CRGB(0,80, 0);

  xhad=3;
  leds[XY(xhad,4 )] = CRGB(0,80, 0);
  leds[XY(xhad,5 )] = CRGB(0,80, 0);

  xhad=4;
  leds[XY(xhad,3 )] = CRGB(0,80, 0);
  leds[XY(xhad,4 )] = CRGB(0,80, 0);

  xhad=5;
  leds[XY(xhad,3 )] = CRGB(0,80, 0);
  leds[XY(xhad,7 )] = CRGB(0,80, 0);

  xhad=6;
  leds[XY(xhad,2 )] = CRGB(0,80, 0);
  leds[XY(xhad,6 )] = CRGB(0,80, 0);
  leds[XY(xhad,7 )] = CRGB(0,80, 0);

  xhad=7;
  leds[XY(xhad,2 )] = CRGB(0,80, 0);
  leds[XY(xhad,5 )] = CRGB(0,80, 0);

  //svetle zelena====================================
  xhad=3;
  leds[XY(xhad,7 )] = CRGB(70,255,0);

  xhad=7;
  leds[XY(xhad,3 )] = CRGB(70,255,0);
  leds[XY(xhad,4 )] = CRGB(70,255,0);

  // zelena====================================
  xhad=3;
  leds[XY(xhad,6 )] = CRGB(0,255,0);

  xhad=4;
  leds[XY(xhad,5 )] = CRGB(0,255,0);
  leds[XY(xhad,6 )] = CRGB(0,255,0);
  leds[XY(xhad,7 )] = CRGB(0,255,0);

  xhad=5;
  leds[XY(xhad,4 )] = CRGB(0,255,0);
  leds[XY(xhad,5 )] = CRGB(0,255,0);
  leds[XY(xhad,6 )] = CRGB(0,255,0);

  xhad=6;
  leds[XY(xhad,3 )] = CRGB(0,255,0);
  leds[XY(xhad,4 )] = CRGB(0,255,0);
  leds[XY(xhad,5 )] = CRGB(0,255,0);
  
}
//===========================================================================================================================krkhada2
void krkhada2(){
  //tmave zelena 0,80, 0
  //svetle zelena 70,255,0
  //bile zelena 162,255, 196
  //zelena 0,255,0
  //tmave cervena 133,0,0
  //cervena 255,0,0


  //tmave zelena====================================
  int xhad=2;
  leds[XY(xhad,6 )] = CRGB(0,80, 0);
  leds[XY(xhad,7 )] = CRGB(0,80, 0);

  xhad=3;
  leds[XY(xhad,5 )] = CRGB(0,80, 0);

  xhad=4;
  leds[XY(xhad,4 )] = CRGB(0,80, 0);
  xhad=6;
  leds[XY(xhad,7 )] = CRGB(0,80, 0);
  xhad=7;
  leds[XY(xhad,7 )] = CRGB(0,80, 0);

  //svetle zelena====================================
  xhad=3;
  leds[XY(xhad,7 )] = CRGB(70,255,0);

  // zelena====================================
  xhad=3;
  leds[XY(xhad,6 )] = CRGB(0,255,0);

  xhad=4;
  leds[XY(xhad,5 )] = CRGB(0,255,0);
  leds[XY(xhad,6 )] = CRGB(0,255,0);
  leds[XY(xhad,7 )] = CRGB(0,255,0);

  xhad=5;
  leds[XY(xhad,5 )] = CRGB(0,255,0);
  leds[XY(xhad,6 )] = CRGB(0,255,0);
  leds[XY(xhad,7 )] = CRGB(0,255,0);

  xhad=6;
  leds[XY(xhad,5 )] = CRGB(0,255,0);
  leds[XY(xhad,6 )] = CRGB(0,255,0);

}
//===========================================================================================================================hlavahada1
void hlavahada1(){
  //tmave zelena 0,80, 0
  //svetle zelena 70,255,0
  //bile zelena 162,255, 196
  //zelena 0,255,0
  //tmave cervena 133,0,0
  //cervena 255,0,0


  //tmave zelena 0,80, 0
  int xhad=8;
  leds[XY(xhad,1)] = CRGB(0,80, 0);
  leds[XY(xhad,5)] = CRGB(0,80, 0);

  xhad=9;
  leds[XY(xhad,1)] = CRGB(0,80, 0);
  leds[XY(xhad,6)] = CRGB(0,80, 0);

  xhad=10;
  leds[XY(xhad,1)] = CRGB(0,80, 0);
  leds[XY(xhad,6)] = CRGB(0,80, 0);

  xhad=11;
  leds[XY(xhad,2)] = CRGB(0,80, 0);
  leds[XY(xhad,4)] = CRGB(0,80, 0);

  xhad=12;
  leds[XY(xhad,3)] = CRGB(0,80, 0);
  leds[XY(xhad,4)] = CRGB(0,80, 0);

  //svetle zelena 70,255,0
  xhad=8;
  leds[XY(xhad,3 )] = CRGB(70,255,0);
  leds[XY(xhad,4 )] = CRGB(70,255,0);

  xhad=9;
  leds[XY(xhad,2 )] = CRGB(70,255,0);
  leds[XY(xhad,3 )] = CRGB(70,255,0);
  leds[XY(xhad,4 )] = CRGB(70,255,0);

  xhad=10;
  leds[XY(xhad,2 )] = CRGB(70,255,0);
  leds[XY(xhad,3 )] = CRGB(70,255,0);
  leds[XY(xhad,4 )] = CRGB(70,255,0);

  xhad=11;
  leds[XY(xhad,3 )] = CRGB(70,255,0);

  //zelena 0,255,0

  xhad=9;
  leds[XY(xhad,5 )] = CRGB(0,255,0);

  //bile zelena 162,255, 196

  xhad=8;
  leds[XY(xhad,2 )] = CRGB(162,255, 196);

  xhad=12;
  leds[XY(xhad,2 )] = CRGB(162,255, 196);

  //tmave cervena 133,0,0

  xhad=10;
  leds[XY(xhad,5 )] = CRGB(133,0,0);

  //cervena 255,0,0
  xhad=11;
  leds[XY(xhad,6 )] = CRGB(255,0,0);
}
//===========================================================================================================================hlavahada2
void hlavahada2(){
  //tmave zelena 0,80, 0
  //svetle zelena 70,255,0
  //bile zelena 162,255, 196
  //zelena 0,255,0
  //tmave cervena 133,0,0
  //cervena 255,0,0


  //tmave zelena 0,80, 0
  int xhad=8;
  leds[XY(xhad,5)] = CRGB(0,80, 0);

  xhad=9;
  leds[XY(xhad,5)] = CRGB(0,80, 0);
  leds[XY(xhad,1)] = CRGB(0,80, 0);

  xhad=9;
  leds[XY(xhad,1)] = CRGB(0,80, 0);
  leds[XY(xhad,4)] = CRGB(0,80, 0);
  leds[XY(xhad,5)] = CRGB(0,80, 0);
  leds[XY(xhad,6)] = CRGB(0,80, 0);

  xhad=10;
  leds[XY(xhad,0)] = CRGB(0,80, 0);
  leds[XY(xhad,3)] = CRGB(0,80, 0);
  leds[XY(xhad,6)] = CRGB(0,80, 0);

  xhad=11;
  leds[XY(xhad,0)] = CRGB(0,80, 0);
  leds[XY(xhad,3)] = CRGB(0,80, 0);
  leds[XY(xhad,6)] = CRGB(0,80, 0);

  xhad=12;
  leds[XY(xhad,1)] = CRGB(0,80, 0);
  leds[XY(xhad,2)] = CRGB(0,80, 0);

  //svetle zelena 70,255,0

  xhad=8;
  leds[XY(xhad,2 )] = CRGB(70,255,0);
  leds[XY(xhad,3 )] = CRGB(70,255,0);
  leds[XY(xhad,4 )] = CRGB(70,255,0);

  xhad=9;
  leds[XY(xhad,2 )] = CRGB(70,255,0);
  leds[XY(xhad,3 )] = CRGB(70,255,0);

  xhad=10;
  leds[XY(xhad,1 )] = CRGB(70,255,0);
  leds[XY(xhad,2 )] = CRGB(70,255,0);

  xhad=11;
  leds[XY(xhad,1 )] = CRGB(70,255,0);

  //zelena 0,255,0

  xhad=11;
  leds[XY(xhad,2 )] = CRGB(0,255,0);

  //bile zelena 162,255, 196

  xhad=8;
  leds[XY(xhad,1 )] = CRGB(162,255, 196);

  xhad=12;
  leds[XY(xhad,0 )] = CRGB(162,255, 196);

  //tmave cervena 133,0,0
  xhad=10;
  leds[XY(xhad,4 )] = CRGB(133,0,0);
  leds[XY(xhad,5 )] = CRGB(133,0,0);

  //cervena 255,0,0
  xhad=11;
  leds[XY(xhad,5 )] = CRGB(255,0,0);

  xhad=12;
  leds[XY(xhad,5 )] = CRGB(255,0,0);

  xhad=13;
  leds[XY(xhad,6 )] = CRGB(255,0,0);
  
}
//===========================================================================================================================hlavahada3
void hlavahada3(){
  //tmave zelena 0,80, 0
  //svetle zelena 70,255,0
  //bile zelena 162,255, 196
  //zelena 0,255,0
  //tmave cervena 133,0,0
  //cervena 255,0,0

   //tmave zelena 0,80, 0
  int xhad=5;
  leds[XY(xhad,3)] = CRGB(0,80, 0);

  xhad=6;
  leds[XY(xhad,3)] = CRGB(0,80, 0);

  xhad=7;
  leds[XY(xhad,3)] = CRGB(0,80, 0);
  leds[XY(xhad,6)] = CRGB(0,80, 0);

  for(xhad = 8;xhad<=11;xhad++){
  leds[XY(xhad,2)] = CRGB(0,80, 0);
  leds[XY(xhad,6)] = CRGB(0,80, 0);
  }

  xhad=12;
  leds[XY(xhad,3)] = CRGB(0,80, 0);
  leds[XY(xhad,6)] = CRGB(0,80, 0);

  xhad=13;
  leds[XY(xhad,4)] = CRGB(0,80, 0);
  leds[XY(xhad,5)] = CRGB(0,80, 0);
  
  //svetle zelena 70,255,0

  xhad=8;
  leds[XY(xhad,4 )] = CRGB(70,255,0);
  leds[XY(xhad,5 )] = CRGB(70,255,0);

  xhad=9;
  leds[XY(xhad,4 )] = CRGB(70,255,0);
  leds[XY(xhad,5 )] = CRGB(70,255,0);

  xhad=10;
  leds[XY(xhad,3 )] = CRGB(70,255,0);
  leds[XY(xhad,4 )] = CRGB(70,255,0);
  leds[XY(xhad,5 )] = CRGB(70,255,0);

  xhad=11;
  leds[XY(xhad,3 )] = CRGB(70,255,0);
  leds[XY(xhad,4 )] = CRGB(70,255,0);
  leds[XY(xhad,5 )] = CRGB(70,255,0);

  xhad=12;
  leds[XY(xhad,4 )] = CRGB(70,255,0);
  leds[XY(xhad,5 )] = CRGB(70,255,0);


  //zelena 0,255,0

  xhad=5;
  leds[XY(xhad,4 )] = CRGB(0,255,0);

  xhad=6;
  leds[XY(xhad,4 )] = CRGB(0,255,0);

  xhad=7;
  leds[XY(xhad,4 )] = CRGB(0,255,0);
  leds[XY(xhad,5 )] = CRGB(0,255,0);

  xhad=8;
  leds[XY(xhad,3 )] = CRGB(0,255,0);

  //bile zelena 162,255, 196

  xhad=9;
  leds[XY(xhad,3 )] = CRGB(162,255, 196);

  xhad=13;
  leds[XY(xhad,3 )] = CRGB(162,255, 196);


}
//===========================================================================================================================hlavahada4
void hlavahada4(){
  //tmave zelena 0,80, 0
  //svetle zelena 70,255,0
  //bile zelena 162,255, 196
  //zelena 0,255,0
  //tmave cervena 133,0,0
  //cervena 255,0,0

  //tmave zelena 0,80, 0
  int xhad=5;
  leds[XY(xhad,4)] = CRGB(0,80, 0);

  xhad=6;
  leds[XY(xhad,3)] = CRGB(0,80, 0);

  xhad=7;
  leds[XY(xhad,3)] = CRGB(0,80, 0);

  for(xhad = 8;xhad<=11;xhad++){
  leds[XY(xhad,3)] = CRGB(0,80, 0);
  leds[XY(xhad,7)] = CRGB(0,80, 0);
  }

  xhad=12;
  leds[XY(xhad,4)] = CRGB(0,80, 0);
  leds[XY(xhad,7)] = CRGB(0,80, 0);

  xhad=13;
  leds[XY(xhad,5)] = CRGB(0,80, 0);
  leds[XY(xhad,6)] = CRGB(0,80, 0);

  //svetle zelena 70,255,0

  xhad=8;
  leds[XY(xhad,5 )] = CRGB(70,255,0);

  xhad=9;
  leds[XY(xhad,5 )] = CRGB(70,255,0);
  leds[XY(xhad,6 )] = CRGB(70,255,0);

  xhad=10;
  leds[XY(xhad,4 )] = CRGB(70,255,0);
  leds[XY(xhad,5 )] = CRGB(70,255,0);
  leds[XY(xhad,6 )] = CRGB(70,255,0);

  xhad=11;
  leds[XY(xhad,4 )] = CRGB(70,255,0);
  leds[XY(xhad,5 )] = CRGB(70,255,0);
  leds[XY(xhad,6 )] = CRGB(70,255,0);

  xhad=12;
  leds[XY(xhad,5 )] = CRGB(70,255,0);
  leds[XY(xhad,6 )] = CRGB(70,255,0);

  //zelena 0,255,0

  xhad=6;
  leds[XY(xhad,4 )] = CRGB(0,255,0);

  xhad=7;
  leds[XY(xhad,4 )] = CRGB(0,255,0);
  leds[XY(xhad,5 )] = CRGB(0,255,0);
  leds[XY(xhad,6 )] = CRGB(0,255,0);

  xhad=8;
  leds[XY(xhad,4 )] = CRGB(0,255,0);
  leds[XY(xhad,6 )] = CRGB(0,255,0);

  //bile zelena 162,255, 196

  xhad=9;
  leds[XY(xhad,4 )] = CRGB(162,255, 196);

  xhad=13;
  leds[XY(xhad,4 )] = CRGB(162,255, 196);
}

//===========================================================================================================================mainanimacehada
void mainanimacehada (){
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0, 15, 15);
  }

    if(animacehada == 0){
      telohada2();
      krkhada2();
      hlavahada4();
      animacehada++;
    }
    else if(animacehada == 1){
      telohada2();
      krkhada2();
      hlavahada3();
      animacehada++;
    }
    else if(animacehada == 2){
      telohada1();
      krkhada1();
      hlavahada1();
      animacehada++;
    }
    else if(animacehada == 3){
      telohada1();
      krkhada1();
      hlavahada2();
      animacehada++;
    }
    else if(animacehada == 4){
      telohada1();
      krkhada1();
      hlavahada1();
      animacehada++;
    }
    else if(animacehada == 5){
      telohada2();
      krkhada2();
      hlavahada3();
      animacehada++;
    }
    else if(animacehada == 6){
      telohada2();
      krkhada2();
      hlavahada4();
      animacehada++;
    }
    if(animacehada >= 6){
      animacehada =0;
    }
}

void animacemikrofon(){
 //bila 255,255,255
 //seda 130,130,130
 //tmaveseda 80,80,80
 for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(15, 0, 15);
  }


   //bila 255,255,255
  int xmik=0;
  leds[XY(xmik,7)] = CRGB(255,255, 255);

  xmik=1;
  leds[XY(xmik,7)] = CRGB(255,255, 255);
  leds[XY(xmik,6)] = CRGB(255,255, 255);
  leds[XY(xmik,5)] = CRGB(255,255, 255);
  leds[XY(xmik,4)] = CRGB(255,255, 255);

  xmik=2;
  leds[XY(xmik,5)] = CRGB(255,255, 255);

  xmik=4;
  leds[XY(xmik,4)] = CRGB(255,255, 255);
  leds[XY(xmik,9)] = CRGB(255,255, 255);
  leds[XY(xmik,10)] = CRGB(255,255, 255);

  xmik=5;
  leds[XY(xmik,4)] = CRGB(255,255, 255);
  leds[XY(xmik,3)] = CRGB(255,255, 255);
  leds[XY(xmik,2)] = CRGB(255,255, 255);
  leds[XY(xmik,1)] = CRGB(255,255, 255);
  leds[XY(xmik,8)] = CRGB(255,255, 255);
  leds[XY(xmik,9)] = CRGB(255,255, 255);

  xmik=6;
  leds[XY(xmik,2)] = CRGB(255,255, 255);
  leds[XY(xmik,7)] = CRGB(255,255, 255);
  leds[XY(xmik,8)] = CRGB(255,255, 255);

  xmik=7;
  leds[XY(xmik,6)] = CRGB(255,255, 255);
  leds[XY(xmik,7)] = CRGB(255,255, 255);

  xmik=10;
  leds[XY(xmik,2)] = CRGB(255,255, 255);

  xmik=11;
  leds[XY(xmik,1)] = CRGB(255,255, 255);
  leds[XY(xmik,2)] = CRGB(255,255, 255);
  leds[XY(xmik,3)] = CRGB(255,255, 255);

  xmik=12;
  leds[XY(xmik,2)] = CRGB(255,255, 255);

  //seda 130,130,130

  xmik=4;
  leds[XY(xmik,11)] = CRGB(130,130,130);

  xmik=5;
  leds[XY(xmik,11)] = CRGB(130,130,130);
  leds[XY(xmik,10)] = CRGB(130,130,130);

  xmik=6;
  leds[XY(xmik,11)] = CRGB(130,130,130);

  xmik=7;
  leds[XY(xmik,10)] = CRGB(130,130,130);

  xmik=8;
  for(int i = 5;i<=9;i++){
  leds[XY(xmik,i)] = CRGB(130,130,130);
  }

  xmik=9;
  leds[XY(xmik,8)] = CRGB(130,130,130);
  leds[XY(xmik,7)] = CRGB(130,130,130);
  leds[XY(xmik,6)] = CRGB(130,130,130);
  leds[XY(xmik,2)] = CRGB(130,130,130);

  xmik=10;
  leds[XY(xmik,7)] = CRGB(130,130,130);
  leds[XY(xmik,3)] = CRGB(130,130,130);
  leds[XY(xmik,1)] = CRGB(130,130,130);

  xmik=11;
  leds[XY(xmik,4)] = CRGB(130,130,130);
  leds[XY(xmik,0)] = CRGB(130,130,130);

  xmik=12;
  leds[XY(xmik,3)] = CRGB(130,130,130);
  leds[XY(xmik,1)] = CRGB(130,130,130);

  xmik=13;
  leds[XY(xmik,2)] = CRGB(130,130,130);

  //tmaveseda 80,80,80
  xmik=3;
  for(int i = 9;i<=13;i++){
  leds[XY(xmik,i)] = CRGB(80,80,80);
  }

  xmik=4;
  leds[XY(xmik,14)] = CRGB(80,80,80);
  leds[XY(xmik,12)] = CRGB(80,80,80);
  leds[XY(xmik,8)] = CRGB(80,80,80);

  xmik=5;
  leds[XY(xmik,15)] = CRGB(80,80,80);
  leds[XY(xmik,12)] = CRGB(80,80,80);
  leds[XY(xmik,7)] = CRGB(80,80,80);

  xmik=6;
  leds[XY(xmik,15)] = CRGB(80,80,80);
  leds[XY(xmik,12)] = CRGB(80,80,80);
  leds[XY(xmik,10)] = CRGB(80,80,80);
  leds[XY(xmik,9)] = CRGB(80,80,80);
  leds[XY(xmik,6)] = CRGB(80,80,80);

  xmik=7;
  leds[XY(xmik,15)] = CRGB(80,80,80);
  leds[XY(xmik,11)] = CRGB(80,80,80);
  leds[XY(xmik,9)] = CRGB(80,80,80);
  leds[XY(xmik,8)] = CRGB(80,80,80);
  leds[XY(xmik,5)] = CRGB(80,80,80);

  xmik=8;
  leds[XY(xmik,14)] = CRGB(80,80,80);
  leds[XY(xmik,10)] = CRGB(80,80,80);
  for(int i = 2;i<=4;i++){
  leds[XY(xmik,i)] = CRGB(80,80,80);
  }

  xmik=9;
  leds[XY(xmik,13)] = CRGB(80,80,80);
  leds[XY(xmik,9)] = CRGB(80,80,80);
  leds[XY(xmik,5)] = CRGB(80,80,80);
  leds[XY(xmik,4)] = CRGB(80,80,80);
  leds[XY(xmik,3)] = CRGB(80,80,80);
  leds[XY(xmik,1)] = CRGB(80,80,80);

  xmik=10;
  leds[XY(xmik,12)] = CRGB(80,80,80);
  leds[XY(xmik,8)] = CRGB(80,80,80);
  leds[XY(xmik,6)] = CRGB(80,80,80);
  leds[XY(xmik,5)] = CRGB(80,80,80);
  leds[XY(xmik,4)] = CRGB(80,80,80);
  leds[XY(xmik,0)] = CRGB(80,80,80);

  xmik=11;
  leds[XY(xmik,11)] = CRGB(80,80,80);
  leds[XY(xmik,7)] = CRGB(80,80,80);
  leds[XY(xmik,6)] = CRGB(80,80,80);
  leds[XY(xmik,5)] = CRGB(80,80,80);

  xmik=12;
  leds[XY(xmik,11)] = CRGB(80,80,80);
  leds[XY(xmik,0)] = CRGB(80,80,80);
  for(int i = 4;i<=7;i++){
  leds[XY(xmik,i)] = CRGB(80,80,80);
  }

  xmik=13;
  leds[XY(xmik,11)] = CRGB(80,80,80);
  leds[XY(xmik,15)] = CRGB(80,80,80);
  leds[XY(xmik,1)] = CRGB(80,80,80);
  leds[XY(xmik,0)] = CRGB(80,80,80);
  for(int i = 3;i<=7;i++){
  leds[XY(xmik,i)] = CRGB(80,80,80);
  }

  xmik=14;
  for(int i = 12;i<=14;i++){
  leds[XY(xmik,i)] = CRGB(80,80,80);
  }
  for(int i = 1;i<=6;i++){
  leds[XY(xmik,i)] = CRGB(80,80,80);
  }

  xmik=15;
  for(int i = 2;i<=5;i++){
  leds[XY(xmik,i)] = CRGB(80,80,80);
  }

}

void joinwifi(){
  vykreslicislo(1,5, 1, 255,255,255);
  vykreslicislo(5,5, 0, 255,255,255);
  vykreslicislo(7,5, 1, 255,255,255);
  vykreslicislo(9,5, 1, 255,255,255);
  vykreslicislo(12,5, 1, 255,255,255);
  vykreslicislo(5,13, 1, 255,255,255);
  vykreslicislo(7,13, 1, 255,255,255);
  vykreslicislo(11,13, 1, 255,255,255);

  int xwifi=1;
  for(int i = 4;i<=5;i++){
  leds[XY(xwifi,i)] = CRGB(255,255,255);
  }
  for(int i = 9;i<=11;i++){
  leds[XY(xwifi,i)] = CRGB(255,255,255);
  }

  xwifi=2;
  for(int i = 12;i<=13;i++){
  leds[XY(xwifi,i)] = CRGB(255,255,255);
  }
  leds[XY(xwifi,5)] = CRGB(255,255,255);

  xwifi=3;
  for(int i = 10;i<=11;i++){
  leds[XY(xwifi,i)] = CRGB(255,255,255);
  }

  xwifi=4;
  for(int i = 12;i<=13;i++){
  leds[XY(xwifi,i)] = CRGB(255,255,255);
  }

  xwifi=5;
  for(int i = 9;i<=11;i++){
  leds[XY(xwifi,i)] = CRGB(255,255,255);
  }

  xwifi=10;
  leds[XY(xwifi,9)] = CRGB(255,255,255);
  leds[XY(xwifi,11)] = CRGB(255,255,255);

  xwifi=11;
  leds[XY(xwifi,9)] = CRGB(255,255,255);
  leds[XY(xwifi,11)] = CRGB(255,255,255);

  xwifi=12;
  leds[XY(xwifi,2)] = CRGB(255,255,255);

  xwifi=13;
  leds[XY(xwifi,3)] = CRGB(255,255,255);
  leds[XY(xwifi,4)] = CRGB(255,255,255);
}

void tuzka(){
  //cerna 0,0,0
  //zluta 255,255,0
  //tmave zluta 100,100,0
  //cervema 255,0,0
  //tmave cervena 100,0,0
  //bila 255,255,255
  //seda 80,80,80
  //bezova 255,180,120

  for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(0, 15, 15);
  }

  //bezova 255,180,120
  int xpen=4;
  leds[XY(xpen,8)] = CRGB(255,180,120);
  leds[XY(xpen,9)] = CRGB(255,180,120);

  xpen=5;
  leds[XY(xpen,9)] = CRGB(255,180,120);
  leds[XY(xpen,10)] = CRGB(255,180,120);

  xpen=6;
  leds[XY(xpen,10)] = CRGB(255,180,120);

  //cerna 0,0,0
  int ypen=10;
  leds[XY(4,ypen)] = CRGB(0,0,0);

  ypen=11;
  leds[XY(12,ypen)] = CRGB(0,0,0);
  leds[XY(13,ypen)] = CRGB(0,0,0);

  ypen=12;
  leds[XY(4,ypen)] = CRGB(0,0,0);
  leds[XY(5,ypen)] = CRGB(0,0,0);
  leds[XY(6,ypen)] = CRGB(0,0,0);
  leds[XY(7,ypen)] = CRGB(0,0,0);
  leds[XY(10,ypen)] = CRGB(0,0,0);
  leds[XY(11,ypen)] = CRGB(0,0,0);
  leds[XY(14,ypen)] = CRGB(0,0,0);
  
  ypen=13;
  leds[XY(8,ypen)] = CRGB(0,0,0);
  leds[XY(9,ypen)] = CRGB(0,0,0);
  

  //tmave zluta 100,100,0

  xpen=4;
  leds[XY(xpen,7)] = CRGB(100,100,0);

  xpen=5;
  leds[XY(xpen,7)] = CRGB(100,100,0);
  leds[XY(xpen,6)] = CRGB(100,100,0);

  xpen=6;
  leds[XY(xpen,6)] = CRGB(100,100,0);
  leds[XY(xpen,5)] = CRGB(100,100,0);

  xpen=7;
  leds[XY(xpen,10)] = CRGB(100,100,0);
  leds[XY(xpen,9)] = CRGB(100,100,0);
  leds[XY(xpen,5)] = CRGB(100,100,0);
  leds[XY(xpen,4)] = CRGB(100,100,0);

  xpen=8;
  leds[XY(xpen,9)] = CRGB(100,100,0);
  leds[XY(xpen,8)] = CRGB(100,100,0);
  leds[XY(xpen,4)] = CRGB(100,100,0);
  leds[XY(xpen,3)] = CRGB(100,100,0);

  xpen=9;
  leds[XY(xpen,8)] = CRGB(100,100,0);
  leds[XY(xpen,7)] = CRGB(100,100,0);
  leds[XY(xpen,3)] = CRGB(100,100,0);

  xpen=10;
  leds[XY(xpen,6)] = CRGB(100,100,0);
  leds[XY(xpen,7)] = CRGB(100,100,0);

  xpen=11;
  leds[XY(xpen,6)] = CRGB(100,100,0);
  leds[XY(xpen,5)] = CRGB(100,100,0);

  //zluta 255,255,0

  xpen=5;
  leds[XY(xpen,8)] = CRGB(255,255,0);

  xpen=6;
  leds[XY(xpen,7)] = CRGB(255,255,0);
  leds[XY(xpen,8)] = CRGB(255,255,0);
  leds[XY(xpen,9)] = CRGB(255,255,0);

  xpen=7;
  leds[XY(xpen,6)] = CRGB(255,255,0);
  leds[XY(xpen,7)] = CRGB(255,255,0);
  leds[XY(xpen,8)] = CRGB(255,255,0);

  xpen=8;
  leds[XY(xpen,5)] = CRGB(255,255,0);
  leds[XY(xpen,6)] = CRGB(255,255,0);
  leds[XY(xpen,7)] = CRGB(255,255,0);

  xpen=9;
  leds[XY(xpen,4)] = CRGB(255,255,0);
  leds[XY(xpen,5)] = CRGB(255,255,0);
  leds[XY(xpen,6)] = CRGB(255,255,0);

  xpen=10;
  leds[XY(xpen,4)] = CRGB(255,255,0);
  leds[XY(xpen,5)] = CRGB(255,255,0);

  //bila 255,255,255

  xpen=10;
  leds[XY(xpen,3)] = CRGB(255,255,255);

  xpen=11;
  leds[XY(xpen,2)] = CRGB(255,255,255);
  leds[XY(xpen,3)] = CRGB(255,255,255);
  leds[XY(xpen,4)] = CRGB(255,255,255);

  xpen=12;
  leds[XY(xpen,2)] = CRGB(255,255,255);
  leds[XY(xpen,3)] = CRGB(255,255,255);

  //seda 80,80,80

  xpen=9;
  leds[XY(xpen,2)] = CRGB(80,80,80);

  xpen=10;
  leds[XY(xpen,1)] = CRGB(80,80,80);
  leds[XY(xpen,2)] = CRGB(80,80,80);

  xpen=11;
  leds[XY(xpen,1)] = CRGB(80,80,80);
  
  xpen=12;
  leds[XY(xpen,4)] = CRGB(80,80,80);
  leds[XY(xpen,5)] = CRGB(80,80,80);

  xpen=13;
  leds[XY(xpen,3)] = CRGB(80,80,80);
  leds[XY(xpen,4)] = CRGB(80,80,80);

  //cervema 255,0,0

  xpen=12;
  leds[XY(xpen,1)] = CRGB(255,0,0);

  xpen=13;
  leds[XY(xpen,1)] = CRGB(255,0,0);
  leds[XY(xpen,2)] = CRGB(255,0,0);

  //tmave cervena 100,0,0

  xpen=11;
  leds[XY(xpen,0)] = CRGB(100,0,0);

  xpen=12;
  leds[XY(xpen,0)] = CRGB(100,0,0);

  xpen=14;
  leds[XY(xpen,2)] = CRGB(100,0,0);
  leds[XY(xpen,3)] = CRGB(100,0,0);
}

void pacman(){

  for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(0, 0, 15);
  }

  if(poradianimace == 0){
    pacman1();
    poradianimace++;
  }

  else if(poradianimace == 1){
    pacman2();
    poradianimace++;
  }

  else if(poradianimace == 2){
    pacman3();
    poradianimace++;
  }

  else if(poradianimace == 3){
    pacman2();
    poradianimace++;
  }
  
  if(poradianimace > 3){
    poradianimace = 0;
  }
}

void pacman1(){
  //zluta 255,255,0
  
  for(int i = 5;i<=10;i++){
  leds[XY(1,i)] = CRGB(255,255,0);
  leds[XY(14,i)] = CRGB(255,255,0);
  }

  for(int i = 3;i<=12;i++){
  leds[XY(2,i)] = CRGB(255,255,0);
  leds[XY(13,i)] = CRGB(255,255,0);
  }

  for(int i = 2;i<=13;i++){
  leds[XY(3,i)] = CRGB(255,255,0);
  leds[XY(4,i)] = CRGB(255,255,0);
  leds[XY(11,i)] = CRGB(255,255,0);
  leds[XY(12,i)] = CRGB(255,255,0);
  }

  for(int i = 1;i<=14;i++){
  leds[XY(5,i)] = CRGB(255,255,0);
  leds[XY(6,i)] = CRGB(255,255,0);
  leds[XY(7,i)] = CRGB(255,255,0);
  leds[XY(8,i)] = CRGB(255,255,0);
  leds[XY(9,i)] = CRGB(255,255,0);
  leds[XY(10,i)] = CRGB(255,255,0);
  }
}

void pacman2(){
  //zluta 255,255,0

  for(int i = 5;i<=10;i++){
  leds[XY(1,i)] = CRGB(255,255,0);
  }

  for(int i = 3;i<=12;i++){
  leds[XY(2,i)] = CRGB(255,255,0);
  }

  for(int i = 2;i<=13;i++){
  leds[XY(3,i)] = CRGB(255,255,0);
  leds[XY(4,i)] = CRGB(255,255,0);
  }

  for(int i = 1;i<=14;i++){
  leds[XY(5,i)] = CRGB(255,255,0);
  }

  int xpac=6;
  for(int i = 1;i<=6;i++){
  leds[XY(xpac,i)] = CRGB(255,255,0);
  }

  for(int i = 8;i<=14;i++){
  leds[XY(xpac,i)] = CRGB(255,255,0);
  }

  xpac=7;
  for(int i = 1;i<=6;i++){
  leds[XY(xpac,i)] = CRGB(255,255,0);
  }

  for(int i = 8;i<=14;i++){
  leds[XY(xpac,i)] = CRGB(255,255,0);
  }

  xpac=8;
  for(int i = 1;i<=5;i++){
  leds[XY(xpac,i)] = CRGB(255,255,0);
  }

  for(int i = 9;i<=14;i++){
  leds[XY(xpac,i)] = CRGB(255,255,0);
  }

  xpac=9;
  for(int i = 1;i<=5;i++){
  leds[XY(xpac,i)] = CRGB(255,255,0);
  }

  for(int i = 9;i<=14;i++){
  leds[XY(xpac,i)] = CRGB(255,255,0);
  }

  xpac=10;
  for(int i = 1;i<=5;i++){
  leds[XY(xpac,i)] = CRGB(255,255,0);
  }

  for(int i = 9;i<=14;i++){
  leds[XY(xpac,i)] = CRGB(255,255,0);
  }

  xpac=11;
  for(int i = 2;i<=5;i++){
  leds[XY(xpac,i)] = CRGB(255,255,0);
  }

  for(int i = 9;i<=13;i++){
  leds[XY(xpac,i)] = CRGB(255,255,0);
  }

  xpac=12;
  for(int i = 2;i<=5;i++){
  leds[XY(xpac,i)] = CRGB(255,255,0);
  }

  for(int i = 9;i<=13;i++){
  leds[XY(xpac,i)] = CRGB(255,255,0);
  }

  xpac=13;
  for(int i = 3;i<=4;i++){
  leds[XY(xpac,i)] = CRGB(255,255,0);
  }

  for(int i = 10;i<=12;i++){
  leds[XY(xpac,i)] = CRGB(255,255,0);
  }

  xpac=14;
  leds[XY(xpac,4)] = CRGB(255,255,0);
  leds[XY(xpac,10)] = CRGB(255,255,0);
}

void pacman3(){
//zluta 255,255,0

  for(int i = 5;i<=10;i++){
  leds[XY(1,i)] = CRGB(255,255,0);
  }

  for(int i = 3;i<=12;i++){
  leds[XY(2,i)] = CRGB(255,255,0);
  }

  for(int i = 2;i<=13;i++){
  leds[XY(3,i)] = CRGB(255,255,0);
  leds[XY(4,i)] = CRGB(255,255,0);
  }

  for(int i = 1;i<=14;i++){
  leds[XY(5,i)] = CRGB(255,255,0);
  }

  int xpac=6;
  for(int i = 1;i<=6;i++){
  leds[XY(xpac,i)] = CRGB(255,255,0);
  }

  for(int i = 9;i<=14;i++){
  leds[XY(xpac,i)] = CRGB(255,255,0);
  }

  xpac=7;
  for(int i = 1;i<=6;i++){
  leds[XY(xpac,i)] = CRGB(255,255,0);
  }

  for(int i = 9;i<=14;i++){
  leds[XY(xpac,i)] = CRGB(255,255,0);
  }

  xpac=8;
  for(int i = 1;i<=5;i++){
  leds[XY(xpac,i)] = CRGB(255,255,0);
  }

  for(int i = 10;i<=14;i++){
  leds[XY(xpac,i)] = CRGB(255,255,0);
  }

  xpac=9;
  for(int i = 1;i<=5;i++){
  leds[XY(xpac,i)] = CRGB(255,255,0);
  }

  for(int i = 10;i<=14;i++){
  leds[XY(xpac,i)] = CRGB(255,255,0);
  }

  xpac=10;
  for(int i = 1;i<=4;i++){
  leds[XY(xpac,i)] = CRGB(255,255,0);
  }

  for(int i = 11;i<=14;i++){
  leds[XY(xpac,i)] = CRGB(255,255,0);
  }

  xpac=11;
  for(int i = 2;i<=4;i++){
  leds[XY(xpac,i)] = CRGB(255,255,0);
  }

  for(int i = 11;i<=13;i++){
  leds[XY(xpac,i)] = CRGB(255,255,0);
  }

  xpac=12;
  for(int i = 2;i<=3;i++){
  leds[XY(xpac,i)] = CRGB(255,255,0);
  }

  for(int i = 12;i<=13;i++){
  leds[XY(xpac,i)] = CRGB(255,255,0);
  }

  xpac=13;
  leds[XY(xpac,3)] = CRGB(255,255,0);
  leds[XY(xpac,12)] = CRGB(255,255,0);
}