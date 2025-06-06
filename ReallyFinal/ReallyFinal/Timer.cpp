#include "gotoxy.h"
#include "Timer.h"
#include <iostream>
#include <mutex>
using namespace std;

bool gamerun = true;
std::mutex output_mutex;

void TimerThread(ULONGLONG start_time, int total) {
    {
        lock_guard<mutex> lock(output_mutex);
        gotoxy(0, 0);
        cout << "���� �ð�: ";
    }

    while (gamerun) {
        int elapsed = (GetTickCount64() - start_time) / 1000;
        int time_left = total - elapsed;
        if (time_left <= 0) {
            gamerun = false;
            break;
        }

        {
            lock_guard<mutex> lock(output_mutex);
            gotoxy(11, 0);
            cout << time_left << "�� ";
        }

        Sleep(200);
    }
}