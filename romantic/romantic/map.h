#include <iostream>
#include "player.h"
#include <conio.h>  // _getch() 함수 사용
#include <windows.h>  // system("cls") 및 콘솔 색상 설정
#include <fstream>  // 파일 입출력
#include <vector>
#include <algorithm>  // sort
#include <ctime>

using namespace std;

void drawMenu(int selected);			// 시작 메뉴 출력
void drawMap(int width, int height);	// 게임 맵 출력
int ingame();							// 맵 호출
void drawMapRe(int width, int height);	// 맵 다시 출력
void User(Player& p);					// 점수
void saveScore(const string& name, int score);	// 점수 저장  Player 클래스.score