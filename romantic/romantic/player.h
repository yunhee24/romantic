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
    int x, y;           // 플레이어 위치
    int hp;             // 체력
    int moveCount;      // 움직임 횟수
    Direction lastDir;  // 마지막 이동 방향

public:
    int score;          // 점수
    int randNumber;     // 이동 횟수 랜덤 할당
    string name;        // 이름 입력

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
