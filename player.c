//
//player.c
//

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
         cur_player[i].energy = initEnergy; //초기 에너지 설정 
     }
}

void initializePlayer(Player *player, const char *name) {
//플레이어 초기화     
    strncpy(player->name, name, MAX_CHARNAME - 1);
    player->name[MAX_CHARNAME - 1] = '\0'; 

//나머지 값 초기화  
    player->energy = DEFAULT_INIT_ENERGY;
    player->position = 0;
    player->accumCredit = 0;
    player->flag_graduate = 0;
}

void goForward(Player *player, int step)
{
     //플레이어의 현재 위치 업데이트
     int oldPosition = cur_player[player].position; //업데이트 전에 정보 저장 
     cur_player[player].position += step;
     
     // Check if the player passed the Home node
     bool passedHouse = (oldPosition < HOUSE_NODE_INDEX && cur_player[player].position >= HOUSE_NODE_INDEX) ||
                      (cur_player[player].position >= board_nr && (oldPosition % board_nr) < HOUSE_NODE_INDEX);
     
     //마지막 노드를 지났는지 확인 ( 첫 노드로 돌아가기)
      if (cur_player[player].position >= board_nr){
        cur_player[player].position %= board_nr;
        //루프 하나를 완료 했을 때 첫 노드로 돌아가기 
        }
      
      // 졸업여부확인
      if(cur_player[player].accumCredit >= GRADUATE_CREDIT && passedHouse){
        
        //졸업했고, 하우스 노드에 도달(지나치면) 게임 종료 
        printf("%s has won the game!\n", cur_player[player].name);
        
        endGame();
        return;
        } 
        //기타 조건 추가할 경우 
} 

//강의 수강 완료 시 등급 알려주는 기능 
void printGrades(int player) 
{
    int i;
    void *gradePtr;

    for (i = 0; i < smmdb_len(LISTNO_OFFSET_GRADE + player); i++) {
        gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
        printf("%s : %i\n", smmObj_getNodeName(gradePtr), smmObj_getNodeGrade(gradePtr));
    }
}

//플레이어 스테이터스 표시 
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

