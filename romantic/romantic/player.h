#pragma once
#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;

void CursorView();

void gotoxy(int, int);

enum Direction { NONE, UP, DOWN, LEFT, RIGHT };     // 플레이어 방향키 입력

class Monster;

class Player {
protected:
    int x, y;
    int moveCount;
    Direction lastDir;

public:
    int score;
    string name;

    Player();
    virtual ~Player() {}

    virtual void draw();                // 플레이어 출력
    virtual void remove();              // 플레이어 제거
    virtual void move(Monster* m);      // 플레이어 움직임    // 공격 출력될때 방향키를 입력받으면 입력받은 만큼 공격이 끝난 후 플레이어 순간이동. 수정필요.
    virtual void attack(Monster* m);    // 플레이어 공격

    void setName(const string& playerName);     // 플레이어 이름 저장
    string getName() const;                     // 플레이어 이름 입력 받음
    int getScore() const;                       // 플레이어 점수

};
