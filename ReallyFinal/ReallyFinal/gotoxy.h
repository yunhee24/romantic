#pragma once
#include <windows.h>

inline void gotoxy(SHORT x, SHORT y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}