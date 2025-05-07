#include "player.h"
#include "monster.h"

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

int main() {

    drawMap(30, 10);
    Player p;
    Monster m(5, 5);

    m.draw();
    p.move(&m);

    

    return 0;
}