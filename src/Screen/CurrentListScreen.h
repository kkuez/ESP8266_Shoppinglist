#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> 
#include "Screen.h"
#include <ArduinoJson.h>
#include "ListScreen.h"

class CurrentListScreen : public ListScreen
{
public:
    CurrentListScreen(Adafruit_ST7735 *gfxIn);
    virtual void click();
    virtual void longClick();
    virtual void processQuestion();
    virtual std::vector<String> *getCurrentList();
};