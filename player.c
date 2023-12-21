//
//player.c
//

#include "player.h"

void initializePlayer(Player *player, const char *name) {
    //�÷��̾� �̸� ����
    strncpy(player->name, name, MAX_PLAYER_NAME - 1);
    player->name[MAX_CHARNAME - 1] = '\0'; // ���ᰡ ����� �� ���� Ȯ��
    
     //�ٸ� �͵� ����
    player->energy = DEFAULT_INIT_ENERGY; //������ �ʱ�ȭ 
    player->position = 0; // ���� ���� �ʱ�ȭ 
    player->accumCredit = 0; // ���� �ʱ�ȭ 
    player->flag_graduate = 0; // ����X 
}

void goForward(Player *player, int step) {
    // Implementation
}
