#pragma once

#include "defines.hpp"
#include "data.hpp"

namespace Terminal{
    void die(const char* s);
    void disableRawMode();
    void enableRawMode();
    int editorReadKey();
    int getCursorPosition(int *rows, int *cols);
    int getWindowSize(int *rows, int *cols);
}