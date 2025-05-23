#include "player.h"
#include "monster.h"
#include "map.h"
#include <future>
using namespace std;

int main() {
    CursorView();

    Player p;

    ingame();
    
    User(p);    // 인게임 앞으로 빼는거 수정필요.

    Monster m(12, 3);

    m.draw();
    p.move(&m);  // 몬스터 처치하면 점수 오름

    saveScore(p.name, p.score);  // 종료 시 최종 점수 저장(Player 클래스로 받음)

    return 0;
}