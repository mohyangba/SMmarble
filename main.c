//
//  main.c
//  SMMarble
//
//
//

#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "smm_object.h"
#include "smm_database.h"
#include "smm_common.h"
#include "game_logic.h"

#define BOARDFILEPATH "marbleBoardConfig.txt"
#define FOODFILEPATH "marbleFoodConfig.txt"



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

//action code when a player stays at a node
void actionNode(int player)
{
     //노드 검색 
    void *boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position );
    //int type = smmObj_getNodeType( cur_player[player].position );
    int type = smmObj_getNodeType( boardPtr );
    char *name = smmObj_getNodeName( boardPtr );
    void *gradePtr;
     //노드별 액션 처리 
    switch(type)
    {
        //case lecture:
        case SMMNODE_TYPE_LECTURE:
             //플레이어의 에너지가 충분한지 검사  
             if (cur_player[player].energy >= some_energy_threshold)
             {
             cur_player[player].accumCredit += smmObj_getNodeCredit( boardPtr );
             cur_player[player].energy -= smmObj_getNodeEnergy( boardPtr );
             } else {
                    //플레이어 에너지가 충분하지 않은 경우
             } 
            //grade generation
            gradePtr = smmObj_genObject(name, smmObjType_grade, 0, smmObj_getNodeCredit( boardPtr ), 0, ??);
            smmdb_addTail(LISTNO_OFFSET_GRADE + player, gradePtr);
            
            break;
        
        case SMMNODE_TYPE_RESTAURANT:
            // Draw a random food card and replenish energy
            drawFoodCardAndReplenishEnergy(player);
            break;
        
        case SMMNODE_TYPE_LAB:
            // Laboratory logic: no action if just arrived
            break;
        
        case SMMNODE_TYPE_HOUSE:
            // Replenish energy every time player passes the house
            replenishEnergyAtHouse(player);
            break;
        
        case SMMNODE_TYPE_EXPERIMENT:
            // Handle experiment logic: spend energy, conduct experiment
            performExperiment(player);
            break;
        
        case SMMNODE_TYPE_SNACKTIME:
            // Similar to Restaurant: draw a random food card for energy
            drawFoodCardAndReplenishEnergy(player);
            break;
        
        case SMMNODE_TYPE_MISSION:
            // Random mission logic: implement various missions
            performRandomMission(player);
            break;
     
     default:
            break;
    }
}



// Example implementation for drawing food card and replenishing energy
void drawFoodCardAndReplenishEnergy(Player* player) {
    smmFoodCard_t* card = getRandomFoodCard();
    if (card != NULL) {
        player->energy += card->energy;  // Recharge energy
        printf("You drew a %s card and gained %d energy!\n", card->name, card->energy);
    }
}

// ... implementations for replenishEnergyAtHouse, performExperiment, performRandomMission

void goForward(int player, int step)
{
     void *boardPtr;
     cur_player[player].position += step;
     boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position );
     
     printf("%s go to node %i (name: %s)\n", 
                cur_player[player].name, cur_player[player].position,
                smmObj_getNodeName(boardPtr));
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
    //foodcard need to be loaded
    void loadFoodCards(const char* filename)
    //1. import parameters ---------------------------------------------------------------------------------
    //1-1. boardConfig 
    if ((fp = fopen(BOARDFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", BOARDFILEPATH);
        getchar();
        return -1;
    }
    
    printf("Reading board component......\n");
    while ( fscanf(fp, "%s %i %i %i", name, &type, &credit, &energy) == 4 ) //read a node parameter set
    {
        //store the parameter set
        //(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade)
        void *boardObj = smmObj_genObject(name, smmObjType_board, type, credit, energy, 0);
        smmdb_addTail(LISTNO_NODE, boardObj);
        
        if (type == SMMNODE_TYPE_HOME)
           initEnergy = energy;
        board_nr++;
    }
    fclose(fp);
    printf("Total number of board nodes : %i\n", board_nr);
    
    
    for (i = 0;i<board_nr;i++)
    {
        void *boardObj = smmdb_getData(LISTNO_NODE, i);
        
        printf("node %i : %s, %i(%s), credit %i, energy %i\n", 
                     i, smmObj_getNodeName(boardObj), 
                     smmObj_getNodeType(boardObj), smmObj_getTypeName(smmObj_getNodeType(boardObj)),
                     smmObj_getNodeCredit(boardObj), smmObj_getNodeEnergy(boardObj));
    }
    //printf("(%s)", smmObj_getTypeName(SMMNODE_TYPE_LECTURE));
    
    
    //2. food card config 
    if ((fp = fopen(FOODFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FOODFILEPATH);
        return -1;
    }
    
    printf("\n\nReading food card component......\n");
    while () //read a food parameter set
    {
        //store the parameter set
    }
    fclose(fp);
    printf("Total number of food cards : %i\n", food_nr);
    
    //2. Player configuration ---------------------------------------------------------------------------------
    
    do
    {
        //input player number to player_nr
        printf("input player no.:");
        scanf("%d", &player_nr);
        fflush(stdin);
    }
    while (player_nr < 0 || player_nr >  MAX_PLAYER);
    
    cur_player = (player_t*)malloc(player_nr*sizeof(player_t));
    generatePlayers(player_nr, initEnergy);
    
    
    //3. SM Marble game starts ---------------------------------------------------------------------------------
    while (1) //is anybody graduated? 게임이 종료되지 않을 조건  
    {
        int die_result;
        
        //GAME LOOP
        //4-1. initial printing
        printPlayerStatus();
        
        //4-2. die rolling (if not in experiment)        
        die_result = rolldie(turn);
        
        //4-3. go forward
        goForward(turn, die_result);

		//4-4. take action at the destination node of the board
        actionNode(turn);
        
        //4-5. next turn
        turn = (turn + 1)%player_nr;
    }
    
    
    free(cur_player);
    system("PAUSE");
    return 0;
}
