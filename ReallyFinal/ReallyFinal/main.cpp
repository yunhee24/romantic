#include "player.h"
#include "monster.h"
#include "map.h"
#include "Timer.h"   //Ÿ�̸� map.cpp -> ingame�ȿ� ����
#include <thread>   //Ÿ�̸�
#include <future>
using namespace std;

int main() {

    CursorView();

    ingame();

    return 0;
}