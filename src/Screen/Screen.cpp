#include "Arduino.h"
#include "Screen.h"
#include <functional>

static IOService *ioService;

Screen::Screen()
{
    if (ioService == NULL)
    {
        ioService = new IOService();
    }
}

Screen::~Screen() {}

void Screen::processCmd(int cmd) {}

void Screen::show()
{
    Serial.println("Not overriden");
}

void Screen::left() {}
void Screen::right() {}
void Screen::click() {}
void Screen::longClick() {}
void Screen::processQuestion() {}

std::vector<String> *Screen::getCurrentList()
{
    //richtig
    return NULL;
}

IOService *Screen::getIOService()
{
    return ioService;
}

void Screen::setAnswer(int answerIn) {
    answer = answerIn;     
}


void Screen::setRenderAllNew(bool val) {
    renderAllNew = val;  
}


bool Screen::getRenderAllNew() {
    return renderAllNew;  
}

