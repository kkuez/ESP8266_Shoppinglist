#include <Adafruit_GFX.h> // Core graphics library
#include <Adafruit_ST7735.h>
#include <U8g2_for_Adafruit_GFX.h>
#include <io/IOService.h>
#include <functional>

#pragma once

class Screen
{
public:
    Adafruit_ST7735 *gfx;
    U8G2_FOR_ADAFRUIT_GFX u8g2_for_adafruit_gfx;
    const int yBoxHeight = 18;
    static const int RIGHT = -1;
    static const int LEFT = 1;
    static const int CLICK = 2;
    static const int LONGCLICK = 3;
    static const int NOTYESORNO = 0;
    static const int YES = 1;
    static const int NO = 2;
    int answer = Screen::NOTYESORNO;
    Screen();
    ~Screen();
    virtual void show();
    virtual void left();
    virtual void right();
    virtual void click();
    virtual void longClick();
    virtual void processCmd(int cmd);
    virtual void processQuestion();
    IOService *getIOService();
    virtual std::vector<String> *getCurrentList();
    bool questionActive;
    void setAnswer(int answerIn);
    void setRenderAllNew(bool val);
    bool getRenderAllNew();
private:
    bool renderAllNew = true;
};