#include <iostream>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include <conio.h>

using namespace std;

class Player {
    int HP; //ü��
    int attack; //���ݷ�
    int move; //������ �� �ִ� Ƚ��
    int score; //����
public:
    //Player(int hp = 5, int at = 5, int mv = 5, int sc = 0) : HP(hp), attack(at), move(mv), score(sc) {}
    Player(int mv = 0) : move(mv) {}
    void Moving()
    {
        cout << "������ �� �ִ� Ƚ��: " << move << endl;
    }
    int MinusMv() {
        return move -=1;
    }
};

class Enemy {
	int Hp; //ü�� > Player HP��ӹ޾Ƽ� ����ص���
	int damge; //������
    //������Ű�� ���  
};

void playermove() //�÷��̾� �����̰� ����� �Լ� + �������������� ���ݳ����� �ϱ� ���
{

}

void Map() //���� �� 
{
     
}

void Title() //���� ȭ�� -> ���� �� �־�� ��
{

}

void Timer(int cnt, Player &p, int mv)  //Ÿ�̸�(���� �ð�)
{
    for (int i = cnt; i >= 0; --i) 
    {
        system("cls");
        cout<<"���� �ð�: " << i << endl;
        p.Moving();
        p.MinusMv();
        Sleep(1000);
    }
    //system("cls");
    cout << "��" <<endl;
}

int main() {
    srand((unsigned int)time(NULL));

    int ctime=3; //���� �ð�
    int moving = rand() % 10;

    Player p(moving);

    Timer(ctime, p, moving);

    cout << "'";
    return 0;
}