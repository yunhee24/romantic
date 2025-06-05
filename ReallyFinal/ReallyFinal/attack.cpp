#include "attack.h"
#include "map.h"
#include "gotoxy.h"
#include "player.h"
#include <iostream>
#include <mutex>
#include <windows.h>

extern std::mutex output_mutex;
extern bool gamerun;

std::vector<AttackObj> attacks;


void CreateAttack(int dir, int x, int y) {
    AttackObj atk;
    atk.active = true;

    switch (dir) {
    case 0: // 위쪽에서 아래로
        atk.x = x;
        atk.y = MAP_TOP+1;
        atk.dx = 0;
        atk.dy = 1;
        atk.symbol = "|";
        break;
    case 1: // 아래쪽에서 위로
        atk.x = x;
        atk.y = MAP_BOTTOM;
        atk.dx = 0;
        atk.dy = -2;
        atk.symbol = "|";
        break;
    case 2: // 왼쪽에서 오른쪽
        atk.x = x + 3;
        atk.y = y;
        atk.dx = 1;
        atk.dy = 0;
        atk.symbol = "─";
        break;
    case 3: // 오른쪽에서 왼쪽
        atk.x = MAP_RIGHT - 4;
        atk.y = y;
        atk.dx = -1;
        atk.dy = 0;
        atk.symbol = "─";
        break;
    }

    attacks.push_back(atk);
}

void UpdateAttacks(Player* player)
{
    for (auto& atk : attacks) {
        if (!atk.active) continue;

        {
            std::lock_guard<std::mutex> lock(output_mutex);
            gotoxy(atk.x, atk.y);
            std::cout << "  ";
        }

        atk.x += atk.dx;
        atk.y += atk.dy;

        if (atk.x == player->getX()*2 && atk.y == player->getY()) {
            atk.active = false;
            player->decreaseHP();

            {
                std::lock_guard<std::mutex> lock(output_mutex);
                gotoxy(0, 2);  // 체력 표시 위치
                std::cout << "체력: " << player->getHP() << "  ";
            }

            if (player->getHP() > 0) {
                player->draw();  // 플레이어 다시 그리기
            }
        }
        
        if (atk.x < MAP_LEFT + 1 || atk.x > MAP_RIGHT - 2 || atk.y < MAP_TOP + 2 || atk.y > MAP_BOTTOM - 2) {
            atk.active = false;
            continue;
        }

        {
            std::lock_guard<std::mutex> lock(output_mutex);
            gotoxy(atk.x, atk.y);
            std::cout << atk.symbol;
        }
    }
}
