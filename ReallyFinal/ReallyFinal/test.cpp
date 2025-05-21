#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <mutex>
#include "Map.h"
#include "gotoxy.h"
using namespace std;

bool gamerun = true;
std::mutex output_mutex;  //��� ��ȣ�� mutex

void TimerThread(ULONGLONG start_time, int total) {
    {
        std::lock_guard<std::mutex> lock(output_mutex);
        gotoxy(0, 0);
        cout << "���� �ð�: ";
    }

    while (gamerun) {
        int timecount = (GetTickCount64() - start_time) / 1000;
        int time_num = total - timecount;

        if (time_num <= 0) {
            gamerun = false;
            break;
        }

        {
            std::lock_guard<std::mutex> lock(output_mutex);
            gotoxy(11, 0);
            cout << "      ";
            gotoxy(11, 0);
            cout << time_num << "�� ";
        }

        Sleep(200);  // Ÿ�̸� ����
    }
}

class Monster {
    int HP;
    string ch[4] = { "��", "��", "��", "��" };
public:
    Monster(int hp = 3) : HP(hp) {}

    void PrintMonster(int x, int y, int select) {
        std::lock_guard<std::mutex> lock(output_mutex);  // ��� ��ȣ
        gotoxy(x, y);
        cout << ch[select];
    }

    void MonsterClear(int x[], int y[], int mfor) {
        std::lock_guard<std::mutex> lock(output_mutex);  // ��� ��ȣ
        for (int i = 0; i < mfor; i++) {
            gotoxy(x[i], y[i]);
            cout << "  ";
        }
    }

    void MonsterCreate() {
        while (gamerun) {
            const int MAX = 5;
            int xs[MAX], ys[MAX], dirs[MAX];

            int mcnt = rand() % 3 + 1;
            for (int i = 0; i < mcnt; i++) {
                int dir = rand() % 4;
                int x, y;

                switch (dir) {
                case 0: y = 3; x = rand() % 20 + 25; break;
                case 1: y = 18; x = rand() % 20 + 25; break;
                case 2: x = 18; y = rand() % 10 + 5; break;
                case 3: x = 49; y = rand() % 10 + 5; break;
                }

                xs[i] = x;
                ys[i] = y;
                dirs[i] = dir;
            }

            for (int i = 0; i < mcnt; i++) {
                PrintMonster(xs[i], ys[i], dirs[i]);
            }

            int duration = (rand() % 5 + 1) * 1000;
            Sleep(duration);  // ���� ���� �ð�

            MonsterClear(xs, ys, mcnt);
            Sleep(50);  // ���� �������� ���
        }
    }
};

int main() {
    srand((unsigned int)time(NULL));
    Monster m;

    drawMap(28, 14);  // ���� �� ���
    ULONGLONG start_time = GetTickCount64();

    thread timerth(TimerThread, start_time, 30);
    Sleep(50);  // ��� ��� �� ���� ����

    m.MonsterCreate();

    if (timerth.joinable()) {
        timerth.join();
    }

    gotoxy(0, 22);
    cout << "���� ����" << endl;
    return 0;
}