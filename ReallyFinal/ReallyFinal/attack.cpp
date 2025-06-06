#include "attack.h"
#include "map.h"
#include "gotoxy.h"
#include "player.h"
#include "Timer.h"
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
    case 0: // ���ʿ��� �Ʒ���
        atk.x = x;
        atk.y = MAP_TOP + 1;
        atk.dx = 0;
        atk.dy = 1;
        atk.symbol = "|";
        break;
    case 1: // �Ʒ��ʿ��� ����
        atk.x = x;
        atk.y = MAP_BOTTOM - 1;
        atk.dx = 0;
        atk.dy = -1;
        atk.symbol = "|";
        break;
    case 2: // ���ʿ��� ������
        atk.x = x + 3;
        atk.y = y;
        atk.dx = 1;
        atk.dy = 0;
        atk.symbol = "��";
        break;
    case 3: // �����ʿ��� ����
        atk.x = MAP_RIGHT - 3;
        atk.y = y;
        atk.dx = -1;
        atk.dy = 0;
        atk.symbol = "��";
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

        if ((atk.x / 2) == player->getX() && atk.y == player->getY()) //�÷��̾� ü�� ��ȣ�ۿ� �̻���
        {
            atk.active = false;
            player->decreaseHP();

            if (player->getHP() > 0) {
                player->draw();  // �÷��̾� �ٽ� �׸���
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