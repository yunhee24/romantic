#pragma once
#include <iostream>
#include <conio.h>  // _getch() �Լ� ���
#include "gotoxy.h"
using namespace std;



// �� ��� �Լ�
void drawMap(int X, int Y, int width, int height) {
    for (int y = 0; y < height; ++y) {
        gotoxy(X, Y + y);
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