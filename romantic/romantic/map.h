#include <iostream>
#include "player.h"
#include <conio.h>		// _getch() 함수 사용
#include <windows.h>	// system("cls") 및 콘솔 색상 설정
#include <fstream>		// 파일 입출력
#include <vector>
#include <algorithm>	// sort
#include <ctime>
#include <string>

using namespace std;

void setCursorPosition(int x, int y);	// 커서 위치 조정
void drawMenu(int selected);			// 시작 메뉴 출력
void drawMap(int width, int height);	// 맵 출력
int ingame();							// 인게임 메인 출력
void drawMapRe(int width, int height);	// 맵 재출력
void moveNumber(Player& p);

void User(Player& p);					// 유저 정보 입력
void saveScore(Player& p);	// 점수 저장  Player 클래스.score