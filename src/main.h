#include "Screen/Screen.h"

class main {
    public:
    static void setScreen(Screen *screen);
    static void setPreviousScreen(Screen *screen);
    static Screen *getPreviousScreen();
};