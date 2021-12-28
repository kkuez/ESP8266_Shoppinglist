#include "Arduino.h"
#include "Screen.h"
#include "ListScreen.h"

class AddListScreen : public ListScreen
{

public:
    AddListScreen(Adafruit_ST7735 *gfxIn);
    virtual void click();
    virtual void longClick();
    virtual void processQuestion();
    virtual std::vector<String> *getCurrentList();
};