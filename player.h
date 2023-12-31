//
// player.h
// 이제 다른 파일의 플레이어 관련 삭제해야함  

#ifndef PLAYER_H
#define PLAYER_H

// Player-related constants
#define DEFAULT_INIT_ENERGY 100
// ... other constants

// Player structure
typedef struct player{
    int energy; 
    int position;
    char name[MAX_PLAYER_NAME];
    int accumCredit;
    int flag_graduate;
    int inExperiment; //실험중 상태 
    int successfulMissions; //미션 성공 
} player_t;

// Function prototypes for player operations
void initializePlayer(Player *player, const char *name, int initEnergy);
void goFoward(Player *player, int step);
void updatePlayerStatus(Player *player);
void generatePlayers(int n, int initEnergy);
void printGrades(int player);
void printPlayerStatus(void);
// ... other player-related functions
bool passesHouse(int oldPosition, int newPosition);

#endif // PLAYER_H
