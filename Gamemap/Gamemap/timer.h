// timer.h
#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <windows.h>  // Sleep, system
using namespace std;

// 남은 시간을 화면에 출력하고 1초마다 감소시킴
void countdownTimer(int seconds) {
    for (int i = seconds; i >= 0; --i) {
        COORD pos = { 0, 0 };  // 좌상단 위치
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
        cout << "남은 시간: " << i << "초   " << endl;  // 공백 추가로 덮어쓰기 방지
        Sleep(1000);
    }

    system("cls");
    cout << "시간 초과! 게임 종료\n";
    exit(0);  // 프로그램 종료
}

#endif
