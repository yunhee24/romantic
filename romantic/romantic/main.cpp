#include "player.h"
#include "monster.h"
#include "map.h"
#include "Timer.h"
#include <future>
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