#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <windows.h> // Windows용. Linux의 경우 unistd.h 사용

#define PI 3.14159265
#define SIZE 25
#define SQUARE_SIZE 10
#define DELAY 100

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x * 2; // 가로 간격을 2배로 설정하여 종횡비 보정
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void clearScreen() {
    system("cls");
}

// 회전된 점의 좌표를 계산
void getRotatedPoint(double x, double y, double centerX, double centerY,
    double angle, double* newX, double* newY) {
    double rad = angle * PI / 180.0;
    *newX = centerX + (x - centerX) * cos(rad) - (y - centerY) * sin(rad);
    *newY = centerY + (x - centerX) * sin(rad) + (y - centerY) * cos(rad);
}

// 커서 숨기기
void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

// 점이 다각형 내부에 있는지 확인하는 함수
int isPointInside(double x, double y, double polygon[][2], int vertexCount) {
    int i, j, c = 0;
    for (i = 0, j = vertexCount - 1; i < vertexCount; j = i++) {
        if (((polygon[i][1] > y) != (polygon[j][1] > y)) &&
            (x < (polygon[j][0] - polygon[i][0]) * (y - polygon[i][1]) /
                (polygon[j][1] - polygon[i][1]) + polygon[i][0]))
            c = !c;
    }
    return c;
}

int main() {
    double angle = 0.0;
    double centerX = SIZE / 2.0;
    double centerY = SIZE / 2.0;
    double points[4][2] = {
        {centerX - SQUARE_SIZE / 2, centerY - SQUARE_SIZE / 2},  // 좌상단
        {centerX + SQUARE_SIZE / 2, centerY - SQUARE_SIZE / 2},  // 우상단
        {centerX + SQUARE_SIZE / 2, centerY + SQUARE_SIZE / 2},  // 우하단
        {centerX - SQUARE_SIZE / 2, centerY + SQUARE_SIZE / 2}   // 좌하단
    };

    hideCursor();

    while (1) {
        clearScreen();

        // 회전된 점들의 좌표 계산
        double rotated[4][2];
        for (int i = 0; i < 4; i++) {
            getRotatedPoint(points[i][0], points[i][1], centerX, centerY,
                angle, &rotated[i][0], &rotated[i][1]);
        }

        // 정사각형의 외곽과 내부를 모두 그리기
        for (int y = 0; y < SIZE; y++) {
            for (int x = 0; x < SIZE; x++) {
                if (isPointInside(x, y, rotated, 4)) {
                    gotoxy(x, y);
                    printf("■"); // 채워진 사각형 문자
                }
            }
        }

        angle += 5.0;
        if (angle >= 360.0) angle = 0.0;

        Sleep(DELAY);
    }

    return 0;
}