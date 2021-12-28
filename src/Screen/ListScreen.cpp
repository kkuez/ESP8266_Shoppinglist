#include "ListScreen.h"

ListScreen::ListScreen()
{
}

void ListScreen::processCmd(int cmd)
{
    if (cmd == 0)
    {
        return;
    }
    else if (cmd == Screen::LEFT)
    {
        selectItemPrevious();
    }
    else if (cmd == Screen::RIGHT)
    {
        selectItemNext();
    }
    else if (cmd == Screen::CLICK)
    {
        click();
    }
    else if (cmd == Screen::LONGCLICK)
    {
        longClick();
    }
}

void ListScreen::selectItemPrevious()
{
    if (currentSelectedItem != 0)
    {
        lastSelectedItem = currentSelectedItem;
        currentSelectedItem--;
    }
}

void ListScreen::selectItemNext()
{
    if ((currentSelectedItem + 1) < getCurrentList()->size())
    {
        lastSelectedItem = currentSelectedItem;
        currentSelectedItem++;
    }
}

void ListScreen::selectItem(int i)
{
    int height = gfx->height();
    int width = gfx->width();
    int newYCurrentItem = (currentSelectedItem - itemShownOnTop) * yBoxHeight + yBoxHeight + 3;
    int newYLastItem = (lastSelectedItem - itemShownOnTop) * yBoxHeight + yBoxHeight + 3;

    if (newYCurrentItem > height)
    {
        setRenderAllNew(true);
        itemShownOnTop++;
    }
    else if (newYCurrentItem < yBoxHeight)
    {
        setRenderAllNew(true);
        itemShownOnTop--;
    }
    else
    {
        gfx->drawRoundRect(1, newYLastItem, width - 1, yBoxHeight, 3, ST7735_WHITE);
        gfx->drawRoundRect(1, newYCurrentItem, width - 1, yBoxHeight, 3, ST7735_GREEN);
    }

    lastSelectedItem = currentSelectedItem;
}

void ListScreen::show()
{
    if (!getRenderAllNew())
    {
        selectItem(currentSelectedItem);
        return;
    }

    int x = 5;
    gfx->fillScreen(ST7735_WHITE);
    gfx->setRotation(3);
    gfx->setTextSize(textSize);

    // header
    gfx->fillRect(0, 0, gfx->width(), yBoxHeight, ST7735_BLUE);
    u8g2_for_adafruit_gfx.setFontMode(1);
    u8g2_for_adafruit_gfx.setFont(u8g2_font_helvR14_tf);
    u8g2_for_adafruit_gfx.setBackgroundColor(ST7735_BLUE);
    u8g2_for_adafruit_gfx.setForegroundColor(ST7735_WHITE);
    u8g2_for_adafruit_gfx.setCursor(x, yBoxHeight - 2);
    u8g2_for_adafruit_gfx.print(headline);

    // items
    int y = yBoxHeight * 2;
    gfx->setTextWrap(false);
    u8g2_for_adafruit_gfx.setForegroundColor(ST7735_BLUE);
    u8g2_for_adafruit_gfx.setBackgroundColor(ST7735_WHITE);

    std::vector<String> *itemsVector = getCurrentList();
    for (unsigned int i = itemShownOnTop; i < itemShownOnTop + shownItems; i++)
    {
        if (i >= itemsVector->size())
        {
            setRenderAllNew(false);
            break;
        }

        u8g2_for_adafruit_gfx.setCursor(x, y);
        u8g2_for_adafruit_gfx.print(i >= itemsVector->size() ? "" : itemsVector->at(i));
        y += yBoxHeight;

        if (i == currentSelectedItem)
        {
            selectItem(i);
        }
    }

    setRenderAllNew(false);
}

void ListScreen::left()
{
    selectItemPrevious();
}

void ListScreen::right()
{
    selectItemNext();
}

void ListScreen::click() {}