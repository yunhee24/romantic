#include "map.h"

const int MENU_COUNT = 4;
const string menuItems[MENU_COUNT] = {
    "게임 시작",
    "게임 랭킹",
    "게임 방법",
    "게임 종료"
};

// 시작 메뉴 출력
void drawMenu(int selected) {
    system("cls");

    cout << "┌────────────────────────────────────────────┐\n";
    cout << "│                                            │\n";
    cout << "│      ######   ###   #     #  #####         │\n";
    cout << "│      #       #   #  ##   ##  #             │\n";
    cout << "│      #  ###  #####  # # # #  #####         │\n";
    cout << "│      #   ##  #   #  #  #  #  #             │\n";
    cout << "│      ######  #   #  #     #  #####         │\n";
    cout << "│                                            │\n";
    cout << "│      #     #  #####  #    #  #####         │\n";
    cout << "│      ##   ##  #      ##   #  #             │\n";
    cout << "│      # # # #  #####  # #  #  #####         │\n";
    cout << "│      #  #  #  #      #  # #  #             │\n";
    cout << "│      #     #  #####  #    #  #####         │\n";
    cout << "│                                            │\n";
    cout << "├──────────────── GAME MENU ─────────────────┤\n";

    for (int i = 0; i < MENU_COUNT; ++i) {
        cout << "│ ";
        if (i == selected) {
            cout << ">> " << menuItems[i] << string(40 - menuItems[i].length(), ' ') << "│\n";
        }
        else {
            cout << "   " << menuItems[i] << string(40 - menuItems[i].length(), ' ') << "│\n";
        }
    }

    cout << "└────────────────────────────────────────────┘\n";
    cout << "  ↑↓ 방향키로 이동, Enter로 선택하세요.\n";
}

// 맵 출력
void drawMap(int width, int height) {
    int offsetX = 20; // 가로 위치 조정 (공백)
    int offsetY = 3;  // 세로 위치 조정 (줄바꿈)

    // 위로 공백 줄 삽입
    for (int i = 0; i < offsetY; ++i) {
        cout << endl;
    }

    for (int y = 0; y < height; ++y) {
        // 왼쪽으로 공백 삽입
        for (int i = 0; i < offsetX; ++i) {
            cout << " ";
        }

        for (int x = 0; x < width; ++x) {
            if (y == 0 && x == 0) cout << "┌";
            else if (y == 0 && x == width - 1) cout << "┐";
            else if (y == height - 1 && x == 0) cout << "└";
            else if (y == height - 1 && x == width - 1) cout << "┘";
            else if (y == 0 || y == height - 1) cout << "─";
            else if (x == 0 || x == width - 1) cout << "│";
            else cout << " ";
        }

        cout << endl;
    }

    cout << endl;
}

int ingame() {
    const int width = 32;
    const int height = 16;
    int selected = 0;

    while (true) {
        drawMenu(selected);
        int key = _getch();

        if (key == 224) {  // 방향키 입력 시
            key = _getch();
            if (key == 72 && selected > 0) { // ↑
                --selected;
            }
            else if (key == 80 && selected < MENU_COUNT - 1) { // ↓
                ++selected;
            }
        }
        else if (key == 13) { // Enter 입력 시
            system("cls");
            if (selected == 0) {
                cout << "\n";
                drawMap(width, height);
                break;
            }
            else if (selected == 1) {
                cout << "[게임 랭킹] 기능은 아직 구현되지 않았습니다.\n";
            }
            else if (selected == 2) {
                cout << "[게임 방법]\n";
                cout << "- 방향키로 이동하세요.\n- 목표를 향해 나아가세요.\n";
            }
            else if (selected == 3) {
                cout << "게임을 종료합니다.\n";
                return 0;
            }
            cout << "\n아무 키나 누르면 메뉴로 돌아갑니다...\n";
            _getch();
        }
    }
    return 0;
}