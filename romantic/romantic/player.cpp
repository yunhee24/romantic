#include "player.h"
#include "monster.h"
#include "map.h"

// �ܼ�â Ŀ�� ���� �Լ�
void CursorView() {
    CONSOLE_CURSOR_INFO cursorInfo = { 0, };
    cursorInfo.dwSize = 1;                  // Ŀ�� ���� (1 ~ 100)
    cursorInfo.bVisible = FALSE;            // Ŀ�� Visible TRUE(����) / FALSE(����)
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void gotoxy(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

Player::Player() {
    x = 11;
    y = 5;
    moveCount = 0;
    lastDir = NONE;
    score = 0;
}

void Player::draw() {
    gotoxy(x * 2, y);
    cout << "��";
}

void Player::remove() {
    gotoxy(x * 2, y);
    cout << " ";
}

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

    for (int i = 0; i < 3; i++) {
        if (dx >= 9 && dx <= 26 && dy >= 3 && dy <= 20) {
            gotoxy(dx * 2, dy);
            cout << "��";
            draw();
            Sleep(100);
            gotoxy(dx * 2, dy);
            cout << " ";

            // ���� �浹 üũ
            if (m->alive && dx == m->x && dy == m->y) {
                m->remove();
                score++;
                gotoxy(0, 7);
                cout << "���� " << score << "   ";
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

void Player::move(Monster* m) {
    char input;
    draw();

    while (true) {
        if (_kbhit()) {
            input = _getch();
            remove();

            bool moved = false;

            switch (input) {
            case 'w': case 'W':
                if (y > 5) { y--; moved = true; lastDir = UP; }
                break;
            case 's': case 'S':
                if (y < 18) { y++; moved = true; lastDir = DOWN; }
                break;
            case 'a': case 'A':
                if (x > 11) { x--; moved = true; lastDir = LEFT; }
                break;
            case 'd': case 'D':
                if (x < 25) { x++; moved = true; lastDir = RIGHT; }
                break;
            case 'q': case 'Q':
                return;
            }

            if (moved) {
                moveCount++;
                gotoxy(0, 6);
                cout << "�̵� " << moveCount << "   ";

                if (moveCount == 4) {
                    attack(m);
                    moveCount = 0;
                }
            }

            draw();
        }
    }
}
