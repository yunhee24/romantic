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

// ������: ��ġ, ü�� �ʱ�ȭ + alive ����
Monster::Monster(int startX, int startY, int hp) {
    x = startX;
    y = startY;
    HP = hp;
    alive = true;
}


// ���� �ټ� ���� ��� (���ݿ��� �ƴ�)
void Monster::PrintMonster(int x, int y, int select) {
    std::lock_guard<std::mutex> lock(output_mutex);
    gotoxy(x, y);
    cout << ch[select];
}

// ���� �ټ� ���� ����
void Monster::MonsterClear(int x[], int y[], int count, vector<Monster>& monsters) {
    std::lock_guard<std::mutex> lock(output_mutex);
    for (int i = 0; i < count; ++i) {
        // ȭ�� �����
        gotoxy(x[i], y[i]);
        cout << "  ";

        // alive = false ó��
        for (auto& m : monsters) {
            if (m.x == x[i] && m.y == y[i] && m.alive) {
                m.alive = false;
                break;
            }
        }
    }
}

// ���� ���� ����
void Monster::MonsterCreate(std::vector<Monster>& monsters) {
    srand((unsigned int)time(NULL));
    const int MAX = 5;
    int xs[MAX], ys[MAX], dirs[MAX];

    while (gamerun) {
        int mcnt = rand() % 3 + 3;     //���� ��

        for (int i = 0; i < mcnt; i++) {
            int dir = rand() % 4;
            int x, y;

            switch (dir) {
                // ����
            case 0: y = MAP_TOP - 1; x = rand() % 20 + 25; break;
                // �Ʒ��� 
            case 1: y = MAP_BOTTOM + 1; x = rand() % 20 + 25; break;  //MAP_BOTTOM + 1
                // ����
            case 2: x = MAP_LEFT - 2; y = rand() % 10 + 5; break;
                // ������
            case 3: x = MAP_RIGHT + 1; y = rand() % 10 + 5; break;    //MAP_RIGHT + 5
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
            monsters.emplace_back(x, y, 3); // ���Ϳ� �߰�, �������� ü��

            CreateAttack(dir, x, y);  // ���� ���� ����
        }
        // ���� ���
        for (int i = 0; i < mcnt; ++i) {
            PrintMonster(xs[i], ys[i], dirs[i]);
        }

        // ���� �ð� ���� �� ����
        int duration = (rand() % 4 + 2) * 1000;
        Sleep(duration);

        this->MonsterClear(xs, ys, mcnt, monsters);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}