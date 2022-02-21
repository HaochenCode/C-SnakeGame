#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include "position.h"
#include "input.h"

#define BOARD_LENGTH 50
#define BOARD_HEIGHT 30
#define SNAKE_CHARACTER 'O'
#define FRUIT_CHARACTER '$'
#define WALL_CHARACTER '#'
#define SNAKE_START_LENGTH 3
#define SNAKE_MAX_LENGTH 100
#define MILLISECOND_PER_FRAME 50

using namespace std;

char board[BOARD_HEIGHT][BOARD_LENGTH];
vector<Position> snake;
Input input = Left;
Position fruit_pos;

void initialize_board(void);
void print_board(void);
void clear_board(void);
void initialize_snake(void);
void update_snake(void);
void update_board(void);
bool check_snake_collision(void);
void place_fruit(void);

int main(void) {

    // Set random seed
    srand(time(NULL));

    initialize_board();
    initialize_snake();
    place_fruit();

    for (;;) {
        update_snake();
        update_board();
        print_board();
        if (check_snake_collision()) {
            break;
        }

        if (kbhit()) {
            if (getch() == 0) { // Arrow
                switch (getch()) {
                    case 72: // Up
                        if (input != Down)
                            input = Up;
                        break;

                    case 80: // Down
                        if (input != Up)
                            input = Down;
                        break;

                    case 75: // Left
                        if (input != Right)
                            input = Left;
                        break;

                    case 77: // Right
                        if (input != Left)
                            input = Right;
                        break;
                }
            }
        }

        this_thread::sleep_for(chrono::milliseconds(MILLISECOND_PER_FRAME));

        clear_board();
    }

    return 0;
}

void initialize_board(void) {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_LENGTH; j++) {
            if (i == 0 || i == BOARD_HEIGHT - 1 || j == 0 || j == BOARD_LENGTH - 1) {
                board[i][j] = WALL_CHARACTER;
                continue;
            }
                
            board[i][j] = ' ';
        }
    }
}

void print_board(void) {
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_LENGTH; j++) {
            cout << board[i][j];
        }
        cout << endl;
    }
}

void clear_board(void) {
    system("cls");
}

void initialize_snake(void) {
    int start_x = BOARD_LENGTH / 2;
    int start_y = BOARD_HEIGHT / 2;
    for (int i = 0; i < SNAKE_START_LENGTH; i++, start_x++) {
        snake.push_back(Position { start_x, start_y });
    }
}

void update_board(void) {
    // Set board and walls
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_LENGTH; j++) {
            // Wall
            if (i == 0 || i == BOARD_HEIGHT - 1 || j == 0 || j == BOARD_LENGTH - 1) {
                board[i][j] = WALL_CHARACTER;
                continue;
            }

            // Fruit
            if (i == fruit_pos.y && j == fruit_pos.x) {
                board[i][j] = FRUIT_CHARACTER;
                continue;
            }
                
            board[i][j] = ' ';
        }
    }
    // Set snake
    for (int i = 0; i < snake.size(); i++) {
        Position* snake_ptr = &snake.at(i);
        board[snake_ptr->y][snake_ptr->x] = SNAKE_CHARACTER;
    }
}

void update_snake(void) {
    int x_increase = 0, y_increase = 0;
    switch (input) {
        case Up:
            y_increase = -1;
            break;

        case Down:
            y_increase = 1;
            break;

        case Right:
            x_increase = 1;
            break;

        default: // Left
            x_increase = -1;
            break;
    }

    Position previous_position;
    for (int i = 0; i < snake.size(); i++) {
        // First body of the snake
        if (i == 0) {
            Position* snake_ptr = &snake.at(i);
            previous_position.x = snake_ptr->x;
            previous_position.y = snake_ptr->y;
            snake_ptr->x += x_increase;
            snake_ptr->y += y_increase;
            continue;
        }
        // Other bodies
        Position* snake_ptr = &snake.at(i);
        Position temp = *snake_ptr;
        snake_ptr->x = previous_position.x;
        snake_ptr->y = previous_position.y;
        previous_position.x = temp.x;
        previous_position.y = temp.y;
    }
}

bool check_snake_collision(void) {
    // Check wall collision
    Position* head = &snake.at(0);
    if (head->x <= 0 || head->x >= BOARD_LENGTH - 1) {
        return true;
    }
    if (head->y <= 0 || head->y >= BOARD_HEIGHT - 1) {
        return true;
    }

    // Check self collision
    for (int i = 1; i < snake.size(); i++) {
        Position* body = &snake.at(i);
        if (head->x == body->x && head->y == body->y) {
            return true;
        }
    }

    // Fruit collision
    if (head->x == fruit_pos.x && head->y == fruit_pos.y) {
        Position newBody;
        snake.push_back(newBody);
        place_fruit();
    }

    return false;
}

void place_fruit(void) {
    fruit_pos.x = rand() % (BOARD_LENGTH - 1) + 1;
    fruit_pos.y = rand() % (BOARD_HEIGHT - 1) + 1;
}