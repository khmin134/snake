#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include "../inc/console.h"
#include "../inc/def.h"

enum class BoardStatus {
    NONE, APPLE, SNAKE_HEAD, SNAKE_BODY,
    WALL_V, WALL_H, WALL_RT, WALL_LT, WALL_RB, WALL_LB
};

struct Snake {
    int x, y;
};

struct Apple {
    int pos;
};

int to_index(int x, int y);
void update(BoardStatus* board, Snake* snake, int size, int socre, bool apple);

void move_snake(Snake* body, int size) {
    for(int i = 1; i < size; i++) {
        body[i-1] = body[i];
    }
}

int main() {
    //init
START:
    BoardStatus board[BOARD_SIZE*BOARD_SIZE] { BoardStatus::NONE };
    Snake snake[BOARD_SIZE*BOARD_SIZE] { {BOARD_SIZE/2, BOARD_SIZE/2} };
    console::Key k = console::Key::K_UP;
    int snakeSize = 1, score = 0;
    bool game = true;

    console::init();
    update(board, snake, snakeSize, score, true);

    while(game) {
        std::clock_t start = std::clock(), end;
        int headX = snake[snakeSize-1].x;
        int headY = snake[snakeSize-1].y;
        bool apple = false;
 
        //enum Key { K_NONE = 0, K_OTHER, K_LEFT, K_RIGHT, K_UP, K_DOWN, K_ESC, K_ENTER };
        if(console::key(console::Key::K_LEFT)
                && board[to_index(headX-1, headY)] != BoardStatus::SNAKE_BODY)
            k = console::Key::K_LEFT;
        else if(console::key(console::Key::K_RIGHT)
                && board[to_index(headX+1, headY)] != BoardStatus::SNAKE_BODY)
            k = console::Key::K_RIGHT;
        else if(console::key(console::Key::K_UP)
                && board[to_index(headX, headY-1)] != BoardStatus::SNAKE_BODY)
            k = console::Key::K_UP;
        else if(console::key(console::Key::K_DOWN)
                && board[to_index(headX, headY+1)] != BoardStatus::SNAKE_BODY)
            k = console::Key::K_DOWN;
        else if(console::key(console::Key::K_ESC))
            return 0;

       switch(k) {
        case console::Key::K_LEFT:
            if(headX > 1)
                switch(board[to_index(headX-1, headY)]) {
                case BoardStatus::NONE:
                    snake[snakeSize-1].x = headX-1;
                    snake[snakeSize-1].y = headY;
                    move_snake(snake, snakeSize);
                    break;

                case BoardStatus::APPLE:
                    snake[snakeSize].x = headX-1;
                    snake[snakeSize].y = headY;
                    snakeSize++;
                    apple = true;
                    score += 10;
                    break;
                
                default:
                    game = false;
                    break;
                }
            
            break;

        case console::Key::K_RIGHT:
            if(headX < BOARD_SIZE-1)
                switch(board[to_index(headX+1, headY)]) {
                case BoardStatus::NONE:
                    snake[snakeSize-1].x = headX+1;
                    snake[snakeSize-1].y = headY;
                    move_snake(snake, snakeSize);
                    break;

                case BoardStatus::APPLE:
                    snake[snakeSize].x = headX+1;
                    snake[snakeSize].y = headY;
                    snakeSize++;
                    apple = true;
                    score += 10;
                    break;
                
                default:
                    game = false;
                    break;
                }
            
            break;

        case console::Key::K_UP:
            if(headY > 1)
                switch(board[to_index(headX, headY-1)]) {
                case BoardStatus::NONE:
                    snake[snakeSize-1].x = headX;
                    snake[snakeSize-1].y = headY-1;
                    move_snake(snake, snakeSize);
                    break;

                case BoardStatus::APPLE:
                    snake[snakeSize].x = headX;
                    snake[snakeSize].y = headY-1;
                    snakeSize++;
                    apple = true;
                    score += 10;
                    break;
                
                default:
                    game = false;
                    break;
                }
            
            break;

        case console::Key::K_DOWN:
            if(headX < BOARD_SIZE-1)
                switch(board[to_index(headX, headY+1)]) {
                case BoardStatus::NONE:
                    snake[snakeSize-1].x = headX;
                    snake[snakeSize-1].y = headY+1;
                    move_snake(snake, snakeSize);
                    break;

                case BoardStatus::APPLE:
                    snake[snakeSize].x = headX;
                    snake[snakeSize].y = headY+1;
                    snakeSize++;
                    apple = true;
                    score += 10;
                    break;
                
                default:
                    game = false;
                    break;
                }
            
            break;

        default:
            break;
        }

        do {
            end = std::clock();
        } while(end-start < 1.0 / (60 / MOVE_DELAY) * CLOCKS_PER_SEC);

        update(board, snake, snakeSize, score, apple);
    }
    
    const char* youLose = "YOU LOSE!";
    const char* tryAgain = "TRY AGAIN? (ENTER)";
    console::draw((BOARD_SIZE-std::strlen(youLose))/2, BOARD_SIZE/2, youLose);
    console::draw((BOARD_SIZE-std::strlen(tryAgain))/2, BOARD_SIZE/2+1, tryAgain);
    console::wait();
    while(!console::key(console::Key::K_ENTER));
    goto START;
}

