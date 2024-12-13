#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
using namespace std;

// 방향키 정의
enum Direction {
    STOP = 0,
    LEFT = 1,
    RIGHT = 2,
    UP = 3,
    DOWN = 4
};

void clearScreen() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = { 0,0 };
    SetConsoleCursorPosition(hOut, position);
}

void hideCursor() {
    HANDLE consoleHandle =
    GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 180;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);

}

// 게임 기본 클래스
class GameBase {
protected:
    bool gameOver;
    const int width = 20;
    const int height = 20;
public:
    virtual void initialize() = 0;
    virtual void render() = 0;
    virtual void input() = 0;
    virtual void logic() = 0;
};

// 스네이크 게임 클래스
class SnakeGame : public GameBase {
private:
    vector<pair<int, int>> snake;
    pair<int, int> fruit;
    Direction dir;
    int score;

    void spawnFruit() {
        fruit.first = rand() % width;
        fruit.second = rand() % height;
    }

public:
    SnakeGame() {
        gameOver = false;
        dir = STOP;
        score = 0;
        // 커서 숨기기 추가
        hideCursor();
    }

    void initialize() override {
        // 뱀 초기 위치
        snake.push_back({ width / 2, height / 2 });
        spawnFruit();
    }

    void render() override {
        clearScreen();
        // 상단 벽
        for (int i = 0; i < width + 2; i++)
            cout << "#";
        cout << endl;

        // 게임 영역
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (j == 0) cout << "#";

                bool printed = false;
                // 뱀 그리기
                for (auto& body : snake) {
                    if (body.first == j && body.second == i) {
                        cout << "O";
                        printed = true;
                        break;
                    }
                }
                // 과일 그리기
                if (!printed && fruit.first == j && fruit.second == i) {
                    cout << "F";
                    printed = true;
                }
                // 빈 공간
                if (!printed) cout << " ";

                if (j == width - 1) cout << "#";
            }
            cout << endl;
        }

        // 하단 벽
        for (int i = 0; i < width + 2; i++)
            cout << "#";
        cout << endl;
        cout << "Score: " << score << endl;
    }

    void input() override {
        if (_kbhit()) {
            int key = _getch();
            if (key == 224) {  // 방향키
                switch (_getch()) {
                case 72:  // 위
                    if (dir != DOWN) dir = UP;
                    break;
                case 80:  // 아래
                    if (dir != UP) dir = DOWN;
                    break;
                case 75:  // 왼쪽
                    if (dir != RIGHT) dir = LEFT;
                    break;
                case 77:  // 오른쪽
                    if (dir != LEFT) dir = RIGHT;
                    break;
                }
            }
            else if (key == 'x' || key == 'X') {  // x키로 게임 종료
                gameOver = true;
            }
        }
    }


    void logic() override {
        // 이전 위치 저장
        pair<int, int> prev = snake[0];
        pair<int, int> prev2;

        // 머리 이동
        switch (dir) {
        case LEFT:
            snake[0].first--;
            break;
        case RIGHT:
            snake[0].first++;
            break;
        case UP:
            snake[0].second--;
            break;
        case DOWN:
            snake[0].second++;
            break;
        }

        // 몸통 이동
        for (size_t i = 1; i < snake.size(); i++) {
            prev2 = snake[i];
            snake[i] = prev;
            prev = prev2;
        }

        // 과일 먹었는지 체크
        if (snake[0] == fruit) {
            score += 10;
            spawnFruit();
            snake.push_back(prev);
        }

        // 벽 충돌 체크
        if (snake[0].first >= width || snake[0].first < 0 ||
            snake[0].second >= height || snake[0].second < 0)
            gameOver = true;

        // 자기 몸통과 충돌 체크
        for (size_t i = 1; i < snake.size(); i++) {
            if (snake[0] == snake[i])
                gameOver = true;
        }
    }

    bool isGameOver() {
        return gameOver;
    }
};

int main() {
    srand(time(0));
    SnakeGame game;
    game.initialize();

    while (!game.isGameOver()) {
        game.render();
        game.input();
        game.logic();
        Sleep(200); // 게임 속도 조절
    }

    cout << "게임 오버ww" << endl;
    return 0;
}
