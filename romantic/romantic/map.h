#include <iostream>
#include "player.h"
#include <conio.h>  // _getch() �Լ� ���
#include <windows.h>  // system("cls") �� �ܼ� ���� ����
#include <fstream>  // ���� �����
#include <vector>
#include <algorithm>  // sort
#include <ctime>

using namespace std;

void drawMenu(int selected);			// ���� �޴� ���
void drawMap(int width, int height);	// ���� �� ���
int ingame();							// �� ȣ��
void drawMapRe(int width, int height);	// �� �ٽ� ���
void User(Player& p);					// ����
void saveScore(const string& name, int score);	// ���� ����  Player Ŭ����.score