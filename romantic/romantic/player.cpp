#include "player.h"
#include "monster.h"
#include "map.h"

// 콘솔창 커서 숨김 함수
void CursorView() {
    CONSOLE_CURSOR_INFO cursorInfo = { 0, };
    cursorInfo.dwSize = 1;                  // 커서 굵기 (1 ~ 100)
    cursorInfo.bVisible = FALSE;            // 커서 Visible TRUE(보임) / FALSE(숨김)
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

// 키보드 입력 함수
void gotoxy(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

Player::Player() {
    x = 18;
    y = 10;
    moveCount = 0;
    lastDir = NONE;
    score = 0;
}

int Player::getScore() const {
    return score;
}

void Player::draw() {
    gotoxy(x * 2, y);
    cout << "♀";
}

void Player::remove() {
    gotoxy(x * 2, y);
    cout << " ";
}

// 플레이어 공격
void Player::attack(Monster* m) {
    int tx = x, ty = y;

    switch (lastDir) {
    case UP:    ty -= 1; break;
    case DOWN:  ty += 1; break;
    case LEFT:  tx -= 1; break;
    case RIGHT: tx += 1; break;
    default: return;
    }

    int dx = tx, dy = ty;

    for (int i = 1; i < 3; i++) {
        if (dx >= 6 && dx <= 26 && dy >= 2 && dy <= 20) {
            gotoxy(dx * 2, dy);
            cout << "※";
            draw();
            Sleep(120);
            gotoxy(dx * 2, dy);
            cout << " ";
            drawMapRe(32, 16);

            // 몬스터 충돌 체크
            if (m->alive && dx == m->x && dy == m->y) {
                m->remove();
                score++;
                gotoxy(0, 7);
                cout << "점수 " << score << "   ";
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

// 플레이어 움직임
void Player::move(Monster* m) {
    char in;
    draw();

    while (true) {
        if (_kbhit()) {
            in = _getch();

            if (in == 0 || in == -32 || in == 224) {
                in = _getch();

                remove();
                bool moved = false;

                switch (in) {
                case 72:
                    if (y > 4) { y--; moved = true; lastDir = UP; }
                    break;
                case 80:
                    if (y < 17) { y++; moved = true; lastDir = DOWN; }
                    break;
                case 75:
                    if (x > 11) { x--; moved = true; lastDir = LEFT; }
                    break;
                case 77:
                    if (x < 25) { x++; moved = true; lastDir = RIGHT; }
                    break;
                }

                // 움직임 횟수
                if (moved) {
                    moveCount++;
                    gotoxy(0, 6);
                    cout << "이동 " << moveCount << "   ";

                    if (moveCount == randNumber) {
                        attack(m);
                        moveCount = 0;
                    }
                }

                draw();
            }
        }
    }
}
