#pragma once

class Monster {
public:
    int x, y;
    bool alive;

    Monster(int startX, int startY);
    void draw();
    void remove();
};