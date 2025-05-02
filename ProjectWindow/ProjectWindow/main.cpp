#include <iostream>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include <conio.h>

using namespace std;

class Player {
    int HP; //체력
    int attack; //공격력
    int move; //움직일 수 있는 횟수
    int score; //점수
public:
    //Player(int hp = 5, int at = 5, int mv = 5, int sc = 0) : HP(hp), attack(at), move(mv), score(sc) {}
    Player(int mv = 0) : move(mv) {}
    void Moving()
    {
        cout << "움직일 수 있는 횟수: " << move << endl;
    }
    int MinusMv() {
        return move -=1;
    }
};

class Enemy {
	int Hp; //체력 > Player HP상속받아서 사용해도됨
	int damge; //데미지
    //생성시키는 방법  
};

void playermove() //플레이어 움직이게 만드는 함수 + 마지막방향으로 공격나가게 하기 ↑★
{

}

void Map() //게임 맵 
{
     
}

void Title() //시작 화면 -> 게임 맵 넣어가는 거
{

}

void Timer(int cnt, Player &p, int mv)  //타이머(남은 시간)
{
    for (int i = cnt; i >= 0; --i) 
    {
        system("cls");
        cout<<"남은 시간: " << i << endl;
        p.Moving();
        p.MinusMv();
        Sleep(1000);
    }
    //system("cls");
    cout << "끝" <<endl;
}

int main() {
    srand((unsigned int)time(NULL));

    int ctime=3; //남은 시간
    int moving = rand() % 10;

    Player p(moving);

    Timer(ctime, p, moving);

    cout << "'";
    return 0;
}