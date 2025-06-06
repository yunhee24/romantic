#include "map.h"
#include "player.h"
#include "monster.h"
#include "attack.h"
#include "Timer.h"   //타이머
#include <thread>
#include <vector>
#include <mutex>

std::vector<Monster> monsters;
extern std::mutex output_mutex;

// 커서 위치 조정
void setCursorPosition(int x, int y) {
    COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

const int MENU_COUNT = 4;
const string menuItems[MENU_COUNT] = {
    "게임 시작",
    "게임 랭킹",
    "게임 방법",
    "게임 종료"
};

struct Scorein {
    string name;
    int score;
};

bool compareByScore(const Scorein& a, const Scorein& b) {
    return a.score > b.score;  // 높은 점수 먼저
}

int getDisplayWidth(const string& text) {
    int width = 0;
    for (unsigned char ch : text) {
        width += (ch & 0x80) ? 2 : 1;
    }
    return width;
}

// 게임 방법 출력
void printGameInstructions() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    cout << "[게임 방법]\n";

    //빨간색 설정
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
    cout << "- 키보드 ↑ ↓ ← → 방향키로 플레이어를 조작할 수 있습니다.\n";

    //흰색(기본색) 복원
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    cout << "- 매 턴마다 랜덤으로 숫자가 부여되고, 해당 숫자만큼 이동할 수 있습니다.\n";
    cout << "- 이동이 끝나면, 마지막으로 움직인 방향으로 자동으로 공격이 나갑니다.\n";
    cout << "- 적의 공격은 **플레이어가 움직였을 때만** 동작합니다.\n";
    cout << "  (즉, 플레이어가 가만히 있으면 적도 공격하지 않습니다!)\n";
    cout << "- 제한된 시간 동안 몬스터를 처치하며 최대한 많은 점수를 획득하세요.\n";
}

// 랭킹 출력
void showRanking() {
    ifstream file("scores.txt");
    vector<Scorein> rankings;

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
/*
저장 실패 처리??

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
*/

// 시작 메뉴 출력
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

// 맵 출력                        
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

// 맵 재출력
void drawMapRe(int width, int height) {
    std::lock_guard<std::mutex> lock(output_mutex);
    int offsetX = 20;
    int offsetY = 3;

    for (int x = 0; x < width; ++x) {
        gotoxy(offsetX + x, offsetY + 0);
        if (x == 0) cout << "┌";
        else if (x == width - 1) cout << "┐";
        else cout << "─";
    }

    for (int x = 0; x < width; ++x) {
        gotoxy(offsetX + x, offsetY + height - 1);
        if (x == 0) cout << "└";
        else if (x == width - 1) cout << "┘";
        else cout << "─";
    }

    for (int y = 1; y < height - 1; ++y) {
        gotoxy(offsetX + 0, offsetY + y);
        cout << "│";
        gotoxy(offsetX + width - 1, offsetY + y);
        cout << "│";
    }
}

// 유저 정보 입력
void User(Player& p) {
    int x = 0, y = 0;

    // 현재 커서 위치 저장
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    x = csbi.dwCursorPosition.X;
    y = csbi.dwCursorPosition.Y;

    // 입력 메시지 출력
    cout << "플레이어 이름을 입력하세요 : ";
    getline(cin, p.name);

    // 입력 메시지 삭제
    setCursorPosition(x, y);
    cout << string(50, ' ');
    setCursorPosition(x, y);
}

// 움직임 횟수 랜덤 부여                 >>> 한 번 정해지면 끝. 계속해서 갱신해야함.
void moveNumber(Player& p) {
    int x = 0, y = 0;

    // 현재 커서 위치 저장
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    x = csbi.dwCursorPosition.X;
    y = csbi.dwCursorPosition.Y;

    srand(static_cast<unsigned int>(std::time(nullptr)));

    cout << "엔터를 눌러주세요.";
    cin.get();

    p.randNumber = std::rand() % 3 + 3;           // Player의 멤버 변수에 저장     1일 때 이상한 오류 생김
    cout << "이동 횟수: " << p.randNumber << "\n";

    // 입력 메시지 삭제
    setCursorPosition(x, y);
    cout << string(60, ' ');
    setCursorPosition(x, y);
}

// 인게임 메인 함수
int ingame() {
    const int width = 28;  // 맵 크기
    const int height = 14;
    int selected = 0;

    while (true) {
        drawMenu(selected);
        int key = _getch();

        if (key == 224) {
            key = _getch();
            if (key == 72 && selected > 0) --selected;
            else if (key == 80 && selected < MENU_COUNT - 1) ++selected;
        }
        else if (key == 13) {
            system("cls");

            if (selected == 0) {
                Player p;
                User(p);
                moveNumber(p);
                drawMap(width, height);

                ULONGLONG start_time = GetTickCount64();
                std::thread timer_thread(TimerThread, start_time, 50);       //게임 플레이 시간

                Monster m;
                std::thread monster_thread(&Monster::MonsterCreate, &m, std::ref(monsters));

                p.move(monsters);

                if (timer_thread.joinable()) timer_thread.join();
                if (monster_thread.joinable()) monster_thread.join();

                if (p.score >= 0) {
                    saveScore(p);
                    gotoxy(0, 4);
                    cout << "점수: " << p.score << endl;
                    //_getch();
                }
                break;
            }
        }
    }
    return 0;
}

void saveScore(const Player& player) {
    ofstream file("scores.txt", ios::app);
    if (file.is_open()) {
        file << player.name << " " << player.score << "\n";
        file.close();
    }
}
