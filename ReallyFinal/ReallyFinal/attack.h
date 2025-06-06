#pragma once
#include <vector>
#include <string>

class Player;

// 공격 객체 구조체
struct AttackObj {
    int x, y;        // 위치
    int dx, dy;      // 이동 방향
    std::string symbol;     // 출력 기호 (' | ' 또는 '━')
    bool active;     // 공격이 살아있는지 여부
};

extern std::vector<AttackObj> attacks;  // 공격 객체 저장 벡터

void CreateAttack(int dir, int x, int y);
void UpdateAttacks(Player* player);