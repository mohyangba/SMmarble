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


void drawFoodCardAndReplenishEnergy(Player* player) {
    smmFoodCard_t* card = getRandomFoodCard();
    if (card != NULL) {
        player->energy += card->energy;  // Recharge energy
        printf("You drew a %s card and gained %d energy!\n", card->name, card->energy);
    } else {
        printf("No more food cards left!\n");
    }
}

void endGame() {
    // Display final scores or messages
    // Clean up resources if needed
    // Ask if players want to play again or exit the game
}           
