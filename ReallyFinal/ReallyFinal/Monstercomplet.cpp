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
        cout << "남은 시간: " << remain << "초   ";
        Sleep(400);
        if (remain <= 0) {
            gameRunning = false;
            break;
        }

    }
}

// 공격 객체 정보 구조체
struct AttackObj {
    int x, y;
    int dx, dy;  // 방향
    int type;    // 0: 세로, 1: 가로
    bool active;
};

class Monster {
    int HP;
    string ch[4] = { "▲", "△", "■", "◆" };        //별 출력할 때 타이머 출력됨 ????
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
    int x = 30, y = 6;  // 플레이어 좌표
    string pl[1] = { "●" };
public:
    void PrintPlayer(int x, int y) {
        gotoxy(x, y);
        cout << pl[0];
        this->x = x;   // 위치 저장
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
    vector<AttackObj> attacks;  // 공격 리스트, 매 틱마다 공격들이 저장 -> 공격이 너무 많아서 버그가 발생하나 
    MonsterPos mons[3];
    Player p;

    const int gameDuration = 30000;
    ULONGLONG start_time = GetTickCount64();

    drawMap(20, 3, 28, 14);
    p.PrintPlayer(30, 6);

    thread timer(TimerThread, start_time, 30);

    while (gameRunning) {
        // 체력 출력
        gotoxy(0, 1);
        cout << "남은 체력: " << p.getHP() << "   ";

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

            // 공격 생성
            AttackObj atk;
            atk.active = true;
            atk.type = (side < 2) ? 0 : 1;  //0,1이면 세로 2,3이면 가로 공격

            switch (side) {
            case 0: atk.x = x; atk.y = y + 2; atk.dx = 0; atk.dy = 1; break;  //위
            case 1: atk.x = x; atk.y = y - 2; atk.dx = 0; atk.dy = -1; break;  //아래
            case 2: atk.x = x + 3; atk.y = y; atk.dx = 1; atk.dy = 0; break;  //왼쪽
            case 3: atk.x = x - 2; atk.y = y; atk.dx = -1; atk.dy = 0; break; //오른쪽
            }

            attacks.push_back(atk);
        }

        // 공격 이동
        for (auto& atk : attacks) {
            if (!atk.active) continue;

            // 공격 이동 전 클리어             <<<<<<<이 부분을 수정해야할 듯 플레이어랑 부딪히면 플레이어도 사라짐
            gotoxy(atk.x, atk.y); 
            cout << " ";                 //cout가 아니라 객체로 만들어서 소멸시켜야 할 거 같은데   
            //ㄴ 아래쪽에서 올라오는 공격으로는 안사라짐, 왼->오 공격도 안사라지고 체력만 깎임  | 
            //위에서 아래 공격사라짐, 오>왼 공격 다른 거랑 겹치면 크기가 커짐
            atk.x += atk.dx*2;
            atk.y += atk.dy*2;

            

            if (isOutOfMap(atk.x, atk.y)) {
                atk.active = false;
                continue;
            }
            // 충돌 검사
            if (atk.x == p.getX() && atk.y == p.getY()) { //atk공격객체의 위치, 플레이어 위치
                p.Damage(1);
                atk.active = false;
                continue;
            }
            // 공격 출력
            gotoxy(atk.x, atk.y);
            if (atk.type == 0) cout << "|";
            else cout << "─";
            
        }
        // 몬스터 제거
        Sleep(500);
        for (int i = 0; i < monsterCount; i++) {
            mons[i].m->Clear(mons[i].x, mons[i].y);
            delete mons[i].m;
        }
        // 체력 검사
        if (p.getHP() <= 0) {
            gameRunning = false;
        }

        Sleep(500);
    }

    if (timer.joinable()) timer.join();

    gotoxy(0, 20);
    cout << "게임 종료! 아무 키나 누르세요...";
    _getch();
    return 0;
}
