#include <Adafruit_GFX.h> // Core graphics library
#include <Adafruit_ST7735.h>
#include "Screen.h"

class YesNoScreen : public Screen
{
public:
    YesNoScreen(Adafruit_ST7735 *gfxIn);
    YesNoScreen(char *headlineIn, Adafruit_ST7735 *gfxIn, Screen *previousScreenIn);
    virtual void show();
    virtual void left();
    virtual void right();
    virtual void click();
    virtual void processCmd(int cmd);
    Screen *previousScreen;
};