//
//game_logec.h
//
//

#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "player.h"

void drawFoodCardAndReplenishEnergy(Player* player);
void endGame() // 게임 종료 
void actionNode(int player); //노드별 액션을 처리하는 함수 

#endif // GAME_LOGIC_H
