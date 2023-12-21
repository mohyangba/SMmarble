//
//game_logic.c
//

#include "game_logic.h"
#include "smm_object.h" 

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
             printf("Player %s has arrived at the Restaurant node.\n", cur_player[player].name);
            // Draw a random food card and replenish energy
            drawFoodCardAndReplenishEnergy(player);
            break;
        
        case SMMNODE_TYPE_LAB:
             printf("Player %s has arrived at the Laboratory node. Nothing happens here.\n", cur_player[player].name);
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


void drawFoodCardAndReplenishEnergy(int player) {
    smmFoodCard_t* card = getRandomFoodCard();
    if (card != NULL) {
        cur_player[player].energy += card->energy;  // Recharge energy
        printf("%s drew a %s card and gained %d energy!\n", cur_player[player].name, card->name, card->energy);
    } else {
        printf("%s attempted to draw a food card but there were none left!\n", cur_player[player].name);
    }
}

void endGame() {
    // Display final scores or messages
    // Clean up resources if needed
    // Ask if players want to play again or exit the game
}           
