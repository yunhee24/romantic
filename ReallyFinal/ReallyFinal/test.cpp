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
std::mutex output_mutex;  //출력 보호용 mutex

void TimerThread(ULONGLONG start_time, int total) {
    {
        std::lock_guard<std::mutex> lock(output_mutex);
        gotoxy(0, 0);
        cout << "남은 시간: ";
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
            cout << time_num << "초 ";
        }

        Sleep(200);  // 타이머 간격
    }
}

class Monster {
    int HP;
    string ch[4] = { "▲", "△", "■", "◆" };
public:
    Monster(int hp = 3) : HP(hp) {}

    void PrintMonster(int x, int y, int select) {
        std::lock_guard<std::mutex> lock(output_mutex);  // 출력 보호
        gotoxy(x, y);
        cout << ch[select];
    }

    void MonsterClear(int x[], int y[], int mfor) {
        std::lock_guard<std::mutex> lock(output_mutex);  // 출력 보호
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
            Sleep(duration);  // 몬스터 유지 시간

            MonsterClear(xs, ys, mcnt);
            Sleep(50);  // 다음 생성까지 대기
        }
    }
};

int main() {
    srand((unsigned int)time(NULL));
    Monster m;

    drawMap(28, 14);  // 게임 맵 출력
    ULONGLONG start_time = GetTickCount64();

    thread timerth(TimerThread, start_time, 30);
    Sleep(50);  // 잠깐 대기 후 몬스터 시작

    m.MonsterCreate();

    if (timerth.joinable()) {
        timerth.join();
    }

    gotoxy(0, 22);
    cout << "게임 종료" << endl;
    return 0;
}