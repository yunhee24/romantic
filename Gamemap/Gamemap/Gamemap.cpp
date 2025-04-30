#include <iostream>
#include <conio.h>  // _getch() �Լ� ���
using namespace std;

// �� ��� �Լ�
void drawMap(int width, int height) {
    for (int y = 0; y < height; ++y) {
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
}

int main() {
    const int width = 32;
    const int height = 16;

    // ���� ��ŸƮ ȭ��
    cout << "=====================\n";
    cout << "   GAME START!    \n";
    cout << "=====================\n";
    cout << "�ƹ� Ű�� ������ �����մϴ�.\n";

    _getch();  // Ű �Է� ���

    system("cls");  // �ܼ� ȭ�� ����� (Windows ����)

    // �� ���
    drawMap(width, height);

    return 0;
}
