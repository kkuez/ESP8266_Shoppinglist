#include <Arduino.h>
#include <Adafruit_GFX.h> // Core graphics library
#include <Adafruit_ST7735.h>
#include "Screen.h"
#include "YesNoScreen.h"
#include "main.h"

bool isJa = true;
int questionHeight;

YesNoScreen::YesNoScreen(char *headlineIn, Adafruit_ST7735 *gfxIn, Screen *previousScreenIn)
{
    gfx = gfxIn;
    u8g2_for_adafruit_gfx.begin(*gfxIn);
    previousScreen = previousScreenIn;

    int yesNoHeight = 30;
    questionHeight = gfx->height() - 2 * yesNoHeight;

    // Questionbox
    gfx->fillScreen(ST7735_WHITE);
    gfx->fillRect(0, 0, gfx->width(), questionHeight, ST7735_BLUE);
    gfx->setTextWrap(true);
    u8g2_for_adafruit_gfx.setFontMode(1);
    u8g2_for_adafruit_gfx.setFont(u8g2_font_helvR14_tf);
    u8g2_for_adafruit_gfx.setBackgroundColor(ST7735_BLUE);
    u8g2_for_adafruit_gfx.setForegroundColor(ST7735_WHITE);
    u8g2_for_adafruit_gfx.setCursor(3, 18);
    u8g2_for_adafruit_gfx.print(headlineIn);

    // Yes/No
    u8g2_for_adafruit_gfx.setBackgroundColor(ST7735_WHITE);
    u8g2_for_adafruit_gfx.setForegroundColor(ST7735_BLUE);
    u8g2_for_adafruit_gfx.setCursor(gfx->width() / 2 - 14, questionHeight + 23);
    u8g2_for_adafruit_gfx.print("Ja");
    u8g2_for_adafruit_gfx.setCursor(gfx->width() / 2 - 22, questionHeight * 2 - 20);
    u8g2_for_adafruit_gfx.print("Nein");
    
    main *m = new main();
    m->setScreen(this);
}

void YesNoScreen::show()
{
    if(isJa) {
        gfx->drawRoundRect(1, questionHeight, gfx->width() - 1, 28, 3, ST7735_GREEN);
        gfx->drawRoundRect(1, questionHeight + 30, gfx->width() - 1, 28, 3, ST7735_WHITE);
    } else {
        gfx->drawRoundRect(1, questionHeight, gfx->width() - 1, 28, 3, ST7735_WHITE);
        gfx->drawRoundRect(1, questionHeight + 30, gfx->width() - 1, 28, 3, ST7735_GREEN);
    }
}

void YesNoScreen::left() {
    isJa = true;
}

void YesNoScreen::right() {
    isJa = false;
}

void YesNoScreen::click() {
    if(isJa) {
        previousScreen->setAnswer(Screen::YES);
    } else {
        previousScreen->setAnswer(Screen::NO);
    }

    main *m = new main();
    previousScreen->setRenderAllNew(true);
    m->setScreen(previousScreen);
}

void YesNoScreen::processCmd(int cmd) {
    if (cmd == 0)
    {
        return;
    }
    else if (cmd == Screen::LEFT)
    {
      left();
    }
    else if (cmd == Screen::RIGHT)
    {
       right();
    }
    else if (cmd == Screen::CLICK)
    {
        click();
    }

}