#pragma once
#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;

void gotoxy(int, int);

enum Direction { NONE, UP, DOWN, LEFT, RIGHT };     // 키보드 방향 입력

class Player {
protected:
    int x, y;           // 플레이어 위치
    int moveCount;      // 플레이어 이동 횟수
    Direction lastDir;  // 마지막 방향

public:
    Player();
    virtual ~Player() {}

    virtual void draw();
    virtual void remove();
    virtual void move();
    virtual void attack();
};