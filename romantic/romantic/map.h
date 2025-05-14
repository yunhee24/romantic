#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;

void drawMenu(int selected);			// 시작 메뉴 출력
void drawMap(int width, int height);	// 게임 맵 출력
int ingame();							// 맵 호출
void drawMapRe(int width, int height);	// 맵 다시 출력