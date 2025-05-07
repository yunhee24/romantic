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
    while (gameRunning) {                              //�ݺ������� �ٲ㺸��
        gotoxy(0, 0);  //���� ���� �� ���� �鳲
        int timecount = (GetTickCount64() - start_time) / 1000;
        int remain = total - timecount;
        //gotoxy(0, 0);
        ::cout << "���� �ð�: " << remain << "��   ";
        //Sleep(200);  //�̰� ������ ���� �����Ǵ� ������ ���� �ð��� ��µ�
        if (remain <= 0) {
            gameRunning = false;
            break;
        }
        Sleep(200);
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
    int x = 30, y = 6;  // �÷��̾� ��ǥ
    string pl[1] = { "��" };
public:
    void PrintPlayer(int x, int y) {
        gotoxy(x, y);
        ::cout << pl[0];
        this->x = x;   // ��ġ ����
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
        ::cout << "���� ü��: " << getHP() << "  ";
    }
};
/*
void PrintPlayerHP(Player& p)
{
    gotoxy(0, 1);
    cout<< "���� ü��: "<< p.getHP() << "   ";
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
            if (y == 0 && x == 0) ::cout << "��";
            else if (y == 0 && x == width - 1) ::cout << "��";
            else if (y == height - 1 && x == 0) ::cout << "��";
            else if (y == height - 1 && x == width - 1) ::cout << "��";
            else if (y == 0 || y == height - 1) ::cout << "��";
            else if (x == 0 || x == width - 1) ::cout << "��";
            else ::cout << " ";
        }
        ::cout << endl;
    }
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
        p.PrintPlayerHP();

        int monsterCount = rand() % 2 + 1 ; //���� ���� �� ����

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
            mons[i].type = side;  //���� ����

            mons[i].m->PrintMonster(x, y, side);  //���� ��� gotoxy ����???

            // ���� ����
            AttackObj atk;
            atk.active = true;
            atk.type = (side < 2) ? 0 : 1;  //0,1�̸� ���� 2,3�̸� ���� ����

            switch (side) {
            case 0: atk.x = x; atk.y = y + 2; atk.dx = 0; atk.dy = 1; break;  //��
            case 1: atk.x = x; atk.y = y - 2; atk.dx = 0; atk.dy = -1; break;  //�Ʒ�
            case 2: atk.x = x + 3; atk.y = y; atk.dx = 1; atk.dy = 0; break;  //����
            case 3: atk.x = x - 3; atk.y = y; atk.dx = -1; atk.dy = 0; break; //������
            }
            attacks.push_back(atk);    //+  ������ ���ٺ��ϱ� ���� �ɸ��µ�?
        }

        // ���� ����      
        Sleep(1000);
        for (int i = 0; i < monsterCount; i++) {
            mons[i].m->Clear(mons[i].x, mons[i].y);
            delete mons[i].m;
        }

        // ���� �̵�
        for (auto& atk : attacks) {
            if (!atk.active) continue;

            // ���� �̵� �� Ŭ����             <<<<<<<�� �κ��� �����ؾ��� �� �÷��̾�� �ε����� �÷��̾ �����
            gotoxy(atk.x, atk.y);
            ::cout <<"  ";                 //cout�� �ƴ϶� ��ü�� ���� �Ҹ���Ѿ� �� �� ������   
            //�� �Ʒ��ʿ��� �ö���� �������δ� �Ȼ����, ��->�� ���ݵ� �Ȼ������ ü�¸� ����(��Ʈ���� �� �÷��̾� ����� ���� ����)  | 
            //������ �Ʒ� ���ݻ����, ��>�� ���� �ٸ� �Ŷ� ��ġ�� ũ�Ⱑ Ŀ��(��Ʈ���� �� �÷��̾� �����)
            //��->�� ������ �� �� ���� �÷��̾� ��ġ�� ������ �� ĭ ������
            //atk.x += atk.dx;
            atk.x = atk.x + atk.dx * 2;
            //atk.y += atk.dy;
            atk.y = atk.y + atk.dy * 2;


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
            if (atk.type == 0) ::cout << "|";
            else ::cout << "��";
        }

        // ü�� �˻�
        if (p.getHP() <= 0) {
            gameRunning = false;
        }
        Sleep(400);                         //sleep �� �ʹ� ���� ����
    }

    if (timer.joinable()) timer.join();

    gotoxy(0, 20);
    ::cout << "���� ����! �ƹ� Ű�� ��������...";
    _getch();
    return 0;
}
