#pragma once
#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;

void gotoxy(int, int);

enum Direction { NONE, UP, DOWN, LEFT, RIGHT };     // �÷��̾� ����Ű �Է�

class Monster;

class Player {
protected:
    int x, y;
    int moveCount;
    Direction lastDir;
    int score;

public:
    Player();
    virtual ~Player() {}

    virtual void draw();
    virtual void remove();
    virtual void move(Monster* m);
    virtual void attack(Monster* m);
};