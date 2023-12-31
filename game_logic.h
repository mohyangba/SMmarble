//
//game_logec.h
//
//

#ifndef game_logic_h
#define game_logic_h

#define EXPERIMENT_SUCCESS_THRESHOLD 4


void actionNode(int player); //노드별 액션  
void drawFoodCardAndReplenishEnergy(int player);//음식카드 섭취  
void performExperiment(int player); // 실험중 상태 
void replenishEnergyAtHouse(int player); // 하우스 
void performRandomMission(int player); //아재패턴 

#endif // GAME_LOGIC_H
