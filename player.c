//
//player.c
//

#include "smm_common.h"
#include "player.h"

void generatePlayers(int n, int initEnergy) //generate a new player
{

     char name[MAX_CHARNAME];
     int i;
     //n time loop
     for (i=0;i<n;i++)
     {
         //input name
         printf("Input player %i's name: ", i);
         fgets(name, MAX_CHARNAME, stdin);
         name[strcspn(name, "\n")] = '\0';
         
         initializePlayer(&cur_player[i], name);
         cur_player[i].energy = initEnergy; //�ʱ� ������ ���� 
     }
}

void initializePlayer(Player *player, const char *name) {
//�÷��̾� �ʱ�ȭ     
    strncpy(player->name, name, MAX_CHARNAME - 1);
    player->name[MAX_CHARNAME - 1] = '\0'; 

//������ �� �ʱ�ȭ  
    player->energy = DEFAULT_INIT_ENERGY;
    player->position = 0;
    player->accumCredit = 0;
    player->flag_graduate = 0;
}

bool passesHouse(int oldPosition, int newPosition) {
    // Check if the player's path from oldPosition to newPosition passes the house
    if (oldPosition < HOUSE_NODE_INDEX && newPosition >= HOUSE_NODE_INDEX) {
        return true; // Passed the house
    } else if (newPosition < oldPosition && newPosition >= HOUSE_NODE_INDEX) {
        // Case for looping past the last node back to the start
        return true;
    }
    return false;
}

void goForward(Player *player, int step)
{
     //�÷��̾��� ���� ��ġ ������Ʈ
     int oldPosition = player->position;
    player->position = (oldPosition + step) % TOTAL_NODES;
     
     // ���� �������� Ȯ��  
     bool passedHouse = (oldPosition < HOUSE_NODE_INDEX && player->position >= HOUSE_NODE_INDEX) ||
                       (oldPosition > player->position); // Simplified loop check
                       
     if (passedHouse) {
        replenishEnergyAtHouse(player);
    }

    // ���� ���� Ȯ��  
    if (player->accumCredit >= GRADUATE_CREDIT && (passedHouse || player->position == HOUSE_NODE_INDEX)) {
        printf("%s has won the game by reaching the House node with enough credits!\n", player->name);
        endGame();
        return;
    }

    //���ο� ��ġ���� ��� �׼� ���� 
    actionNode(player);
}

//���� ���� �Ϸ� �� ��� �˷��ִ� ��� 
void printGrades(int player) 
{
    int i;
    void *gradePtr;

    for (i = 0; i < smmdb_len(LISTNO_OFFSET_GRADE + player); i++) {
        gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
        printf("%s : %i\n", smmObj_getNodeName(gradePtr), smmObj_getNodeGrade(gradePtr));
    }
}

//�÷��̾� �������ͽ� ǥ�� 
void printPlayerStatus(void) 
{
    if (cur_player == NULL || player_nr == 0) {
        printf("No players to display.\n");
        return;
    }

    for (int i = 0; i < player_nr; i++) {
        printf("Player %d - %s: Credits: %d, Energy: %d, Position: %d\n", 
               i + 1, 
               cur_player[i].name,
               cur_player[i].accumCredit,
               cur_player[i].energy,
               cur_player[i].position);
    }
}

