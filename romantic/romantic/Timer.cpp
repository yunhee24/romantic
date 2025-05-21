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
    ::cout << "���� �ð�:    ��  ";

    while (gameRunning) {                              //�ݺ������� �ٲ㺸��
        int timecount = (GetTickCount64() - start_time) / 1000;
        int remain = total - timecount;
        gotoxy(12, 0);
        for (int i = 0; i < x; i++)
        {
            printf("\t");
        }
        ::cout << remain;        //����� ������ 
        if (remain <= 0) {
            gameRunning = false;
            break;
        }
        Sleep(200);
    }
}