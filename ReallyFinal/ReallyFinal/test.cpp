#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include <thread>
#include <atomic>

using namespace std;

atomic<bool> gameRunning(true);

void gotoxy(int x, int y) {
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void TimerThread(ULONGLONG start_time, int total) {
    while (gameRunning) {                              //반복문으로 바꿔보기
        gotoxy(0, 0);  //여기 있을 때 오류 들남
        int timecount = (GetTickCount64() - start_time) / 1000;
        int remain = total - timecount;
        //gotoxy(0, 0);
        ::cout << "남은 시간: " << remain << "초   ";
        //Sleep(200);  //이거 없으면 몬스터 생성되는 곳마다 남은 시간이 출력됨
        if (remain <= 0) {
            gameRunning = false;
            break;
        }
        Sleep(200);
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
        ::cout << ch[select % 4];
    }
    void Clear(int x, int y) {
        gotoxy(x, y);
        ::cout << "  ";
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
        ::cout << pl[0];
        this->x = x;   // 위치 저장
        this->y = y;
    }
    void Damage(int dmg) {
        HP -= dmg;
    }
    int getHP() const { return HP; }
    int getX() const { return x; }
    int getY() const { return y; }

    void PrintPlayerHP()
    {
        gotoxy(0, 1);
        ::cout << "남은 체력: " << getHP() << "  ";
    }
};
/*
void PrintPlayerHP(Player& p)
{
    gotoxy(0, 1);
    cout<< "남은 체력: "<< p.getHP() << "   ";
}
*/
struct MonsterPos {
    int x, y, type;
    Monster* m;
};

bool isOutOfMap(int x, int y) {
    return (x < 21 || x > 47 || y < 4 || y > 15);
}

void drawMap(int X, int Y, int width, int height) {
    for (int y = 0; y < height; ++y) {
        gotoxy(X, Y + y);
        for (int x = 0; x < width; ++x) {
            if (y == 0 && x == 0) ::cout << "┌";
            else if (y == 0 && x == width - 1) ::cout << "┐";
            else if (y == height - 1 && x == 0) ::cout << "└";
            else if (y == height - 1 && x == width - 1) ::cout << "┘";
            else if (y == 0 || y == height - 1) ::cout << "─";
            else if (x == 0 || x == width - 1) ::cout << "│";
            else ::cout << " ";
        }
        ::cout << endl;
    }
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
        p.PrintPlayerHP();

        int monsterCount = rand() % 2 + 1 ; //몬스터 생성 수 결정

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
            mons[i].type = side;  //몬스터 종류

            mons[i].m->PrintMonster(x, y, side);  //몬스터 출력 gotoxy 오류???

            // 공격 생성
            AttackObj atk;
            atk.active = true;
            atk.type = (side < 2) ? 0 : 1;  //0,1이면 세로 2,3이면 가로 공격

            switch (side) {
            case 0: atk.x = x; atk.y = y + 2; atk.dx = 0; atk.dy = 1; break;  //위
            case 1: atk.x = x; atk.y = y - 2; atk.dx = 0; atk.dy = -1; break;  //아래
            case 2: atk.x = x + 3; atk.y = y; atk.dx = 1; atk.dy = 0; break;  //왼쪽
            case 3: atk.x = x - 3; atk.y = y; atk.dx = -1; atk.dy = 0; break; //오른쪽
            }
            attacks.push_back(atk);    //+  공격이 많다보니까 오류 걸리는듯?
        }

        // 몬스터 제거      
        Sleep(1000);
        for (int i = 0; i < monsterCount; i++) {
            mons[i].m->Clear(mons[i].x, mons[i].y);
            delete mons[i].m;
        }

        // 공격 이동
        for (auto& atk : attacks) {
            if (!atk.active) continue;

            // 공격 이동 전 클리어             <<<<<<<이 부분을 수정해야할 듯 플레이어랑 부딪히면 플레이어도 사라짐
            gotoxy(atk.x, atk.y);
            ::cout <<"  ";                 //cout가 아니라 객체로 만들어서 소멸시켜야 할 거 같은데   
            //ㄴ 아래쪽에서 올라오는 공격으로는 안사라짐, 왼->오 공격도 안사라지고 체력만 깎임(노트북일 때 플레이어 사라질 때도 있음)  | 
            //위에서 아래 공격사라짐, 오>왼 공격 다른 거랑 겹치면 크기가 커짐(노트북일 때 플레이어 사라짐)
            //왼->오 공격일 때 한 번씩 플레이어 위치가 옆으로 한 칸 움직임
            //atk.x += atk.dx;
            atk.x = atk.x + atk.dx * 2;
            //atk.y += atk.dy;
            atk.y = atk.y + atk.dy * 2;


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
            if (atk.type == 0) ::cout << "|";
            else ::cout << "─";
        }

        // 체력 검사
        if (p.getHP() <= 0) {
            gameRunning = false;
        }
        Sleep(400);                         //sleep 를 너무 많이 썻나
    }

    if (timer.joinable()) timer.join();

    gotoxy(0, 20);
    ::cout << "게임 종료! 아무 키나 누르세요...";
    _getch();
    return 0;
}
