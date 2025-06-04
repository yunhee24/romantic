#include <iostream>
#include "player.h"
#include <conio.h>  // _getch() �Լ� ���
#include <windows.h>  // system("cls") �� �ܼ� ���� ����
#include <fstream>  // ���� �����
#include <vector>
#include <algorithm>  // sort
#include <ctime>
#include <string>

using namespace std;

enum map_pos {   // �� ��ǥ
    MAP_LEFT = 20,
    MAP_RIGHT = 47,
    MAP_TOP = 3,
    MAP_BOTTOM = 18
};

void setCursorPosition(int x, int y);	// Ŀ�� ��ġ ����
void drawMenu(int selected);			// ���� �޴� ���
void drawMap(int width, int height);	// �� ���
int ingame();							// �ΰ��� ���� ���
void drawMapRe(int width, int height);	// �� �����
void moveNumber(Player& p);

void User(Player& p);					// ���� ���� �Է�
void saveScore(const Player& player);	// ���� ����  Player Ŭ����.score