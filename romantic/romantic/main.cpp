#include "player.h"
#include "monster.h"
#include "map.h"
using namespace std;

int main() {

    CursorView();

    ingame();

    Player p;
    Monster m(12, 3);

    m.draw();
    p.move(&m);

    return 0;
}