#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"
#include "Font_Regular.h"
#include "Font_3x7.h"
#include "Html_page.h"

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW 
#define MAX_ZONES 2
#define MAX_DEVICES 4
#define CLK_PIN   D5 // or SCK
#define DATA_PIN  D7 // or MOSI
#define CS_PIN    D8 // or SS

MD_Parola parola = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES); // Software spi
DNSServer dnsServer;
AsyncWebServer server(80);

const char* ssid     = "";
const char* password = "";

uint8_t curText;
char displayTime[9];    // mm:ss\0
char displayInterval[3];

int counter = -5;
bool started = false;
unsigned int countType = 0;
unsigned int countWork = 0;
unsigned int countRest = 0;
unsigned int countInterval = 30;

class CaptiveRequestHandler : public AsyncWebHandler {
public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request){
    return request->host().indexOf(WiFi.softAPIP().toString()) < 0;
  }

  void handleRequest(AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("text/html");
    response->print("<!DOCTYPE html><html><head><title>Garage Gym Timer</title></head><body>");
    response->print("<p>This is the captive portal front page.</p>");
    response->printf("<p>You were trying to reach: http://%s%s</p>", request->host().c_str(), request->url().c_str());
    response->printf("<p>Try opening <a href='http://%s/settings'>Timer settings page</a> instead</p>", WiFi.softAPIP().toString().c_str());
    response->print("</body></html>");
    request->send(response);
  }
};

void setup(){
  parola.begin(MAX_ZONES);
  parola.setIntensity(4);

  parola.setZone(0, 0, 2); // timer
  parola.setZone(1, 3, 3); // counter

  parola.setFont(0, fontRegular);
  parola.setFont(1, font3x7);
  parola.setFont(2, fontRegular);

  parola.displayZoneText(0, displayTime, PA_RIGHT, 0, 0, PA_PRINT, PA_NO_EFFECT);
  parola.displayZoneText(1, displayInterval, PA_LEFT, 0, 0, PA_PRINT, PA_NO_EFFECT);

  Serial.begin(115200);
  delay(10);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password, 5, false, 2);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  dnsServer.start(53, "*", WiFi.softAPIP());
  server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);
  server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request){
    request -> send_P(200, "text/html", index_html);
  });
  server.on("/start", HTTP_POST, [](AsyncWebServerRequest *request){
    int prepare = request->hasArg("prepare") ? request->arg("prepare").toInt() : 0;
    int work = request->hasArg("work") ? request->arg("work").toInt() : 0;
    int rest = request->hasArg("rest") ? request->arg("rest").toInt() : 0;
    startTimer(prepare, work, rest);
    request -> send(200, "text/plain", "OK");
  });
  server.on("/stop", HTTP_POST, [](AsyncWebServerRequest *request){
    stopTimer();
    request -> send(200, "text/plain", "OK");
  });
  server.onNotFound([](AsyncWebServerRequest *request){
    request -> redirect("/");
  });
  server.begin();
}

bool isTimerStarted() {
  return started;
}

void startTimer(int prepare, int work, int rest) {
  counter = prepare * -1;
  countWork = work;
  countRest = rest;
  countInterval = work + rest;
  started = true;
}

void stopTimer() {
  started = false;
  for (uint8_t i = 0; i < MAX_ZONES; i++) {
    parola.displayClear(i);
  }
}

void loop() {
  dnsServer.processNextRequest();
  static uint32_t lastTime = millis();
  static uint8_t curZone = 0;
  static bool flash = true;

  if (!isTimerStarted()) {
    return;
  }

  parola.displayAnimate();

  if (parola.getZoneStatus(1) && countInterval > 0 && counter > 0) {    
    int started = counter % countInterval > 0;
    int interval = (counter / countInterval) + 1;
    // We want to display the next interval when its started
    int intervalDisplay = started ? interval : interval - 1;
    displayIntervalCounter(intervalDisplay);
    parola.displayReset(1);
  }

  if (parola.getZoneStatus(0)) {
    if (counter == 0) {
      displayStartMessage();
    } else if (counter < 0) {
      unsigned int tick = counter * -1;
      displayPrepareCounter(tick);
    } else if (countInterval > 0) {
      int tick = counter % countInterval;
      bool rest = tick > countWork || tick == 0;
      int started = tick > 0;
      // We want to display the full interval instead of zeroes
      int tickDisplay = started ? tick : countInterval;
      displayUpTime(tickDisplay, !rest && flash);
    } else {
      displayUpTime(counter, flash);
    }
    
    parola.displayReset(0);
  }

  float diff = millis() - lastTime;
  if (diff >= 1000) {
    lastTime = millis();
    counter++;
    flash = true;
  } else if (diff >= 500) {
    flash = false;
  }
  
}

void displayIntervalCounter(int interval) {
  sprintf(displayInterval, "%d", interval);
}

void displayPrepareCounter(int tick) {
  sprintf(displayTime, "%d", tick);
}

void displayUpTime(int tick, bool flash) {
  int seconds = tick % 60;
  int minutes = tick / 60;
  sprintf(displayTime, "%02d%c%02d", minutes, (flash ? ':' : ' '), seconds);
}

void displayStartMessage() {
  sprintf(displayTime, "%s", "Go!");
}
