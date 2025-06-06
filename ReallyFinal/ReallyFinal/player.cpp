#include "player.h"
#include "monster.h"
#include "map.h"
#include "attack.h"
#include <mutex>
extern std::mutex output_mutex;
extern bool gamerun;

Player::Player() {
    //x = 18;  32*16
    //y = 10;
    x = 14;    //28*14
    y = 9;
    moveCount = 0;
    lastDir = NONE;
    score = 0;
    hp = 5; // 초기 체력 설정
}

int Player::getScore() const {
    return score;
}

void Player::draw() {
    std::lock_guard<std::mutex> lock(output_mutex);
    gotoxy(x * 2, y);
    cout << "♀";

    gotoxy(0, 2);
    cout << "체력: ";
    for (int i = 0; i < hp; ++i) cout << "♥";
    cout << "   ";
}

void Player::remove() {
    std::lock_guard<std::mutex> lock(output_mutex);
    gotoxy(x * 2, y);
    cout << "  ";
}

int Player::getX() const { return x; }
int Player::getY() const { return y; }

void Player::decreaseHP() {
    hp--;
    draw();
    if (hp <= 0)
        gamerun = false;
}

int Player::getHP() {
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
            drawMapRe(28, 14);                               // <<<<맵 재생성

            for (auto& m : monsters) {
                if (m.alive && (m.x / 2) == dx && m.y == dy) {
                    m.alive = false;
                    score++;
                    {
                        std::lock_guard<std::mutex> lock(output_mutex);
                        gotoxy(0, 3);
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
                case 80: if (y < 15) { y++; moved = true; lastDir = DOWN; } break;    //y<17
                case 75: if (x > 11) { x--; moved = true; lastDir = LEFT; } break;
                case 77: if (x < 22) { x++; moved = true; lastDir = RIGHT; } break;   //x<25
                }
                if (moved) {
                    moveCount++;
                    {
                        std::lock_guard<std::mutex> lock(output_mutex);
                        gotoxy(0, 6);
                        cout << "이동 " << moveCount << "   ";
                    }

                    UpdateAttacks(this);     //몬스터 공격 이동 시키기

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