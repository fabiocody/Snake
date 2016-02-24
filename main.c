//
//  main.c
//  Snake
//
//  Created by Fabio Codiglioni on 23/02/16.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <termios.h>

#define VERSION "1.1.0"
#define HEIGHT 20
#define WIDTH 40
#define EMPTY 0
#define FOOD -1
#define for_i for (i = 0; i < HEIGHT; i++)
#define for_j for (j = 0; j < WIDTH; j++)


/*** GLOBAL VARIABLES ***/
int grid[HEIGHT][WIDTH];


/*** HEADERS ***/
void clearScreen();
void splashScreen();
void printGrid(int score, int record);
void generateFood();
unsigned int turnSnake(char direction, unsigned int end, unsigned int *foodFlag);




/*** MAIN ***/
int main(int argc, const char *argv[]) {
    unsigned int i, j;
    unsigned int end = 0;
    unsigned int foodFlag = 0;
    unsigned int length = 1;
    int score = -1, record = 0;
    char direction;
    static struct termios oldt, newt;       // Structs to get all the keystrokes directly to stdin
    
    FILE *recordFile = fopen("snake_record.bin", "r+b");
    if (recordFile != NULL) {
        fread(&record, sizeof(int), 1, recordFile);
        fclose(recordFile);
    }
    recordFile = NULL;
    
    srand((unsigned int)time(NULL));
    
    // Initialize grid
    for_i
        for_j
            grid[i][j] = EMPTY;
    for (i = 21; i >= 18; i--, length++)
        grid[10][i] = length;
    
    splashScreen();
    
    // Instructions to get all the keystrokes directly to stdin
    tcgetattr( STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON);
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);
    
    // Game cycle
    while (end == 0) {
        if (foodFlag == 0) {
            generateFood();
            foodFlag = 1;
            score++;
        }
        clearScreen();
        printGrid(score, record);
        do {
            direction = getchar();
        } while (direction != 'w' && direction != 'a' && direction != 's' && direction != 'd' && direction != 'q');
        if (direction == 'q') {
            puts("\n\nEXIT\n");
            exit(EXIT_SUCCESS);
        }
        end = turnSnake(direction, end, &foodFlag);
    }

    puts("\n\nGAME OVER!\n");
    
    if (score > record) {
        recordFile = fopen("snake_record.bin", "wb");
        if (recordFile != NULL) {
            fwrite(&score, sizeof(int), 1, recordFile);
            fclose(recordFile);
        }
    }
    
    // Restore normal mode
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
}



/*** FUNCTIONS ***/

void clearScreen() {
    fflush(stdout);
    printf("\e[1;1H\e[2J");
    fflush(stdout);
}


void splashScreen() {
    clearScreen();
    //printf("        _____ _   _____    __ __ ______\n       / ___// | / /   |  / //_// ____/\n       \\__ \\/  |/ / /| | / ,<  / __/\n      ___/ / /|  / ___ |/ /| |/ /___\n     /____/_/ |_/_/  |_/_/ |_/_____/               v%s\n", VERSION);
    //puts("\nCreated by Fabio Codiglioni");
    printf("   _____ _   _          _  ________\n  / ____| \\ | |   /\\   | |/ /  ____|     \n | (___ |  \\| |  /  \\  | ' /| |__        \n  \\___ \\| . ` | / /\\ \\ |  < |  __|                                  ____\n  ____) | |\\  |/ ____ \\| . \\| |____        ________________________/ O  \\___/\n |_____/|_| \\_/_/    \\_\\_|\\_\\______|      <_/_\\_/_\\_/_\\_/_\\_/_\\_/_______/   \\        v%s", VERSION);
    fflush(stdout);
    sleep(2);
    clearScreen();
}


void printGrid(int score, int record) {
    unsigned int i, j;
    fflush(stdout);
    for (i = 0; i < WIDTH + 2; i++)
        printf("%c", '-');
    printf("        Score: %d\n", score);
    for_i {
        printf("%c", '|');
        for_j {
            if (grid[i][j] == EMPTY)
                printf("%c", ' ');
            else if (grid[i][j] == FOOD)
                printf("%c", 'w');
            else if (grid[i][j] == 1)
                printf("%c", 'o');
            else if (grid[i][j] > 1)
                printf("%c", '*');
        }
        if (i == 0)
            printf("|        Record: %d\n", record);
        else
            printf("%s", "|\n");
    }
    for (i = 0; i < WIDTH + 2; i++)
        printf("%c", '-');
    puts("");
    fflush(stdout);
}


void generateFood() {
    unsigned int i, j;
    do {
        i = rand() % HEIGHT;
        j = rand() % WIDTH;
    } while (grid[i][j] != EMPTY);
    grid[i][j] = FOOD;
}


unsigned int turnSnake(char direction, unsigned int end, unsigned int *foodFlag) {
    unsigned int i, j, foodEaten = 0;
    int max = 0;
    for_i {
        for_j {
            if (grid[i][j] > 0) {
                grid[i][j]++;
            }
        }
    }
    for_i
        for_j
            if (grid[i][j] > max)
                max = grid[i][j];
    for_i
        for_j
            if (grid[i][j] == 2) {
                switch (direction) {
                    case 'w':
                        if (grid[i - 1][j] == EMPTY)
                            grid[i - 1][j] = 1;
                        else if (grid[i - 1][j] == FOOD) {
                            foodEaten = 1;
                            grid[i - 1][j] = 1;
                        }
                        else
                            end = 1;
                        break;
                    case 'a':
                        if (grid[i][j - 1] == EMPTY)
                            grid[i][j - 1] = 1;
                        else if (grid[i][j - 1] == FOOD) {
                            foodEaten = 1;
                            grid[i][j - 1] = 1;
                        }
                        else
                            end = 1;
                        break;
                    case 's':
                        if (grid[i + 1][j] == EMPTY)
                            grid[i + 1][j] = 1;
                        else if (grid[i + 1][j] == FOOD) {
                            foodEaten = 1;
                            grid[i + 1][j] = 1;
                        }
                        else
                            end = 1;
                        break;
                    case 'd':
                        if (grid[i][j + 1] == EMPTY)
                            grid[i][j + 1] = 1;
                        else if (grid[i][j + 1] == FOOD) {
                            foodEaten = 1;
                            grid[i][j + 1] = 1;
                        }
                        else
                            end  = 1;
                        break;
                }
            }
    for_i
        for_j
            if (grid[i][j] == max && foodEaten == 0)
                grid[i][j] = EMPTY;
    if (foodEaten == 1)
        *foodFlag = 0;
    return end;
}