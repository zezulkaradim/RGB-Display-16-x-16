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
const canvas=document.getElementById("c");
const ctx=canvas.getContext("2d");
ctx.imageSmoothingEnabled=false;
let drawing=false,brush=1,erase=false;
function setBrush(s){brush=s;erase=false;}
function eraser(){erase=true;}
function hexToRgb(h){const n=parseInt(h.slice(1),16);return[(n>>16)&255,(n>>8)&255,n&255];}
function draw(cx,cy){
  const r=canvas.getBoundingClientRect();
  const px=Math.floor((cx-r.left)/r.width*16),py=Math.floor((cy-r.top)/r.height*16);
  const half=Math.floor(brush/2),col=erase?[0,0,0]:hexToRgb(document.getElementById("color").value);
  let pixels=[];
  for(let y=-half;y<=half;y++)for(let x=-half;x<=half;x++){
    const qx=px+x,qy=py+y;
    if(qx<0||qy<0||qx>=16||qy>=16)continue;
    ctx.fillStyle=erase?"#000":document.getElementById("color").value;
    ctx.fillRect(qx,qy,1,1);
    pixels.push({x:qx,y:qy,r:col[0],g:col[1],b:col[2]});
  }
  if(pixels.length)fetch("/update",{method:"POST",headers:{"Content-Type":"application/json"},body:JSON.stringify({pixels})});
}
["mousedown","touchstart"].forEach(e=>canvas.addEventListener(e,ev=>{drawing=true;const p=ev.touches?ev.touches[0]:ev;draw(p.clientX,p.clientY);ev.preventDefault();}));
["mousemove","touchmove"].forEach(e=>canvas.addEventListener(e,ev=>{if(!drawing)return;const p=ev.touches?ev.touches[0]:ev;draw(p.clientX,p.clientY);ev.preventDefault();}));
["mouseup","mouseleave","touchend"].forEach(e=>canvas.addEventListener(e,()=>drawing=false));
function clearPanel(){ctx.fillStyle="#000";ctx.fillRect(0,0,16,16);fetch("/clear",{method:"POST"});}
</script>
</body>
</html>
)rawliteral";

void handleRoot()   { server.send_P(200, "text/html", MAIN_page); }

void handleUpdate() {
  if (!server.hasArg("plain")) { server.send(400,"text/plain","NO DATA"); return; }
  DynamicJsonDocument doc(2048);
  if (deserializeJson(doc, server.arg("plain"))) { server.send(400,"text/plain","BAD JSON"); return; }
  for (JsonObject p : doc["pixels"].as<JsonArray>()) {
    uint8_t x=p["x"],y=p["y"],r=p["r"],g=p["g"],b=p["b"];
    if (x<WIDTH && y<HEIGHT) { ledBuffer[x][y]=CRGB(r,g,b); needsShow=true; }
  }
  server.send(200,"text/plain","OK");
}

void handleClear() {
  for(uint8_t y=0;y<HEIGHT;y++) for(uint8_t x=0;x<WIDTH;x++) ledBuffer[x][y]=CRGB::Black;
  needsShow=true; server.send(200,"text/plain","CLEARED");
}

void kresleni() {
  if (!kresleniActive) {
    kresleniActive=true;
    FastLED.clear(true); delay(5);
    WiFi.disconnect(true); delay(100);
    WiFi.mode(WIFI_AP);
    WiFi.softAP("RGB-DISPLAY","12345678");
    server.on("/",       handleRoot);
    server.on("/update", HTTP_POST, handleUpdate);
    server.on("/clear",  HTTP_POST, handleClear);
    server.begin();
    joinwifi();
    FastLED.show();
  }

  server.handleClient();
  static unsigned long lastShow = 0;
  if (millis()-lastShow > 50 && needsShow) {
    for(uint8_t y=0;y<HEIGHT;y++) for(uint8_t x=0;x<WIDTH;x++) leds[XY(x,y)]=ledBuffer[x][y];
    FastLED.show(); needsShow=false; lastShow=millis();
  }
}

void stopKresleni() {
  if (!kresleniActive) return;
  kresleniActive=false;
  server.stop();
  WiFi.softAPdisconnect(true); delay(100);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
}
