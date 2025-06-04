#include "player.h"
#include "monster.h"
#include "map.h"
#include "Timer.h"   //타이머 map.cpp -> ingame안에 있음
#include <thread>   //타이머
#include <future>
using namespace std;

int main() {

    CursorView();

    ingame();

    return 0;
}