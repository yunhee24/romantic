#pragma once
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "player.h"
using namespace std;

bool gameRunning = true;

//const int x, y;

void TimerThread(ULONGLONG start_time, int total) {
    const int x = 12, y = 0;
    gotoxy(0, 0);
    ::cout << "남은 시간:    초  ";

    while (gameRunning) {                              //반복문으로 바꿔보기
        int timecount = (GetTickCount64() - start_time) / 1000;
        int remain = total - timecount;
        gotoxy(12, 0);
        for (int i = 0; i < x; i++)
        {
            printf("\t");
        }
        ::cout << remain;        //출력은 밖으로 
        if (remain <= 0) {
            gameRunning = false;
            break;
        }
        Sleep(200);
    }
}