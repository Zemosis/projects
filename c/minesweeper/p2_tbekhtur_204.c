#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Prototype
int commandline_check(int argc, char **argv);
int get_bomb_percent(char *level);
int get_num_bomb(int rows, int cols, int bomb_percent);
char **setup(int rows, int cols);
void init(char **gboard, int rows, int cols, int num_bomb);
int get_adjacent_bombs(char **gboard, int rows, int cols, int current_row,
                       int current_col);
void print_board(char **board, int rows, int cols);
void walker(char **pboard, char **gboard, int rows, int cols, int click_r,
            int click_c);
void play(char **pboard, char **gboard, int rows, int cols, int num_bomb);
void cleanup(char **board, int rows);

int main(int argc, char *argv[]) {
    if (!commandline_check(argc, argv)) {
        return 1;
    }

    int row = atoi(argv[1]);
    int col = atoi(argv[2]);
    char difficulty[10];
    srand(atoi(argv[4]));

    strcpy(difficulty, argv[3]);

    // Bomb num initialize
    int bomb_percent = get_bomb_percent(difficulty);
    int bomb_total = get_num_bomb(row, col, bomb_percent);

    // Two array initialize
    char **game_board = setup(row, col);
    char **play_board = setup(row, col);

    init(game_board, row, col, bomb_total);

    printf("This is the game board after initialization\n");
    print_board(game_board, row, col);
    printf("\nThis is the play board after setup\n");
    print_board(play_board, row, col);

    printf("\nThe game will start\n");
    play(play_board, game_board, row, col, bomb_total);
    cleanup(play_board, row);
    cleanup(game_board, row);
}

int commandline_check(int argc, char **argv) {
    if (argc != 5) {
        printf("Error: Invalid number of arguments\n");
        printf("Usage: %s <rows> <cols> <difficulty> <seed>\n", argv[0]);
        return 0;
    }

    int rows = atoi(argv[1]);
    if (rows <= 0) {
        printf("Error: Invalid rows. Must be positive integer\n");
        return 0;
    }

    int cols = atoi(argv[2]);
    if (cols <= 0) {
        printf("Error: Invalid columns. Must be positive integer\n");
        return 0;
    }

    if (strcmp(argv[3], "easy") != 0 && strcmp(argv[3], "medium") != 0 &&
        strcmp(argv[3], "hard") != 0) {
        printf("Error: Invalid difficulty levels. Must be either easy, medium, "
               "or hard\n");
        return 0;
    }

    return 1;
}

int get_bomb_percent(char *level) {
    if (strcmp(level, "easy") == 0) {
        return 5;
    } else if (strcmp(level, "medium") == 0) {
        return 20;
    } else {
        return 35;
    }
}

int get_num_bomb(int rows, int cols, int bomb_percent) {
    return rows * cols * bomb_percent / 100;
}

char **setup(int rows, int cols) {
    char **board = malloc(rows * sizeof(char *));
    for (int i = 0; i < rows; i++) {
        board[i] = malloc(cols * sizeof(char));
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            board[i][j] = '.';
        }
    }

    return board;
}

void init(char **gboard, int rows, int cols, int num_bomb) {
    int bomb_placed = 0;

    while (bomb_placed < num_bomb) {
        int rand_row = rand() % rows;
        int rand_col = rand() % cols;

        if (gboard[rand_row][rand_col] != '*') {
            gboard[rand_row][rand_col] = '*';
            bomb_placed++;
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (gboard[i][j] != '*') {
                int count = get_adjacent_bombs(gboard, rows, cols, i, j);
                gboard[i][j] = '0' + count;
            }
        }
    }
}

int get_adjacent_bombs(char **gboard, int rows, int cols, int current_row,
                       int current_col) {
    int bomb_count = 0;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) {
                continue;
            }

            int new_row = current_row + i;
            int new_col = current_col + j;

            if (new_row >= 0 && new_row < rows && new_col >= 0 &&
                new_col < cols) {
                if (gboard[new_row][new_col] == '*') {
                    bomb_count++;
                }
            }
        }
    }

    return bomb_count;
}

void print_board(char **board, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

void walker(char **pboard, char **gboard, int rows, int cols, int click_r,
            int click_c) {
    if (pboard[click_r][click_c] != '.') {
        return;
    }

    pboard[click_r][click_c] = gboard[click_r][click_c];

    if (gboard[click_r][click_c] != '0') {
        return;
    }

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) {
                continue;
            }

            int new_row = click_r + i;
            int new_col = click_c + j;

            if (new_row >= 0 && new_row < rows && new_col >= 0 &&
                new_col < cols) {
                if (pboard[new_row][new_col] == '.') {
                    walker(pboard, gboard, rows, cols, new_row, new_col);
                }
            }
        }
    }
}

void play(char **pboard, char **gboard, int rows, int cols, int num_bomb) {
    int total_tiles = rows * cols;
    int tiles_to_reveal = total_tiles - num_bomb;

    while (1) {
        int rand_row = rand() % rows;
        int rand_col = rand() % cols;

        if (gboard[rand_row][rand_col] == '*') {
            printf("Click at (%d, %d). Bomb Exploded! Game Over!\n", rand_row,
                   rand_col);
            pboard[rand_row][rand_col] = '!';
            printf("This is the play board after explosion\n");
            print_board(pboard, rows, cols);
            return;
        } else if (pboard[rand_row][rand_col] == '.') {
            printf("Click at (%d, %d)\n", rand_row, rand_col);
            walker(pboard, gboard, rows, cols, rand_row, rand_col);
            print_board(pboard, rows, cols);
            printf("\n");
        }

        int revealed = 0;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (pboard[i][j] != '.') {
                    revealed++;
                }
            }
        }

        if (revealed >= tiles_to_reveal) {
            printf("Game Completed!\n");
            print_board(pboard, rows, cols);
            return;
        }
    }
}

void cleanup(char **board, int rows) {
    for (int i = 0; i < rows; i++) {
        free(board[i]);
    }
    free(board);
}
