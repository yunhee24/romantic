#include <iostream>
#include <conio.h>  // _getch() �Լ� ���
#include <windows.h>  // system("cls") �� �ܼ� ���� ����
using namespace std;


// �� ��� �Լ�
void drawMap(int width, int height) {
    int offsetX = 20; // ���� ��ġ ���� (����)
    int offsetY = 4;  // ���� ��ġ ���� (�ٹٲ�)

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