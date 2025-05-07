#include "player.h"

void gotoxy(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

Player::Player() {
    x = 0;
    y = 0;
    moveCount = 0;
    lastDir = NONE;
}

void Player::draw() {
    gotoxy(x * 2, y);
    cout << "●";
}

void Player::remove() {
    gotoxy(x * 2, y);
    cout << " ";
}

void Player::attack() {
    int tx = x, ty = y;

    // 공격 방향에 따라 <초기> 위치 설정
    switch (lastDir) {
    case UP:    ty -= 1; break;
    case DOWN:  ty += 1; break;
    case LEFT:  tx -= 1; break;
    case RIGHT: tx += 1; break;
    default: return;
    }

    // 별 이동 위치
    int dx = tx, dy = ty;

    for (int i = 0; i < 3; i++) {
        if (dx >= 0 && dx <= 7 && dy >= 0 && dy <= 4) { // 이동 및 공격 범위
            gotoxy(dx * 2, dy);
            cout << "☆";
            draw();       // 플레이어 출력
            Sleep(100);
            gotoxy(dx * 2, dy);
            cout << " ";  // 공백 출력
        }

        // 이동 방향에 따라 별의 위치 새로 입력
        switch (lastDir) {
        case UP:    dy--; break;
        case DOWN:  dy++; break;
        case LEFT:  dx--; break;
        case RIGHT: dx++; break;
        }
    }
}


void Player::move() {
    char input;
    draw();

    while (true) {
        if (_kbhit()) {
            input = _getch();
            remove();

            bool moved = false;

            switch (input) {
            case 'w': case 'W':
                if (y > 0) { y--; moved = true; lastDir = UP; }
                break;
            case 's': case 'S':
                if (y < 4) { y++; moved = true; lastDir = DOWN; }
                break;
            case 'a': case 'A':
                if (x > 0) { x--; moved = true; lastDir = LEFT; }
                break;
            case 'd': case 'D':
                if (x < 7) { x++; moved = true; lastDir = RIGHT; }
                break;
            case 'q': case 'Q':
                return;
            }

            if (moved) {
                moveCount++;
                gotoxy(0, 6);
                cout << "이동 " << moveCount << "   ";

                if (moveCount == 4) {
                    attack();
                    moveCount = 0;
                }
            }

            draw();
        }
    }
}
