#include "map.h"

const int MENU_COUNT = 4;
const string menuItems[MENU_COUNT] = {
    "���� ����",
    "���� ��ŷ",
    "���� ���",
    "���� ����"
};

// ���� �޴� ���
void drawMenu(int selected) {
    system("cls");

    cout << "��������������������������������������������������������������������������������������������\n";
    cout << "��                                            ��\n";
    cout << "��      ######   ###   #     #  #####         ��\n";
    cout << "��      #       #   #  ##   ##  #             ��\n";
    cout << "��      #  ###  #####  # # # #  #####         ��\n";
    cout << "��      #   ##  #   #  #  #  #  #             ��\n";
    cout << "��      ######  #   #  #     #  #####         ��\n";
    cout << "��                                            ��\n";
    cout << "��      #     #  #####  #    #  #####         ��\n";
    cout << "��      ##   ##  #      ##   #  #             ��\n";
    cout << "��      # # # #  #####  # #  #  #####         ��\n";
    cout << "��      #  #  #  #      #  # #  #             ��\n";
    cout << "��      #     #  #####  #    #  #####         ��\n";
    cout << "��                                            ��\n";
    cout << "���������������������������������� GAME MENU ������������������������������������\n";

    for (int i = 0; i < MENU_COUNT; ++i) {
        cout << "�� ";
        if (i == selected) {
            cout << ">> " << menuItems[i] << string(40 - menuItems[i].length(), ' ') << "��\n";
        }
        else {
            cout << "   " << menuItems[i] << string(40 - menuItems[i].length(), ' ') << "��\n";
        }
    }

    cout << "��������������������������������������������������������������������������������������������\n";
    cout << "  ��� ����Ű�� �̵�, Enter�� �����ϼ���.\n";
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

int ingame() {
    const int width = 32;
    const int height = 16;
    int selected = 0;

    while (true) {
        drawMenu(selected);
        int key = _getch();

        if (key == 224) {  // ����Ű �Է� ��
            key = _getch();
            if (key == 72 && selected > 0) { // ��
                --selected;
            }
            else if (key == 80 && selected < MENU_COUNT - 1) { // ��
                ++selected;
            }
        }
        else if (key == 13) { // Enter �Է� ��
            system("cls");
            if (selected == 0) {
                cout << "\n";
                drawMap(width, height);
                break;
            }
            else if (selected == 1) {
                cout << "[���� ��ŷ] ����� ���� �������� �ʾҽ��ϴ�.\n";
            }
            else if (selected == 2) {
                cout << "[���� ���]\n";
                cout << "- ����Ű�� �̵��ϼ���.\n- ��ǥ�� ���� ���ư�����.\n";
            }
            else if (selected == 3) {
                cout << "������ �����մϴ�.\n";
                return 0;
            }
            cout << "\n�ƹ� Ű�� ������ �޴��� ���ư��ϴ�...\n";
            _getch();
        }
    }
    return 0;
}