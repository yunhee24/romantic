#include "player.h"
#include "monster.h"

void drawMap(int width, int height) {
    int offsetX = 20; // ���� ��ġ ���� (����)
    int offsetY = 3;  // ���� ��ġ ���� (�ٹٲ�)

    // ���� ���� �� ����
    for (int i = 0; i < offsetY; ++i) {
        cout << endl;
    }

    for (int y = 0; y < height; ++y) {
        // �������� ���� ����
        for (int i = 0; i < offsetX; ++i) {
            cout << " ";
        }

        for (int x = 0; x < width; ++x) {
            if (y == 0 && x == 0) cout << "��";
            else if (y == 0 && x == width - 1) cout << "��";
            else if (y == height - 1 && x == 0) cout << "��";
            else if (y == height - 1 && x == width - 1) cout << "��";
            else if (y == 0 || y == height - 1) cout << "��";
            else if (x == 0 || x == width - 1) cout << "��";
            else cout << " ";
        }
        cout << endl;
    }

    cout << endl;
}

int main() {

    drawMap(20,10);
    Player p;
    Monster m(5, 5);

    m.draw();
    p.move(&m);



    return 0;
}