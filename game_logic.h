//
//game_logec.h
//
//

#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#define EXPERIMENT_SUCCESS_THRESHOLD 4

#include "player.h"

void actionNode(int player); //��庰 �׼�  
void drawFoodCardAndReplenishEnergy(int player);//����ī�� ����  
void performExperiment(int player); // ������ ���� 
void replenishEnergyAtHouse(int player); // �Ͽ콺 
void performRandomMission(int player); //�������� 

#endif // GAME_LOGIC_H
