#include "player.h"
#include "monster.h"

int main() {
    Player p;
    Monster m(5, 5);

    m.draw();
    p.move(&m);

    return 0;
}