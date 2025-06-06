#include "map.h"
#include "player.h"
#include "monster.h"
#include "attack.h"
#include "Timer.h"   //Ÿ�̸�
#include <thread>
#include <vector>
#include <mutex>

std::vector<Monster> monsters;
extern std::mutex output_mutex;

// Ŀ�� ��ġ ����
void setCursorPosition(int x, int y) {
    COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

const int MENU_COUNT = 4;
const string menuItems[MENU_COUNT] = {
    "���� ����",
    "���� ��ŷ",
    "���� ���",
    "���� ����"
};

struct Scorein {
    string name;
    int score;
};

bool compareByScore(const Scorein& a, const Scorein& b) {
    return a.score > b.score;  // ���� ���� ����
}

int getDisplayWidth(const string& text) {
    int width = 0;
    for (unsigned char ch : text) {
        width += (ch & 0x80) ? 2 : 1;
    }
    return width;
}

// ���� ��� ���
void printGameInstructions() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    cout << "[���� ���]\n";

    //������ ����
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
    cout << "- Ű���� �� �� �� �� ����Ű�� �÷��̾ ������ �� �ֽ��ϴ�.\n";

    //���(�⺻��) ����
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    cout << "- �� �ϸ��� �������� ���ڰ� �ο��ǰ�, �ش� ���ڸ�ŭ �̵��� �� �ֽ��ϴ�.\n";
    cout << "- �̵��� ������, ���������� ������ �������� �ڵ����� ������ �����ϴ�.\n";
    cout << "- ���� ������ **�÷��̾ �������� ����** �����մϴ�.\n";
    cout << "  (��, �÷��̾ ������ ������ ���� �������� �ʽ��ϴ�!)\n";
    cout << "- ���ѵ� �ð� ���� ���͸� óġ�ϸ� �ִ��� ���� ������ ȹ���ϼ���.\n";
}

// ��ŷ ���
void showRanking() {
    ifstream file("scores.txt");
    vector<Scorein> rankings;

    if (!file) {
        cout << "��ŷ ������ �����ϴ�.\n";
        return;
    }

    string name;
    int score;
    while (file >> name >> score) {
        rankings.push_back({ name, score });
    }
    file.close();

    sort(rankings.begin(), rankings.end(), compareByScore);

    cout << "===== ���� ��ŷ =====\n";
    for (size_t i = 0; i < rankings.size(); ++i) {
        cout << i + 1 << ". " << rankings[i].name << " - " << rankings[i].score << endl;
    }
}
/*
���� ���� ó��??

void saveScore(const string& name, int score) {
    ofstream file("scores.txt", ios::app);  // append ���
    if (file) {
        file << name << " " << score << endl;
        file.close();
    }
    else {
        cout << "��ŷ ���� ����!\n";
    }
}
*/

// ���� �޴� ���
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

    const int boxWidth = 58;  // ���� ��

    cout << "\n";
    cout << "        �������������������������������������������� MENU ��������������������������������������������\n";

    for (int i = 0; i < MENU_COUNT; ++i) {
        cout << "        �� ";

        string label = (i == selected ? "> " : "  ") + menuItems[i];

        int width = getDisplayWidth(label);
        int padding = boxWidth - width;

        // �ٽ�: padding - 3 �� ����
        cout << label << string(padding - 3, ' ') << "��\n";
    }

    cout << "        ����������������������������������������������������������������������������������������������������\n";
    cout << "            ��� ����Ű�� �̵�, Enter�� �����ϼ���.\n";
}

// �� ���                        
void drawMap(int width, int height) {
    int offsetX = 20; // ���� ��ġ ���� (����)
    int offsetY = 3;  // ���� ��ġ ���� (�ٹٲ�)

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

// �� �����
void drawMapRe(int width, int height) {
    std::lock_guard<std::mutex> lock(output_mutex);
    int offsetX = 20;
    int offsetY = 3;

    for (int x = 0; x < width; ++x) {
        gotoxy(offsetX + x, offsetY + 0);
        if (x == 0) cout << "��";
        else if (x == width - 1) cout << "��";
        else cout << "��";
    }

    for (int x = 0; x < width; ++x) {
        gotoxy(offsetX + x, offsetY + height - 1);
        if (x == 0) cout << "��";
        else if (x == width - 1) cout << "��";
        else cout << "��";
    }

    for (int y = 1; y < height - 1; ++y) {
        gotoxy(offsetX + 0, offsetY + y);
        cout << "��";
        gotoxy(offsetX + width - 1, offsetY + y);
        cout << "��";
    }
}

// ���� ���� �Է�
void User(Player& p) {
    int x = 0, y = 0;

    // ���� Ŀ�� ��ġ ����
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    x = csbi.dwCursorPosition.X;
    y = csbi.dwCursorPosition.Y;

    // �Է� �޽��� ���
    cout << "�÷��̾� �̸��� �Է��ϼ��� : ";
    getline(cin, p.name);

    // �Է� �޽��� ����
    setCursorPosition(x, y);
    cout << string(50, ' ');
    setCursorPosition(x, y);
}

// ������ Ƚ�� ���� �ο�                 >>> �� �� �������� ��.   ����ؼ� �����ؾ���.
void moveNumber(Player& p) {
    int x = 0, y = 0;

    // ���� Ŀ�� ��ġ ����
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    x = csbi.dwCursorPosition.X;
    y = csbi.dwCursorPosition.Y;

    srand(static_cast<unsigned int>(std::time(nullptr)));

    cout << "���͸� �����ּ���.";
    cin.get();

    p.randNumber = std::rand() % 4 + 2;           // Player�� ��� ������ ����     1�� �� �̻��� ���� ����
    cout << "���� ����: " << p.randNumber << "\n";

    // �Է� �޽��� ����
    setCursorPosition(x, y);
    cout << string(60, ' ');
    setCursorPosition(x, y);
}

// �ΰ��� ���� �Լ�
int ingame() {
    const int width = 28;  // �� ũ�� 28x14�� ��������
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
                std::thread timer_thread(TimerThread, start_time, 50);       //���� �÷��� �ð�

                Monster m;
                std::thread monster_thread(&Monster::MonsterCreate, &m, std::ref(monsters));

                p.move(monsters);

                if (timer_thread.joinable()) timer_thread.join();
                if (monster_thread.joinable()) monster_thread.join();


                if (p.score >= 0) {
                    saveScore(p);
                    gotoxy(0, 4);
                    cout << "����: " << p.score << endl;
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