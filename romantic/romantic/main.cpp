#include "player.h"
#include "monster.h"
#include "map.h"
#include <future>
using namespace std;

int main() {
    CursorView();

    Player p;

    ingame();
    
    User(p);    // �ΰ��� ������ ���°� �����ʿ�.

    Monster m(12, 3);

    m.draw();
    p.move(&m);  // ���� óġ�ϸ� ���� ����

    saveScore(p.name, p.score);  // ���� �� ���� ���� ����(Player Ŭ������ ����)

    return 0;
}