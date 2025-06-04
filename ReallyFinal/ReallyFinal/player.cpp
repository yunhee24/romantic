#include "player.h"
#include "monster.h"
#include "map.h"
#include <mutex>
extern std::mutex output_mutex;
extern bool gamerun;

Player::Player() {
    x = 18;
    y = 10;
    moveCount = 0;
    lastDir = NONE;
    score = 0;
    hp = 3; // 초기 체력 설정
}

int Player::getScore() const {
    return score;
}

void Player::draw() {
    std::lock_guard<std::mutex> lock(output_mutex);
    gotoxy(x * 2, y);
    cout << "♀";
}

void Player::remove() {
    std::lock_guard<std::mutex> lock(output_mutex);
    gotoxy(x * 2, y);
    cout << "  ";
}

void Player::decreaseHP() {
    hp--;
    std::lock_guard<std::mutex> lock(output_mutex);
    gotoxy(60, 2);
    cout << "체력: " << hp << "  ";
}

int Player::getHP() const {
    return hp;
}

void Player::attack(std::vector<Monster>& monsters) {
    int tx = x, ty = y;
    switch (lastDir) {
    case UP:    ty -= 1; break;
    case DOWN:  ty += 1; break;
    case LEFT:  tx -= 1; break;
    case RIGHT: tx += 1; break;
    default: return;
    }

    int dx = tx, dy = ty;
    for (int i = 1; i < 6; i++) {
        if (dx >= 6 && dx <= 26 && dy >= 2 && dy <= 20) {
            {
                std::lock_guard<std::mutex> lock(output_mutex);
                gotoxy(dx * 2, dy);
                cout << "※";
            }
            draw();
            Sleep(120);
            {
                std::lock_guard<std::mutex> lock(output_mutex);
                gotoxy(dx * 2, dy);
                cout << "  ";
            }
            drawMapRe(32, 16);

            for (auto& m : monsters) {
                if (m.alive && (m.x/2) == dx && m.y == dy) {
                    m.alive = false;
                    score++;
                    {
                        std::lock_guard<std::mutex> lock(output_mutex);
                        gotoxy(60, 1);
                        cout << "점수: " << score << "  ";

                        gotoxy(m.x * 2, m.y);
                        cout << "  ";
                    }
                    break;
                }
            }
        }
        switch (lastDir) {
        case UP:    dy--; break;
        case DOWN:  dy++; break;
        case LEFT:  dx--; break;
        case RIGHT: dx++; break;
        }
    }
}

void Player::move(std::vector<Monster>& monsters) {
    char in;
    draw();
    while (gamerun) {
        if (_kbhit()) {
            in = _getch();
            if (in == 0 || in == -32 || in == 224) {
                in = _getch();
                remove();
                bool moved = false;
                switch (in) {
                case 72: if (y > 4) { y--; moved = true; lastDir = UP; } break;
                case 80: if (y < 17) { y++; moved = true; lastDir = DOWN; } break;
                case 75: if (x > 11) { x--; moved = true; lastDir = LEFT; } break;
                case 77: if (x < 25) { x++; moved = true; lastDir = RIGHT; } break;
                }
                if (moved) {
                    moveCount++;
                    {
                        std::lock_guard<std::mutex> lock(output_mutex);
                        gotoxy(0, 6);
                        cout << "이동 " << moveCount << "   ";
                    }
                    if (moveCount == randNumber) {
                        attack(monsters);
                        moveCount = 0;
                    }
                }
                draw();
            }
        }
    }
}
