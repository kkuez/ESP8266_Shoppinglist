#include "Screen.h"

#pragma once

class ListScreen : public Screen
{
public:
    ListScreen();
    char *headline;
    int shownItems;
    int currentSelectedItem = 0;
    int lastSelectedItem = 1;
    const int textSize = 2;
    int itemShownOnTop = 0;
    virtual void processCmd(int cmd);
    virtual void selectItemPrevious();
    virtual void selectItemNext();
    virtual void selectItem(int i);
    virtual void show();
    virtual void left();
    virtual void right();
    virtual void click();
};