#pragma once
#include <vector>
#include <string>

class Player;

// ���� ��ü ����ü
struct AttackObj {
    int x, y;        // ��ġ
    int dx, dy;      // �̵� ����
    std::string symbol;     // ��� ��ȣ (' | ' �Ǵ� '��')
    bool active;     // ������ ����ִ��� ����
};

extern std::vector<AttackObj> attacks;  // ���� ��ü ���� ����

void CreateAttack(int dir, int x, int y);
void UpdateAttacks(Player* player);
