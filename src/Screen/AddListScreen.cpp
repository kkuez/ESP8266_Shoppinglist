#include "AddListScreen.h"
#include "main.h"
#include "YesNoScreen.h"

AddListScreen::AddListScreen(Adafruit_ST7735 *gfxIn)
{
    gfx = gfxIn;
    shownItems = round((gfx->height() - yBoxHeight) / yBoxHeight);
    Serial.printf("Items to be shown on Screen: %d\n", shownItems);
    u8g2_for_adafruit_gfx.begin(*gfxIn);
    headline = "Hinzufügen:";
}

void AddListScreen::click() {
    main *m = new main();
    Screen *nextScreen = m->getPreviousScreen();
    nextScreen->setRenderAllNew(true);
    m->setScreen(nextScreen);
    m->setPreviousScreen(this);
}

void AddListScreen::longClick() {
    String item = getCurrentList()->at(currentSelectedItem);
    char itemCharArray[item.length() + 1];
    item.toCharArray(itemCharArray, item.length() + 1);

    char message[100];
    sprintf(message, "%s\nhinzufügen?", itemCharArray);
    Screen *yesOrNoScreen = new YesNoScreen(message, gfx, this);
}

void AddListScreen::processQuestion() {
    if (answer != Screen::NOTYESORNO && answer == Screen::YES)
    {
        //addItem
        getIOService()->addItem(currentSelectedItem);
        getIOService()->fetchItems();
        answer = Screen::NOTYESORNO;
        setRenderAllNew(true);
    }
}

std::vector<String> *AddListScreen::getCurrentList() {
    return getIOService()->getAddList();
}