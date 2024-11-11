#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <windows.h>
#define PI 3.14159265
#define SIZE 40
#define CUBE_SIZE 10
#define DELAY 50

typedef struct {
    double x, y, z;
} Point3D;

typedef struct {
    double x, y;
} Point2D;

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x * 2;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void clearScreen() {
    system("cls");
}

void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

// 3D 회전 변환
void rotate3D(Point3D* p, double angleX, double angleY, double angleZ) {
    double x = p->x, y = p->y, z = p->z;
    double radX = angleX * PI / 180.0;
    double radY = angleY * PI / 180.0;
    double radZ = angleZ * PI / 180.0;
    // X축 회전
    double tempY = y;
    y = y * cos(radX) - z * sin(radX);
    z = tempY * sin(radX) + z * cos(radX);
    // Y축 회전
    double tempX = x;
    x = x * cos(radY) + z * sin(radY);
    z = -tempX * sin(radY) + z * cos(radY);
    // Z축 회전
    tempX = x;
    x = x * cos(radZ) - y * sin(radZ);
    y = tempX * sin(radZ) + y * cos(radZ);
    p->x = x;
    p->y = y;
    p->z = z;
}

// 3D to 2D 투영
Point2D project(Point3D p3d, double distance) {
    Point2D p2d;
    double factor = distance / (distance + p3d.z + 20);
    p2d.x = p3d.x * factor + SIZE / 2;
    p2d.y = p3d.y * factor + SIZE / 2;
    return p2d;
}

// 선 그리기
void drawLine(Point2D p1, Point2D p2, char buffer[SIZE][SIZE]) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    double steps = fmax(fabs(dx), fabs(dy));
    double xinc = dx / steps;
    double yinc = dy / steps;
    double x = p1.x;
    double y = p1.y;
    for (int i = 0; i <= steps; i++) {
        int ix = (int)round(x);
        int iy = (int)round(y);
        if (ix >= 0 && ix < SIZE && iy >= 0 && iy < SIZE) {
            buffer[iy][ix] = '*';
        }
        x += xinc;
        y += yinc;
    }
}

int main() {
    hideCursor();
    double angleX = 0, angleY = 0, angleZ = 0;
    char buffer[SIZE][SIZE];
    Point3D cube[8];
    int edges[12][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0},  // 앞면
        {4, 5}, {5, 6}, {6, 7}, {7, 4},  // 뒷면
        {0, 4}, {1, 5}, {2, 6}, {3, 7}   // 연결선
    };

    // 큐브의 꼭짓점 초기화
    for (int i = 0; i < 8; i++) {
        cube[i].x = ((i & 1) ? CUBE_SIZE : -CUBE_SIZE);
        cube[i].y = ((i & 2) ? CUBE_SIZE : -CUBE_SIZE);
        cube[i].z = ((i & 4) ? CUBE_SIZE : -CUBE_SIZE);
    }

    while (1) {
        // 버퍼 초기화
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                buffer[i][j] = ' ';

        // 각 꼭짓점 회전 및 투영
        Point3D rotated[8];
        Point2D projected[8];
        for (int i = 0; i < 8; i++) {
            rotated[i] = cube[i];
            rotate3D(&rotated[i], angleX, angleY, angleZ);
            projected[i] = project(rotated[i], 30);
        }

        // 모서리 그리기
        for (int i = 0; i < 12; i++) {
            drawLine(projected[edges[i][0]], projected[edges[i][1]], buffer);
        }

        // 화면 출력
        clearScreen();
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                gotoxy(j, i);
                printf("%c", buffer[i][j]);
            }
            printf("\n");
        }

        // 회전 각도 업데이트
        angleX += 2; // X축 회전
        angleY += 2; // Y축 회전
        angleZ += 2; // Z축 회전
        if (angleX >= 360) angleX -= 360;
        if (angleY >= 360) angleY -= 360;
        if (angleZ >= 360) angleZ -= 360;
        Sleep(DELAY);
    }
    return 0;
}