#pragma once
#include <iostream>
#include <windows.h>
#include <mutex>
#include <vector>
using namespace std;

class Monster {
public:
    int x, y;           
    bool alive;         
    int HP;
    string ch[4] = { "▲", "△", "■", "◆" };

    Monster(int startX = 12, int startY = 5, int hp = 3);  // 생성자

    void MonsterCreate(vector<Monster>& monsters);

    void PrintMonster(int x, int y, int select);
    void MonsterClear(int x[], int y[], int count, vector<Monster>& monsters);
};
