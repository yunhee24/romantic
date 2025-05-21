﻿#include <iostream>
#include "timer.h" 
#include <conio.h>  // _getch() 함수 사용
#include <windows.h>  // system("cls") 및 콘솔 색상 설정
#include <fstream>  // 파일 입출력
#include <vector>
#include <algorithm>  // sort
#include <ctime>

using namespace std;

const int MENU_COUNT = 4;
const string menuItems[MENU_COUNT] = {
    "게임 시작",
    "게임 랭킹",
    "게임 방법",
    "게임 종료"
};

struct ScoreEntry {
    string name;
    int score;
};

bool compareByScore(const ScoreEntry& a, const ScoreEntry& b) {
    return a.score > b.score;  // 높은 점수 먼저
}


int getDisplayWidth(const string& text) {
    int width = 0;
    for (unsigned char ch : text) {
        width += (ch & 0x80) ? 2 : 1;
    }
    return width;
}

void printGameInstructions() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    cout << "[게임 방법]\n";

    // 🔴 빨간색 설정
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
    cout << "- 키보드 ↑ ↓ ← → 방향키로 플레이어를 조작할 수 있습니다.\n";

    // ⚪ 흰색(기본색) 복원
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    cout << "- 매 턴마다 랜덤으로 숫자가 부여되고, 해당 숫자만큼 이동할 수 있습니다.\n";
    cout << "- 이동이 끝나면, 마지막으로 움직인 방향으로 자동으로 공격이 나갑니다.\n";
    cout << "- 적의 공격은 **플레이어가 움직였을 때만** 동작합니다.\n";
    cout << "  (즉, 플레이어가 가만히 있으면 적도 공격하지 않습니다!)\n";
    cout << "- 제한된 시간 동안 몬스터를 처치하며 최대한 많은 점수를 획득하세요.\n";
}


void showRanking() {
    ifstream file("scores.txt");
    vector<ScoreEntry> rankings;

    if (!file) {
        cout << "랭킹 파일이 없습니다.\n";
        return;
    }

    string name;
    int score;
    while (file >> name >> score) {
        rankings.push_back({ name, score });
    }
    file.close();

    sort(rankings.begin(), rankings.end(), compareByScore);

    cout << "===== 게임 랭킹 =====\n";
    for (size_t i = 0; i < rankings.size(); ++i) {
        cout << i + 1 << ". " << rankings[i].name << " - " << rankings[i].score << endl;
    }
}

void saveScore(const string& name, int score) {
    ofstream file("scores.txt", ios::app);  // append 모드
    if (file) {
        file << name << " " << score << endl;
        file.close();
    }
    else {
        cout << "랭킹 저장 실패!\n";
    }
}

void drawMenu(int selected) {
    system("cls");

    cout << "\n";
    cout << R"(
  _____                            _   _____                  _            _ 
 /  __ \                          | | /  ___|                (_)          | |
 | /  \/ ___  _ __  ___  ___   ___| | \ `--. _   _ _ ____   _____   ____ _| |
 | |    / _ \| '_ \/ __|/ _ \ / _ \ |  `--. \ | | | '__\ \ / / \ \ / / _` | |
 | \__/\ (_) | | | \__ \ (_) |  __/ | /\__/ / |_| | |   \ V /| |\ V / (_| | |
  \____/\___/|_| |_|___/\___/ \___|_| \____/ \__,_|_|    \_/ |_| \_/ \__,_|_|
                                                                             
    )";

    const int boxWidth = 58;  // 내부 폭

    cout << "\n";
    cout << "        ┌───────────────────── MENU ─────────────────────┐\n";

    for (int i = 0; i < MENU_COUNT; ++i) {
        cout << "        │ ";

        string label = (i == selected ? "> " : "  ") + menuItems[i];

        int width = getDisplayWidth(label);
        int padding = boxWidth - width;

        // 핵심: padding - 3 로 보정
        cout << label << string(padding - 3, ' ') << "│\n";
    }

    cout << "        └────────────────────────────────────────────────┘\n";
    cout << "            ↑↓ 방향키로 이동, Enter로 선택하세요.\n";
}


// 맵 출력 함수
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
    srand(time(0));
    const int width = 32;
    const int height = 16;
    int selected = 0;

    while (true) {
        drawMenu(selected);
        int key = _getch();

        if (key == 224) {  // 방향키 입력 시
            key = _getch();
            if (key == 72 && selected > 0) { // ↑
                --selected;
            }
            else if (key == 80 && selected < MENU_COUNT - 1) { // ↓
                ++selected;
            }
        }
        else if (key == 13) { // Enter 입력 시
            system("cls");
            if (selected == 0) {
                cout << "게임을 시작합니다!\n";
                drawMap(width, height);
                countdownTimer(30);
                string name;
                int score;

                cout << "플레이어 이름을 입력하세요: ";
                cin >> name;

                score = rand() % 100;  // 테스트용 랜덤 점수
                cout << name << "님의 점수는 " << score << "점입니다.\n";

                saveScore(name, score);

                break;
            }
            else if (selected == 1) {
                cout << "[게임 랭킹]\n";
                showRanking();
            }
            else if (selected == 2) {
                printGameInstructions();
            }

            else if (selected == 3) {
                cout << "게임을 종료합니다.\n";
                return 0;
            }
            cout << "\n아무 키나 누르면 메뉴로 돌아갑니다.\n";
            _getch();
        }
    }

    return 0;
}