#include <Arduino.h>
#include <Adafruit_GFX.h> // Core graphics library
#include <Adafruit_ST7735.h>
#include "Screen.h"
#include "ListScreen.h"
#include "CurrentListScreen.h"
#include "YesNoScreen.h"
#include "AddListScreen.h"
#include <ArduinoJson.h>
#include <math.h>
#include "main.h"


CurrentListScreen::CurrentListScreen(Adafruit_ST7735 *gfxIn)
{
    gfx = gfxIn;
    shownItems = round((gfx->height() - yBoxHeight) / yBoxHeight);
    Serial.printf("Items to be shown on Screen: %d\n", shownItems);

    u8g2_for_adafruit_gfx.begin(*gfxIn);
    headline = "Aktuelle Liste:";
}

void CurrentListScreen::click() {
    Serial.println("click current");
    main *m = new main();
    Serial.println("click current1");
    Screen *nextScreen = m->getPreviousScreen();
    Serial.println("click current2");

    if(nextScreen == NULL) {
        m->setPreviousScreen(new AddListScreen(gfx));
        nextScreen = m->getPreviousScreen();
    }

    nextScreen->setRenderAllNew(true);
    Serial.println("click current3");
    m->setScreen(nextScreen);
    Serial.println("click current4");
    m->setPreviousScreen(this);
    Serial.println("click current5");
}

void CurrentListScreen::longClick()
{
    String item = getCurrentList()->at(currentSelectedItem);
    char itemCharArray[item.length() + 1];
    item.toCharArray(itemCharArray, item.length() + 1);

    char message[100];
    sprintf(message, "%s\nlöschen?", itemCharArray);
    Screen *yesOrNoScreen = new YesNoScreen(message, gfx, this);
}

void CurrentListScreen::processQuestion()
{
    if (answer != Screen::NOTYESORNO && answer == Screen::YES)
    {
        //addItem
        getIOService()->deleteItem(currentSelectedItem);
        answer = Screen::NOTYESORNO;
        getIOService()->fetchItems();
        setRenderAllNew(true);
    }
}

std::vector<String> *CurrentListScreen::getCurrentList()
{
    //richtig
    return getIOService()->getCurrentList();
}