int to_index(int x, int y) {
    return x + BOARD_SIZE * y;
}

void update(BoardStatus* board, Snake* snake, int size, int score, bool apple) {
    console::clear();

    for(int i = 0; i < BOARD_SIZE*BOARD_SIZE; i++)
        board[i] = BoardStatus::NONE;

    //board
    for(int x = 0, y = 0; x < BOARD_SIZE; x++)
        board[to_index(x, y)] = BoardStatus::WALL_H;
    for(int x = 0, y = BOARD_SIZE-1; x < BOARD_SIZE; x++)
        board[to_index(x, y)] = BoardStatus::WALL_H;
    for(int x = 0, y = 0; y < BOARD_SIZE; y++)
        board[to_index(x, y)] = BoardStatus::WALL_V;
    for(int x = BOARD_SIZE-1, y = 0; y < BOARD_SIZE; y++)
        board[to_index(x, y)] = BoardStatus::WALL_V;

    board[to_index(0, 0)] = BoardStatus::WALL_LT;
    board[to_index(0, BOARD_SIZE-1)] = BoardStatus::WALL_LB;
    board[to_index(BOARD_SIZE-1, 0)] = BoardStatus::WALL_RT;
    board[to_index(BOARD_SIZE-1, BOARD_SIZE-1)] = BoardStatus::WALL_RB;

    //apple
    static Apple _apple;
    if(apple) {
        std::srand(std::time(nullptr));
        
        while(true) {
            int pos = rand() % (BOARD_SIZE*BOARD_SIZE);

            if(board[pos] == BoardStatus::NONE) {
                board[pos] = BoardStatus::APPLE;
                _apple.pos = pos;
                break;
            }
        }
    } else {
        board[_apple.pos] = BoardStatus::APPLE;
    }
  
    //snake
    for(int i = 0; i < size-1; i++)
        board[to_index(snake[i].x, snake[i].y)] = BoardStatus::SNAKE_BODY;
    
    board[to_index(snake[0].x, snake[0].y)] = BoardStatus::SNAKE_HEAD;

    //draw
    for(int x = 0; x < BOARD_SIZE; x++) {
        for(int y = 0; y < BOARD_SIZE; y++) {
            switch (board[to_index(x, y)]) {
            case BoardStatus::NONE:
                console::draw(x, y, " "); break;
            case BoardStatus::APPLE:
                console::draw(x, y, APPLE_STRING); break;
            case BoardStatus::SNAKE_HEAD:
                console::draw(x, y, SNAKE_STRING); break;
            case BoardStatus::SNAKE_BODY:
                console::draw(x, y, SNAKE_BODY_STRING); break;
            case BoardStatus::WALL_V:
                console::draw(x, y, WALL_VERTICAL_STRING); break;
            case BoardStatus::WALL_H:
                console::draw(x, y, WALL_HORIZONTAL_STRING); break;
            case BoardStatus::WALL_LT:
                console::draw(x, y, WALL_LEFT_TOP_STRING); break;
            case BoardStatus::WALL_LB:
                console::draw(x, y, WALL_LEFT_BOTTOM_STRING); break;
            case BoardStatus::WALL_RT:
                console::draw(x, y, WALL_RIGHT_TOP_STRING); break;
            case BoardStatus::WALL_RB:
                console::draw(x, y, WALL_RIGHT_BOTTOM_STRING); break;
            }
        }
    }

    //score
    char scoreStr[100];
    std::sprintf(scoreStr, "Score: %d", score);
    console::draw((BOARD_SIZE-std::strlen(scoreStr))/2, BOARD_SIZE+1, scoreStr);

    console::wait();
}
