#include "monster.h"
#include "gotoxy.h"
#include "map.h"
#include "attack.h" 
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

extern bool gamerun;
extern std::mutex output_mutex;

// 생성자: 위치, 체력 초기화 + alive 설정
Monster::Monster(int startX, int startY, int hp) {
    x = startX;
    y = startY;
    HP = hp;
    alive = true;
}

// 랜덤 다수 몬스터 출력 (공격용은 아님)
void Monster::PrintMonster(int x, int y, int select) {
    std::lock_guard<std::mutex> lock(output_mutex);
    gotoxy(x, y);
    cout << ch[select];
}

// 랜덤 다수 몬스터 제거
void Monster::MonsterClear(int x[], int y[], int count, vector<Monster>& monsters) {
    std::lock_guard<std::mutex> lock(output_mutex);
    for (int i = 0; i < count; ++i) {
        // 화면 지우기
        gotoxy(x[i], y[i]);
        cout << "  ";

        // alive = false 처리
        for (auto& m : monsters) {
            if (m.x == x[i] && m.y == y[i] && m.alive) {
                m.alive = false;
                break;
            }
        }
    }
}

// 몬스터 생성 루프
void Monster::MonsterCreate(std::vector<Monster>& monsters) {
    srand((unsigned int)time(NULL));
    const int MAX = 5;
    int xs[MAX], ys[MAX], dirs[MAX];

    while (gamerun) {
        int mcnt = rand() % 3 + 3;     //몬스터 수

        for (int i = 0; i < mcnt; i++) {
            int dir = rand() % 4;
            int x, y;

            switch (dir) {
            // 위쪽
            case 0: y = MAP_TOP - 1; x = rand() % 20 + 25; break;
            // 아래쪽 
            case 1: y = MAP_BOTTOM + 1; x = rand() % 20 + 25; break;
            // 왼쪽
            case 2: x = MAP_LEFT - 2; y = rand() % 10 + 5; break;
            // 오른쪽
            case 3: x = MAP_RIGHT + 1; y = rand() % 10 + 5; break;
            }
            xs[i] = x;
            ys[i] = y;
            dirs[i] = dir;

            bool positionTaken = false;
            for (auto& m : monsters) {
                if (m.alive && m.x == x && m.y == y) {
                    positionTaken = true;
                    break;
                }
            }

            monsters.emplace_back(x, y, 3); // 벡터에 추가, 마지막은 체력
            CreateAttack(dir, x, y);  // 몬스터 공격 생성
        }
        // 몬스터 출력
        for (int i = 0; i < mcnt; ++i) {
            PrintMonster(xs[i], ys[i], dirs[i]);
        }

        // 일정 시간 유지 후 제거
        int duration = (rand() % 4 + 2) * 1000;
        Sleep(duration);

        this->MonsterClear(xs, ys, mcnt, monsters);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}