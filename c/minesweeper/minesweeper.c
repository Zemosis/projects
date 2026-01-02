#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct GameState {
        int rows;
        int cols;
        int num_bombs;
        char **game_board;
        char **play_board;
} GameState;

// Prototype
int commandline_check(int argc, char **argv);
int get_bomb_percent(char *level);
int get_num_bomb(int rows, int cols, int bomb_percent);
char **setup(int rows, int cols);
void init(GameState *game);
int get_adjacent_bombs(char **gboard, int rows, int cols, int current_row,
                       int current_col);
void print_board(char **board, int rows, int cols);
void walker(char **pboard, char **gboard, int rows, int cols, int click_r,
            int click_c);
void play(GameState *game);
void cleanup(char **board, int rows);
int parse_int(const char *str);

int main(int argc, char *argv[]) {
    if (!commandline_check(argc, argv)) {
        return 1;
    }

    GameState game;

    game.rows = atoi(argv[1]);
    game.cols = atoi(argv[2]);

    char difficulty[10];
    srand(atoi(argv[4]));
    strcpy(difficulty, argv[3]);

    // Bomb num initialize
    int bomb_percent = get_bomb_percent(difficulty);
    game.num_bombs = get_num_bomb(game.rows, game.cols, bomb_percent);

    // Two array initialize
    game.game_board = setup(game.rows, game.cols);

    if (game.game_board == NULL) {
        printf("Error: Memory allocation failed for game board\n");
        return 1;
    }

    game.play_board = setup(game.rows, game.cols);

    init(&game);

    if (game.play_board == NULL) {
        printf("Error: Memory allocation failed for play board\n");
        cleanup(game.game_board, game.rows);
        return 1;
    }

    printf("This is the game board after initialization\n");
    print_board(game.game_board, game.rows, game.cols);
    printf("\nThis is the play board after setup\n");
    print_board(game.play_board, game.rows, game.cols);

    printf("\nThe game will start\n");
    play(&game);
    cleanup(game.play_board, game.rows);
    cleanup(game.game_board, game.rows);

    return 0;
}

int commandline_check(int argc, char **argv) {
    if (argc != 5) {
        printf("Error: Invalid number of arguments\n");
        printf("Usage: %s <rows> <cols> <difficulty> <seed>\n", argv[0]);
        return 0;
    }

    int rows = parse_int(argv[1]);
    if (rows <= 0) {
        printf("Error: Invalid rows. Must be positive integer\n");
        return 0;
    }

    int cols = parse_int(argv[2]);
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
    if (board == NULL) {
        return NULL;
    }

    for (int i = 0; i < rows; i++) {
        board[i] = malloc(cols * sizeof(char));
        if (board[i] == NULL) {
            // Clean up previously allocated rows
            for (int j = 0; j < i; j++) {
                free(board[j]);
            }
            free(board);
            return NULL;
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            board[i][j] = '.';
        }
    }

    return board;
}

void init(GameState *game) {
    int bomb_placed = 0;

    while (bomb_placed < game->num_bombs) {
        int rand_row = rand() % game->rows;
        int rand_col = rand() % game->cols;

        if (game->game_board[rand_row][rand_col] != '*') {
            game->game_board[rand_row][rand_col] = '*';
            bomb_placed++;
        }
    }

    for (int i = 0; i < game->rows; i++) {
        for (int j = 0; j < game->cols; j++) {
            if (game->game_board[i][j] != '*') {
                int count = get_adjacent_bombs(game->game_board, game->rows,
                                               game->cols, i, j);
                game->game_board[i][j] = '0' + count;
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
    printf("   ");
    for (int j = 0; j < cols; j++) {
        printf("%2d ", j);
    }
    printf("\n");

    printf("   ");
    for (int j = 0; j < cols; j++) {
        printf("---");
    }
    printf("\n");

    for (int i = 0; i < rows; i++) {
        printf("%2d|", i);
        for (int j = 0; j < cols; j++) {
            printf(" %c ", board[i][j]);
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

void play(GameState *game) {
    int total_tiles = game->rows * game->cols;
    int tiles_to_reveal = total_tiles - game->num_bombs;
    int r, c;
    char buffer[100];

    while (1) {
        printf("Enter command (e.g. '1 2' to reveal or 'f 1 2' to flag): ");

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("\nInput stream ended. Exiting.\n");
            return;
        }

        char action = 'r';

        if (buffer[0] == 'f' || buffer[0] == 'F') {
            action = 'f';
            // Flag command parse
            if (sscanf(buffer + 1, " %d %d", &r, &c) != 2) {
                printf("Invalid input. Use 'f row col' format.\n");
                continue;
            }
        } else {
            // Standar reveal command parse
            if (sscanf(buffer, " %d %d", &r, &c) != 2) {
                printf("Invalid input. Please enter two numbers.\n");
                continue;
            }
        }

        // Bounds Check
        if (r < 0 || r >= game->rows || c < 0 || c >= game->cols) {
            printf("Coordinates out of bounds (0-%d, 0-%d). Try again.\n",
                   game->rows - 1, game->cols - 1);
            continue;
        }

        // Logic for Flagging
        if (action == 'f') {
            if (game->play_board[r][c] == '.') {
                game->play_board[r][c] = 'F';
            } else if (game->play_board[r][c] == 'F') {
                // Toggle off
                game->play_board[r][c] = '.';
            } else {
                printf("Cannot toggle a revealed tile.\n");
            }
            print_board(game->play_board, game->rows, game->cols);
            continue;
        }

        // Logic for revealing
        if (game->play_board[r][c] == 'F') {
            printf("Tile is flagged! Unflag it first to reveal.\n");
            continue;
        }

        if (game->play_board[r][c] != '.') {
            printf("Tile already revealed. Pick another.\n");
            continue;
        }

        if (game->game_board[r][c] == '*') {
            printf("Hit mine at (%d, %d)! Game Over!\n", r, c);
            game->play_board[r][c] = '!';
            print_board(game->play_board, game->rows, game->cols);
            return;
        }

        printf("Revealing (%d, %d)\n", r, c);
        walker(game->play_board, game->game_board, game->rows, game->cols, r,
               c);
        print_board(game->play_board, game->rows, game->cols);
        printf("\n");

        int revealed = 0;
        for (int i = 0; i < game->rows; i++) {
            for (int j = 0; j < game->cols; j++) {
                if (game->play_board[i][j] != '.' &&
                    game->play_board[i][j] != 'F') {
                    revealed++;
                }
            }
        }

        if (revealed >= tiles_to_reveal) {
            printf("Congratulations! You cleared the minefield!\n");
            print_board(game->play_board, game->rows, game->cols);
            return;
        }
    }
}

void cleanup(char **board, int rows) {
    if (board == NULL) {
        return;
    }
    for (int i = 0; i < rows; i++) {
        free(board[i]);
    }
    free(board);
}

int parse_int(const char *str) {
    char *endptr;
    int val = strtol(str, &endptr, 10);

    if (str == endptr || *endptr != '\0') {
        return -1;
    }

    return val;
}
