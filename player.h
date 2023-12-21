//
// player.h
// ���� �ٸ� ������ �÷��̾� ���� �����ؾ���  

#ifndef PLAYER_H
#define PLAYER_H

// Player-related constants
#define MAX_PLAYER_NAME 50
#define DEFAULT_INIT_ENERGY 100
// ... other constants

// Player structure
typedef struct {
    int energy; 
    int position;
    char name[MAX_PLAYER_NAME];
    int accumCredit;
    int flag_graduate;
} player_t;

// Function prototypes for player operations
void initializePlayer(Player *player, const char *name, int initEnergy);
void goFoward(Player *player, int step);
void updatePlayerStatus(Player *player);
// ... other player-related functions

#endif // PLAYER_H
