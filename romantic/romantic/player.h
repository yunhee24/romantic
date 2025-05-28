#pragma once
#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;

void CursorView();

void gotoxy(int, int);

enum Direction { NONE, UP, DOWN, LEFT, RIGHT };     // �÷��̾� ����Ű �Է�

class Monster;

class Player {
protected:
    int x, y;
    int moveCount;
    Direction lastDir;

public:
    int score;
    int randNumber;
    string name;

    Player();
    virtual ~Player() {}

    virtual void draw();                // �÷��̾� ���
    virtual void remove();              // �÷��̾� ����
    virtual void move(Monster* m);      // �÷��̾� ������    / ���� ��µɶ� ����Ű�� �Է¹����� �Է¹��� ��ŭ ������ ���� �� �÷��̾� �����̵�. �����ʿ�.
    virtual void attack(Monster* m);    // �÷��̾� ����

    int getScore() const;                       // �÷��̾� ����

};
