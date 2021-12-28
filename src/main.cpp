#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h> // Core graphics library
#include <Adafruit_ST7735.h>
#include <Encoder.h>
#include "updater.h"
#include "Screen/CurrentListScreen.h"
#include "Screen/Screen.h"
#include "main.h"

#if defined(ARDUINO_FEATHER_ESP32) // Feather Huzzah32
#define TFT_CS 14
#define TFT_RST 15
#define TFT_DC 32

#elif defined(ESP8266)
#define TFT_CS D8
#define TFT_RST D6
#define TFT_DC D3

#else
// For the breakout board, you can use any 2 or 3 pins.
// These pins will also work for the 1.8" TFT shield.
#define TFT_CS 10
#define TFT_RST 9 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC 8
#endif

// Arduino_DataBus *bus = new Arduino_HWSPI(D3 /* DC */, D8 /* CS */);
//Arduino_ST7735 *gfx = new Arduino_ST7735(bus, D6 /* RST */);

Adafruit_ST7735 gfx = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
uint8_t a = 0;

int lCnt = 0;          // Keep track of last rotary value
volatile int vPos = 0; // Updated by the ISR (Interrupt Service Routine)
int cmd = 0;
const int RIGHT = -1;
const int LEFT = 1;
const int CLICK = 2;
const int LONGCLICK = 3;
long lastInput;
static Screen *currentScreen;
static Screen *previousScreen;
unsigned long timePressBegin;
unsigned long pressTime = 500;

void ICACHE_RAM_ATTR interruptButtonBegin();
void ICACHE_RAM_ATTR interruptButton();
void ICACHE_RAM_ATTR interruptWheel();
void left();
void right();
void showList();
bool isBounce();
void showRelease();
void checkDeepSleep();

void setup()
{
  Serial.println("\nStarting up...");
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  pinMode(D8, INPUT_PULLDOWN_16);
  pinMode(D4, INPUT_PULLDOWN_16);
  pinMode(D0, INPUT_PULLDOWN_16);

  attachInterrupt(digitalPinToInterrupt(D4), interruptButton, CHANGE); // Button
  attachInterrupt(digitalPinToInterrupt(D2), interruptWheel, FALLING); // Wheel
  lastInput = millis();

  gfx.initR(INITR_BLACKTAB);

  gfx.fillScreen(ST7735_WHITE);
  gfx.setCursor(5, 5);
  gfx.setTextSize(2);
  gfx.setTextColor(ST7735_RED);
  gfx.setRotation(3);
  gfx.setTextWrap(false);
  gfx.print("Hole Items...");
  currentScreen = new CurrentListScreen(&gfx);

  Serial.println("\nLooping...");
  gfx.fillScreen(ST7735_WHITE);
}

void loop()
{
  currentScreen->processCmd(cmd);
  currentScreen->processQuestion();
  cmd = 0;
  currentScreen->show();
  showRelease();
  checkDeepSleep();
  delay(100);
}

void interruptButtonBegin()
{
  if (isBounce())
  {
    return;
  }

  timePressBegin = millis();
}

void interruptButton()
{
  if (isBounce())
  {
    return;
  }

  int state = digitalRead(D4);
  if (state == LOW)
  {
    timePressBegin = millis();
    return;
  }
  else if (state == HIGH)
  {
    if ((millis() - timePressBegin) > pressTime)
    {
      cmd = LONGCLICK;
    }
    else
    {
      cmd = CLICK;
    }
  }
}

void interruptWheel()
{
  if (isBounce())
  {
    return;
  }

  if (digitalRead(D1) == LOW)
  {
    vPos++; // Could be +5 or +10
    left();
  }
  else
  {
    vPos--; // Could be -5 or -10
    right();
  }
}

bool isBounce()
{
  long currentInput = millis();
  bool isBounce = (currentInput - lastInput) < 100;
  lastInput = currentInput;
  return isBounce;
}

void left()
{
  cmd = LEFT;
}

void right()
{
  cmd = RIGHT;
}

void click()
{
  cmd = CLICK;
}

void main::setScreen(Screen *screen)
{
  currentScreen = screen;
}

void main::setPreviousScreen(Screen *screen)
{
  previousScreen = screen;
}

Screen *main::getPreviousScreen()
{
  return previousScreen;
}

void showRelease()
{
  if (digitalRead(D4) == LOW && millis() - timePressBegin > pressTime)
  {
    int xCenterPoint = 140;
    int yCenterPoint = 3;
    gfx.drawLine(xCenterPoint, yCenterPoint, 135, 8, ST7735_WHITE);
    gfx.drawLine(xCenterPoint, yCenterPoint, 145, 8, ST7735_WHITE);
    gfx.drawLine(xCenterPoint, yCenterPoint, 140, 15, ST7735_WHITE);
  }
}

void checkDeepSleep() {
  if(millis() - lastInput > 20000) {
    Serial.println("DeepSleep....zZz...");
    ESP.deepSleep(0);
  }
}