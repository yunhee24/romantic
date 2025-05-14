#include <iostream>
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

/* 메뉴 출력 함수
void drawMenu(int selected) {
    system("cls");
    cout << "========================\n";
    cout << "      GAME MENU         \n";
    cout << "========================\n";
    for (int i = 0; i < MENU_COUNT; ++i) {
        if (i == selected) {
            cout << " > " << menuItems[i] << endl;
        }
        else {
            cout << "   " << menuItems[i] << endl;
        }
    }
    cout << "\n방향키 ↑↓로 이동, Enter로 선택하세요.\n";
}
*/
int getDisplayWidth(const string& text) {
    int width = 0;
    for (char ch : text) {
        // 한글은 대부분 음수로 표현되는 멀티바이트 첫 바이트
        width += (ch & 0x80) ? 2 : 1;
    }
    return width;
}

void drawMenu(int selected) {
    system("cls");

    // 타이틀 출력 생략 (위 코드 유지)
    cout << "\n\n";
    cout << "           ██████╗ ██████╗ ███╗   ██╗███████╗ ██████╗ ██╗     ███████╗\n";
    cout << "          ██╔════╝██╔═══██╗████╗  ██║██╔════╝██╔═══██╗██║     ██╔════╝\n";
    cout << "          ██║     ██║   ██║██╔██╗ ██║█████╗  ██║   ██║██║     █████╗  \n";
    cout << "          ██║     ██║   ██║██║╚██╗██║██╔══╝  ██║   ██║██║     ██╔══╝  \n";
    cout << "          ╚██████╗╚██████╔╝██║ ╚████║███████╗╚██████╔╝███████╗███████╗\n";
    cout << "           ╚═════╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝ ╚═════╝ ╚══════╝╚══════╝\n";
    cout << "\n";
    cout << "             ███████╗██╗   ██╗██████╗ ██╗   ██╗███████╗██╗     ██╗      \n";
    cout << "             ██╔════╝██║   ██║██╔══██╗██║   ██║██╔════╝██║     ██║      \n";
    cout << "             ███████╗██║   ██║██████╔╝██║   ██║█████╗  ██║     ██║      \n";
    cout << "             ╚════██║██║   ██║██╔═══╝ ██║   ██║██╔══╝  ██║     ██║      \n";
    cout << "             ███████║╚██████╔╝██║     ╚██████╔╝███████╗███████╗███████╗\n";
    cout << "             ╚══════╝ ╚═════╝ ╚═╝      ╚═════╝ ╚══════╝╚══════╝╚══════╝\n";

    cout << "\n";
    cout << "        ┌───────────────────── MENU ─────────────────────┐\n";

    for (int i = 0; i < MENU_COUNT; ++i) {
        cout << "        │ ";
        string label = (i == selected) ? "▶ " + menuItems[i] : "  " + menuItems[i];

        int displayWidth = getDisplayWidth(label);
        int padding = 53 - displayWidth;  // 내부 출력 폭 기준

        cout << label << string(padding, ' ') << "│\n";
    }

    cout << "        └───────────────────────────────────────────────┘\n";
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
                cout << "[게임 방법]\n";
                cout << "- 방향키로 이동하세요.\n- 목표를 향해 나아가세요.\n";
            }
            else if (selected == 3) {
                cout << "게임을 종료합니다.\n";
                return 0;
            }
            cout << "\n아무 키나 누르면 메뉴로 돌아갑니다...\n";
            _getch();
        }
    }

    return 0;
}
