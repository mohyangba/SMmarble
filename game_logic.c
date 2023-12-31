//
//game_logic.c
//

#include "game_logic.h"
#include "smm_object.h" 
#include "player.h" 

#define MISSION_STRING_LENGTH 8

void drawFoodCardAndReplenishEnergy(int player) {
    smmFoodCard_t* card = getRandomFoodCard();
    if (card != NULL) {
        cur_player[player].energy += card->energy;  // Recharge energy
        printf("%s drew a %s card and gained %d energy!\n", cur_player[player].name, card->name, card->energy);
    } else {
        printf("%s attempted to draw a food card but there were none left!\n", cur_player[player].name);
    }
}

//실험중 상태  
void performExperiment(int player) {
    printf("Player %s is in the laboratory, conducting an experiment.\n", cur_player[player].name);

    // Mark the player as in the experiment state
    cur_player[player].inExperiment = 1;

    // Roll the die to determine the outcome of the experiment
    int diceRoll = rolldie(player);
    printf("Player %s rolls a %d in the experiment.\n", cur_player[player].name, diceRoll);

    if (diceRoll >= EXPERIMENT_SUCCESS_THRESHOLD) {
        printf("Experiment successful! Player %s exits the laboratory.\n", cur_player[player].name);
        // Exit experimenting state
        cur_player[player].inExperiment = 0;
    } else {
        printf("Experiment continues. Player %s must stay in the laboratory.\n", cur_player[player].name);
        // The player remains in the laboratory
    }
}

const char missionChars[] = {'q', 'w', 'e', 'r', 'a', 's', 'd', 'f'};
const int missionCharsCount = sizeof(missionChars) / sizeof(missionChars[0]);

void performRandomMission(int playerIndex) {
    char missionString[MISSION_STRING_LENGTH + 1];
    char playerInput[MISSION_STRING_LENGTH + 1];

    // Generate a random string from the specified set of characters
    for (int i = 0; i < MISSION_STRING_LENGTH; i++) {
        int randIndex = rand() % missionCharsCount;
        missionString[i] = missionChars[randIndex];
    }
    missionString[MISSION_STRING_LENGTH] = '\0'; // Null-terminate the string

    // Notify the player and get input
    printf("Player %s has arrived at a Mission node. Replicate this string: %s\n", cur_player[playerIndex].name, missionString);
    printf("Enter the string: ");
    scanf("%s", playerInput);

    // Check if the player replicated it correctly
    if (strcmp(missionString, playerInput) == 0) {
        cur_player[playerIndex].successfulMissions++;
        printf("Correct! Total successful missions: %d\n", cur_player[playerIndex].successfulMissions);

        // Check for three successful missions
        if (cur_player[playerIndex].successfulMissions >= 3) {
            printf("%s has completed three missions and wins the game!\n", cur_player[playerIndex].name);
            endGame();
        }
    } else {
        printf("Incorrect. Try again next time.\n");
    }
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
             //플레이어 에너지 확인 
    if (cur_player[player].energy >= some_energy_threshold) {
       printf("Player %s has arrived at the Lecture node.\n", cur_player[player].name);
        printf("Do you want to take the lecture at %s? (Y/N): ", name); //수강여부 
        char choice;
        scanf(" %c", &choice); 
        
        //수강한 경우 
        if (choice == 'Y' || choice == 'y') {
            cur_player[player].accumCredit += smmObj_getNodeCredit(boardPtr);
            cur_player[player].energy -= smmObj_getNodeEnergy(boardPtr);
            
            printf("You have taken the lecture. Credits: %d, Remaining Energy: %d\n", 
                   cur_player[player].accumCredit, cur_player[player].energy);

            // 성적 생성 
            gradePtr = smmObj_genObject(name, smmObjType_grade, 0, smmObj_getNodeCredit(boardPtr), 0, /* grade value */);
            smmdb_addTail(LISTNO_OFFSET_GRADE + player, gradePtr);
        //수강하지 않은 경우 
        } else {
            printf("You chose not to take the lecture.\n");
        }
    } else {
        printf("Not enough energy to take the lecture at %s.\n", name);
    }
    break;
        case SMMNODE_TYPE_RESTAURANT:
             {
             int energyValue = smmObj_getNodeEnergy(boardPtr); // Get energy value from the node
             cur_player[player].energy += energyValue; // Replenish player's energy

             printf("Player %s has arrived at the Restaurant and replenishes %d energy. Total energy: %d.\n", 
             cur_player[player].name, energyValue, cur_player[player].energy);

             break;
             }
        
        case SMMNODE_TYPE_LAB:
             printf("Player %s has arrived at the Laboratory node. Nothing happens here.\n", cur_player[player].name);
             break;
        
        case SMMNODE_TYPE_HOUSE:
             printf("Player %s has arrived at the House.\n", cur_player[player].name);
             break;
        
        case SMMNODE_TYPE_EXPERIMENT:
         performExperiment(player);

        // 실험중 상태에 있는지 아닌지 확인 
        if (!cur_player[player].inExperiment) {
           // 이제 실험 중 상태를 벗어낫으니까 주사위를 새로 굴린다 
           int diceRoll = rolldie(player);
           printf("Player %s rolls a %d and moves forward.\n", cur_player[player].name, diceRoll);

           // Assuming you have a function to move the player based on dice roll
           goFoward(player, diceRoll);
           }
            break;
        
        case SMMNODE_TYPE_SNACKTIME:
            //draw a random food card for energy
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


void endGame() {
    // Display final scores or messages
    // Clean up resources if needed
    // Ask if players want to play again or exit the game
}           
