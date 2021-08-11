#include <stdio.h>

#define N 25
#define M 80

void simulation();
int menu(int grid[N][M]);
void print_choices();
int file_input(char a[20], int grid[N][M]);
int stdin_input(int grid[N][M]);
int check_linebreak(char symbol, int pos);
int check_bit(char symbol, int pos);
void compute_next_gen(int curr_generation_grid[N][M], int next_generation_grid[N][M]);
int count_neibhors(int grid[N][M], int x, int y);
int gens_equal(int gen1[N][M], int gen2[N][M]);
void shift_gens(int curr[N][M], int next[N][M]);
void print_gen(int grid[N][M]);

int main() {
    simulation();
    return 0;
}

void simulation() {
    int curr_generation_grid[N][M];
    int error = menu(curr_generation_grid);
    if (!error) {
        int stop = 0;
        while (!stop) {
            char key = getchar();
            if (key == ' ') {
                int next_generation_grid[N][M];
                compute_next_gen(curr_generation_grid, next_generation_grid);
                stop = gens_equal(curr_generation_grid, next_generation_grid);
                print_gen(next_generation_grid);
                shift_gens(curr_generation_grid, next_generation_grid);
            } else if (key == '\n') {
                print_gen(curr_generation_grid);
            } else if (key == 'q' || key == 'Q' || key == 27) {
                break;
            }
        }
        printf("Game has been ended.\n");
    } else if (error == -1) {
        printf("Application has been closed.\n");
    } else {
        printf("Wrong input!\n");
    }
}

int menu(int grid[N][M]) {
    int choice, error = 0;
    char confirm;

    print_choices();
    if (scanf("%d%c", &choice, &confirm) == 2 && confirm == '\n') {
        if (choice > 0 && choice < 8) {
            char filenames[][25] = {
                "static.txt", "stilllifetagalong.txt", "glider_gun.txt",
                "reaper.txt", "sombreros.txt", "light_speed.txt", "train.txt"
            };
            error = !file_input(filenames[choice - 1], grid);
        } else if (choice == 8) {
            error = !stdin_input(grid);
        } else if (choice == 9) {
            error = -1;
        } else {
            error = 1;
        }
    } else {
        error = 1;
    }
    return error;
}

void print_choices() {
    printf("\033[0d\033[2J");
    printf("Enter option:\n");
    printf("1. Static figures.\n");
    printf("2. Tagalong.\n");
    printf("3. Glider gun.\n");
    printf("4. Reaper.\n");
    printf("5. Sombreros.\n");
    printf("6. Light speed.\n");
    printf("7. Train.\n");
    printf("8. Manual input.\n\n");
    printf("9. Exit \"Game of Life\"\n");
}

int file_input(char a[20], int grid[N][M]) {
    FILE *input = fopen(a, "r");
    int res = 1;
    // заполнение массива состоянием клеток с файла
    char current;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j <= M; j++) {
            current = fgetc(input);
            if (feof(input) || !(check_bit(current, j) || check_linebreak(current, j))) {
                res = 0;
                break;
            } else {
                if (j < M) {
                    grid[i][j] = current - '0';
                }
            }
        }
        if (!res) {
            break;
        }
    }
    if (res) {
        print_gen(grid);
    }
    fclose(input);
    return res;
}

int check_linebreak(char symbol, int pos) {
    return (pos == M && (symbol == '\n' || symbol == -1));
}

int check_bit(char symbol, int pos) {
    return (pos < M && symbol >= '0' && symbol <= '1');
}

int stdin_input(int grid[N][M]) {
    int res = 1;
    char current;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j <= M; j++) {
            current = getchar();
            if (check_bit(current, j) || check_linebreak(current, j)) {
                if (j < M) {
                    grid[i][j] = current - '0';
                }
            } else {
                res = 0;
                break;
            }
        }
        if (!res) {
            break;
        }
    }
    return res;
}

void compute_next_gen(int curr_generation_grid[N][M], int next_generation_grid[N][M]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            int state = curr_generation_grid[i][j];

            // считаем кол-во соседей
            int neighbors = count_neibhors(curr_generation_grid, i, j);
            // если клетка мертва, и рядом есть 3 соседа --> клетка респавнится
            if (state == 0 && neighbors == 3) {
                next_generation_grid[i][j] = 1;
            } else if (state == 1 && (neighbors < 2 || neighbors > 3)) {
                // если клетка жива, и рядом НЕ 2-3 соседа --> клетка умирает
                next_generation_grid[i][j] = 0;
                } else {
                    next_generation_grid[i][j] = state;              // иначе, все остается, как есть
                }
        }
    }
}

int gens_equal(int gen1[N][M], int gen2[N][M]) {
    // проверяем, совпадает ли новая фаза с текущей
    int equal = 1;
    for (int i = 0; i < N && equal == 1; i++) {
        for (int j = 0; j < M && equal  == 1; j++) {
            if (gen1[i][j] != gen2[i][j]) {
                equal = 0;
            }
        }
    }
    return equal;
}

void print_gen(int grid[N][M]) {
    printf("\033[0d\033[2J\033[7m\033[1m");
    // выводим на экран новое состояние
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (grid[i][j] == 1) {
                printf("*");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
    printf("\033[0;0m");
}

void shift_gens(int curr[N][M], int next[N][M]) {
    //  смещение: prev <-- curr; curr <-- next; next обнуляем
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            curr[i][j] = next[i][j];
            next[i][j] = 0;
        }
    }
}

int count_neibhors(int grid[N][M], int x, int y) {
    int sum = 0;
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            int col = (x + i + N) % N;
            int row = (y + j + M) % M;
            sum += grid[col][row];
        }
    }
    sum -= grid[x][y];
    return sum;
}
