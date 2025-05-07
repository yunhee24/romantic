#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <atomic>
#include "DrawMap.h"
using namespace std;

atomic<bool> gameRunning(true);


void TimerThread(ULONGLONG start_time, int total) {
    while (gameRunning) {
        int timecount = (GetTickCount64() - start_time) / 1000;
        int remain = total - timecount;
        gotoxy(0, 0);
        cout << "���� �ð�: " << remain << "��   ";
        Sleep(400);
        if (remain <= 0) {
            gameRunning = false;
            break;
        }

    }
}

// ���� ��ü ���� ����ü
struct AttackObj {
    int x, y;
    int dx, dy;  // ����
    int type;    // 0: ����, 1: ����
    bool active;
};

class Monster {
    int HP;
    string ch[4] = { "��", "��", "��", "��" };        //�� ����� �� Ÿ�̸� ��µ� ????
public:
    Monster(int hp = 3) : HP(hp) {}
    void PrintMonster(int x, int y, int select) {
        gotoxy(x, y);
        cout << ch[select % 4];
    }
    void Clear(int x, int y) {
        gotoxy(x, y);
        cout << " ";
    }
    void Damage(int dmg) {
        HP -= dmg;
    }
    bool IsDead() {
        return HP <= 0;
    }
};

class Player {
    int HP = 5;
    int x = 30, y = 6;  // �÷��̾� ��ǥ
    string pl[1] = { "��" };
public:
    void PrintPlayer(int x, int y) {
        gotoxy(x, y);
        cout << pl[0];
        this->x = x;   // ��ġ ����
        this->y = y;
    }
    void Damage(int dmg) {
        HP -= dmg;
    }
    int getHP() const { return HP; }
    int getX() const { return x; }
    int getY() const { return y; }
};

struct MonsterPos {
    int x, y, type;
    Monster* m;
};

bool isOutOfMap(int x, int y) {
    return (x < 21 || x > 47 || y < 4 || y > 16);
}

int main() {
    srand((unsigned int)time(NULL));
    vector<AttackObj> attacks;  // ���� ����Ʈ, �� ƽ���� ���ݵ��� ���� -> ������ �ʹ� ���Ƽ� ���װ� �߻��ϳ� 
    MonsterPos mons[3];
    Player p;

    const int gameDuration = 30000;
    ULONGLONG start_time = GetTickCount64();

    drawMap(20, 3, 28, 14);
    p.PrintPlayer(30, 6);

    thread timer(TimerThread, start_time, 30);

    while (gameRunning) {
        // ü�� ���
        gotoxy(0, 1);
        cout << "���� ü��: " << p.getHP() << "   ";

        int monsterCount = rand() % 3 + 1;

        for (int i = 0; i < monsterCount; i++) {
            int side = rand() % 4;
            int x, y;

            switch (side) {
            case 0: y = 2; x = rand() % 20 + 25; break;
            case 1: y = 17; x = rand() % 20 + 25; break;
            case 2: x = 18; y = rand() % 10 + 5; break;
            case 3: x = 48; y = rand() % 10 + 5; break;
            }

            mons[i].m = new Monster();
            mons[i].x = x;
            mons[i].y = y;
            mons[i].type = side;

            mons[i].m->PrintMonster(x, y, side);

            // ���� ����
            AttackObj atk;
            atk.active = true;
            atk.type = (side < 2) ? 0 : 1;  //0,1�̸� ���� 2,3�̸� ���� ����

            switch (side) {
            case 0: atk.x = x; atk.y = y + 2; atk.dx = 0; atk.dy = 1; break;  //��
            case 1: atk.x = x; atk.y = y - 2; atk.dx = 0; atk.dy = -1; break;  //�Ʒ�
            case 2: atk.x = x + 3; atk.y = y; atk.dx = 1; atk.dy = 0; break;  //����
            case 3: atk.x = x - 2; atk.y = y; atk.dx = -1; atk.dy = 0; break; //������
            }

            attacks.push_back(atk);
        }

        // ���� �̵�
        for (auto& atk : attacks) {
            if (!atk.active) continue;

            // ���� �̵� �� Ŭ����             <<<<<<<�� �κ��� �����ؾ��� �� �÷��̾�� �ε����� �÷��̾ �����
            gotoxy(atk.x, atk.y); 
            cout << " ";                 //cout�� �ƴ϶� ��ü�� ���� �Ҹ���Ѿ� �� �� ������   
            //�� �Ʒ��ʿ��� �ö���� �������δ� �Ȼ����, ��->�� ���ݵ� �Ȼ������ ü�¸� ����  | 
            //������ �Ʒ� ���ݻ����, ��>�� ���� �ٸ� �Ŷ� ��ġ�� ũ�Ⱑ Ŀ��
            atk.x += atk.dx*2;
            atk.y += atk.dy*2;

            

            if (isOutOfMap(atk.x, atk.y)) {
                atk.active = false;
                continue;
            }
            // �浹 �˻�
            if (atk.x == p.getX() && atk.y == p.getY()) { //atk���ݰ�ü�� ��ġ, �÷��̾� ��ġ
                p.Damage(1);
                atk.active = false;
                continue;
            }
            // ���� ���
            gotoxy(atk.x, atk.y);
            if (atk.type == 0) cout << "|";
            else cout << "��";
            
        }
        // ���� ����
        Sleep(500);
        for (int i = 0; i < monsterCount; i++) {
            mons[i].m->Clear(mons[i].x, mons[i].y);
            delete mons[i].m;
        }
        // ü�� �˻�
        if (p.getHP() <= 0) {
            gameRunning = false;
        }

        Sleep(500);
    }

    if (timer.joinable()) timer.join();

    gotoxy(0, 20);
    cout << "���� ����! �ƹ� Ű�� ��������...";
    _getch();
    return 0;
}
