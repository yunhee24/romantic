#pragma once
#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;

class Monster {
public:
    int x, y;
    bool alive;

    Monster(int startX, int startY);
    void draw();
    void remove();
};