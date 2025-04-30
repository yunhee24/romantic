#include <iostream>
#include <conio.h>  // _getch() 함수 사용
using namespace std;
int x;

// 맵 출력 함수
void drawMap(int width, int height) {
    for (int y = 0; y < height; ++y) {
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
}

int main() {
    const int width = 32;
    const int height = 16;

    // 게임 스타트 화면
    cout << "=====================\n";
    cout << "   GAME START!    \n";
    cout << "=====================\n";
    cout << "아무 키나 누르면 시작합니다.\n";

    _getch();  // 키 입력 대기

    system("cls");  // 콘솔 화면 지우기 (Windows 전용)

    // 맵 출력
    drawMap(width, height);

    return 0;
}
