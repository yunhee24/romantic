#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <mutex>
#include <vector>
#include "Map.h"
#include "gotoxy.h"

using namespace std;

bool gamerun = true;
mutex output_mutex;

enum map_pos {   //�� ��ǥ
    MAP_LEFT = 20,
    MAP_RIGHT = 47,
    MAP_TOP = 3,
    MAP_BOTTOM = 18
};

// ���� ��ü ����ü
struct AttackObj {
    int x, y;        // ��ġ
    int dx, dy;      // �̵� ����
    string symbol;     // ��� ��ȣ (' | ' �Ǵ� '��')
    bool active;     // ������ ����ִ��� ����
};

vector<AttackObj> attacks;  // ���� ��ü ���� ����


void TimerThread(ULONGLONG start_time, int total) {
    {
        lock_guard<mutex> lock(output_mutex);
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
            lock_guard<mutex> lock(output_mutex);
            gotoxy(11, 0);
            cout << time_num << "�� ";
        }

        Sleep(200);
    }
}

// ���� �̵� �Լ�
void AttackThread() { 
    while (gamerun) { 
        for (auto& atk : attacks) {
            if (!atk.active) continue;

            {
                lock_guard<mutex> lock(output_mutex);
                gotoxy(atk.x, atk.y);
                cout << "  ";  // ���� ��ġ �����
            }

            // ��ġ �̵�
            atk.x += atk.dx;
            atk.y += atk.dy;

            // �� �ٱ��̸� ��Ȱ��ȭ
            if (atk.x < MAP_LEFT || atk.x > MAP_RIGHT-2 ||
                atk.y < MAP_TOP+2 || atk.y > MAP_BOTTOM-2) {
                atk.active = false;
                continue;
            }

            {
                lock_guard<mutex> lock(output_mutex);
                gotoxy(atk.x, atk.y);
                cout << atk.symbol;
            }
        }

        Sleep(1000);  // 1�ʸ��� �̵�
    }
}

class Player {
    int HP;
    string st = "��";
public:
    void Playerprint()  //�׳� �����̶� �ε����� ����� ����
    {
        gotoxy(30, 7);
        cout << st;
    }
    void setHP() { HP = 3; }
    int getHP() { return HP; }
    void GameOver()
    {
        if (HP < 0) {
            gamerun = false;
        }
    }
};


// ���� Ŭ����
class Monster {
    int HP;
    string ch[4] = { "��", "��", "��", "��" };
public:
    Monster(int hp = 3) : HP(hp) {}

    void PrintMonster(int x, int y, int select) {
        lock_guard<mutex> lock(output_mutex);
        gotoxy(x, y);
        cout << ch[select];
    }

    void MonsterClear(int x[], int y[], int mfor) {
        lock_guard<mutex> lock(output_mutex);
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

                AttackObj atk;
                atk.active = true;

                switch (dir) {
                case 0: // ��
                    y = MAP_TOP;
                    x = rand() % 20 + 25;
                    atk.x = x;
                    atk.y = y + 2;
                    atk.dx = 0;
                    atk.dy = 1;
                    atk.symbol = "|";
                    break;

                case 1: // �Ʒ�
                    y = MAP_BOTTOM;
                    x = rand() % 20 + 25;
                    atk.x = x;
                    atk.y = y ;
                    atk.dx = 0;
                    atk.dy = -2;
                    atk.symbol = "|";
                    break;

                case 2: // ����
                    x = MAP_LEFT-2;
                    y = rand() % 10 + 5;
                    atk.x = x + 3;
                    atk.y = y;
                    atk.dx = 1;
                    atk.dy = 0;
                    atk.symbol = "��";
                    break;

                case 3: // ������
                    x = MAP_RIGHT+2;
                    y = rand() % 10 + 5;
                    atk.x = x-4;
                    atk.y = y;
                    atk.dx = -2;
                    atk.dy = 0;
                    atk.symbol = "��";
                    break;
                }

                xs[i] = x;
                ys[i] = y;
                dirs[i] = dir;

                attacks.push_back(atk);
            }

            // ���� ���
            for (int i = 0; i < mcnt; i++) {
                PrintMonster(xs[i], ys[i], dirs[i]);
            }
            int duration = (rand() % 5 + 1) * 1000;
            Sleep(duration);

            // ���� ����
            MonsterClear(xs, ys, mcnt);
            Sleep(50);
        }
    }
};
//���� �Լ�
int main() {
    srand((unsigned int)time(NULL));
    Player p;
    Monster m;

    drawMap(28, 14);  // �� ���
    p.Playerprint();
    ULONGLONG start_time = GetTickCount64();

    thread timerth(TimerThread, start_time, 30);
    Sleep(50);

    thread attackth(AttackThread);  // ���� ������ ����
    Sleep(50);  // Ÿ�̸� ���� �����ϵ��� ��� ���

    m.MonsterCreate();

    if (timerth.joinable()) timerth.join();
    if (attackth.joinable()) attackth.join();

    gotoxy(0, 22);
    cout << "���� ����" << endl;
    return 0;
}