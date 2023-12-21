//
//player.c
//

#include "player.h"

void initializePlayer(Player *player, const char *name) {
    //플레이어 이름 세팅
    strncpy(player->name, name, MAX_PLAYER_NAME - 1);
    player->name[MAX_CHARNAME - 1] = '\0'; // 종료가 제대로 된 건지 확인
    
     //다른 것도 세팅
    player->energy = DEFAULT_INIT_ENERGY; //에너지 초기화 
    player->position = 0; // 시작 지점 초기화 
    player->accumCredit = 0; // 학점 초기화 
    player->flag_graduate = 0; // 졸업X 
}

void goForward(Player *player, int step) {
    // Implementation
}
