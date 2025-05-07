#pragma once
#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;

void gotoxy(int, int);

enum Direction { NONE, UP, DOWN, LEFT, RIGHT };     // Ű���� ���� �Է�

class Player {
protected:
    int x, y;           // �÷��̾� ��ġ
    int moveCount;      // �÷��̾� �̵� Ƚ��
    Direction lastDir;  // ������ ����

public:
    Player();
    virtual ~Player() {}

    virtual void draw();
    virtual void remove();
    virtual void move();
    virtual void attack();
};