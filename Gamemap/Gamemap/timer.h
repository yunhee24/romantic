// timer.h
#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <windows.h>  // Sleep, system
using namespace std;

// ���� �ð��� ȭ�鿡 ����ϰ� 1�ʸ��� ���ҽ�Ŵ
void countdownTimer(int seconds) {
    for (int i = seconds; i >= 0; --i) {
        COORD pos = { 0, 0 };  // �»�� ��ġ
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
        cout << "���� �ð�: " << i << "��   " << endl;  // ���� �߰��� ����� ����
        Sleep(1000);
    }

    system("cls");
    cout << "�ð� �ʰ�! ���� ����\n";
    exit(0);  // ���α׷� ����
}

#endif
