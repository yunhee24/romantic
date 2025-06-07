#pragma once
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include "gotoxy.h"
using namespace std;

void CursorView();

enum Direction { NONE, UP, DOWN, LEFT, RIGHT };
class Monster;

class Player {
protected:
    int x, y;
    int hp;
    int moveCount;
    Direction lastDir;

public:
    int score;
    int randNumber;
    string name;

    Player();
    virtual ~Player() {}

    virtual void draw();
    virtual void remove();
    virtual void move(std::vector<Monster>& monsters);
    virtual void attack(std::vector<Monster>& monsters);

    virtual void decreaseHP();
    virtual int getHP();

    int getX() const;
    int getY() const;

    int getScore() const;
};