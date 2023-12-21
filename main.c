//
//  main.c
//  SMMarble
//
//
//

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "smm_object.h"
#include "smm_database.h"
#include "smm_common.h"
#include "game_logic.h"

#define BOARDFILEPATH "marbleBoardConfig.txt"
#define FOODFILEPATH "marbleFoodConfig.txt"
#define MAX_FOOD_CARDS 100



//board configuration parameters
static int board_nr;
static int food_nr;
static int festival_nr;

static int player_nr;

static player_t *cur_player;
//static player_t cur_player[MAX_PLAYER];

#if 0
static int player_energy[MAX_PLAYER];
static int player_position[MAX_PLAYER];
static char player_name[MAX_PLAYER][MAX_CHARNAME];
#endif

//function prototypes
#if 0
int isGraduated(void); //check if any player is graduated
 //print grade history of the player
void goForward(int player, int step); //make player go "step" steps on the board (check if player is graduated)
void printPlayerStatus(void); //print all player status at the beginning of each turn
float calcAverageGrade(int player); //calculate average grade of the player
smmGrade_e takeLecture(int player, char *lectureName, int credit); //take the lecture (insert a grade of the player)
void* findGrade(int player, char *lectureName); //find the grade from the player's grade history
void printGrades(int player); //print all the grade history of the player
void actionNode(int player) // 플레이어가 특정 노드 위에 도착했을 때 취하는 행동 구현 
void endGame(void);//플레이어 승리 시 게임 종료  
#endif

void loadBoardConfig(const char* filepath) {
    FILE* fp;
    char name[MAX_CHARNAME];
    int type, credit, energy;

    fp = fopen(filepath, "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open board configuration file: %s\n", filepath);
        exit(EXIT_FAILURE);
    }

    while (fscanf(fp, "%s %d %d %d", name, &type, &credit, &energy) == 4) {
        // Here, create a new board object and add it to your board data structure
        void* boardObj = smmObj_genObject(name, smmObjType_board, type, credit, energy, 0);
        smmdb_addTail(LISTNO_NODE, boardObj);
    }

    if (ferror(fp)) {
        fprintf(stderr, "Error reading from file: %s\n", filepath);
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    fclose(fp);
}
                   
                   
static smmFoodCard_t* foodCards[MAX_FOOD_CARDS];
static int foodCardCount = 0;

void loadFoodCards(const char* filepath) {
    FILE* file = fopen(filepath, "r");
    if (!file) {
        fprintf(stderr, "Error opening food cards file: %s\n", filepath);
        exit(EXIT_FAILURE);
    }

    char foodName[MAX_CHARNAME];
    int energy;
    while (foodCardCount < MAX_FOOD_CARDS && fscanf(file, "%s %d", foodName, &energy) == 2) {
        foodCards[foodCardCount++] = smmObj_genFoodCard(foodName, energy);
    }

    if (ferror(file)) {
        fprintf(stderr, "Error reading from food cards file: %s\n", filepath);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fclose(file);
}

// Function to get a random food card - assumes `srand` has been called
smmFoodCard_t* getRandomFoodCard() {
    if (foodCardCount == 0) return NULL;
    int index = rand() % foodCardCount;
    return foodCards[index];
}

// Function to free allocated food cards
void freeFoodCards() {
    for (int i = 0; i < foodCardCount; i++) {
        free(foodCards[i]);
    }
    foodCardCount = 0;
}

int rolldie(int player)
{
    char c;
    printf(" Press any key to roll a die (press g to see grade): ");
    c = getchar();
    fflush(stdin);
}
    
#if 1
    if (c == 'g')
        printGrades(player);
#endif
    
    return (rand()%MAX_DIE + 1);
}

int main(int argc, const char * argv[]) {
    
    FILE* fp;
    char name[MAX_CHARNAME];
    int type;
    int credit;
    int energy;
    int i;
    int initEnergy;
    int turn=0;
    
    board_nr = 0;
    food_nr = 0;
    festival_nr = 0;
    
    srand(time(NULL));
    //foodcard&board need to be loaded
    loadBoardConfig(BOARDFILEPATH);
    loadFoodCards(FOODFILEPATH);
    
    
    // Player configuration
    printf("Enter the number of players: ");
    scanf("%d", &player_nr);
    while (player_nr < 1 || player_nr > MAX_PLAYER) {
        printf("Invalid number of players. Please enter a number between 1 and %d: ", MAX_PLAYER);
        scanf("%d", &player_nr);
    }

    cur_player = (player_t*)malloc(player_nr * sizeof(player_t));
    if (!cur_player) {
        perror("Failed to allocate memory for players");
        return -1;
    }

    // Initialize players
    for (int i = 0; i < player_nr; i++) {
        initializePlayer(&cur_player[i], i); // Assuming this function prompts for player name and initializes the player
    }

    // Main game loop
    while (1) { //조건 게임종료 X 
        printf("\nPlayer %s's turn.\n", cur_player[turn].name);

        // Player rolls die and moves
        int die_result = rolldie(&cur_player[turn]);
        goForward(&cur_player[turn], die_result);

        // Perform action at the current node
        actionNode(&cur_player[turn]);

        // Print player's updated status
        printPlayerStatus(&cur_player[turn]);

        // Check for game end condition (e.g., graduation)
        if (isGraduated(&cur_player[turn])) {
            printf("%s has graduated and wins the game!\n", cur_player[turn].name);
            break; // End the game loop
        }

        // Next player's turn
        turn = (turn + 1) % player_nr;
    }

    // Clean up
    free(cur_player);
    freeFoodCards(); // Clean up food cards if necessary
    printf("Game over. Thanks for playing!\n");

    return 0;
}
