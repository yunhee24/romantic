#include "monster.h"
#include <iostream>
#include <windows.h>
using namespace std;

extern void gotoxy(int x, int y);

Monster::Monster(int startX, int startY) {
    x = startX;
    y = startY;
    alive = true;
}

void Monster::draw() {
    if (alive) {
        gotoxy(x * 2, y);
        cout << "бс";
    }
}

void Monster::remove() {
    if (alive) {
        gotoxy(x * 2, y);
        cout << " ";
        alive = false;
    }
}