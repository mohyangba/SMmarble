//
//game_logec.h
//
//

#ifndef game_logic_h
#define game_logic_h

#define EXPERIMENT_SUCCESS_THRESHOLD 4


void actionNode(int player); //��庰 �׼�  
void drawFoodCardAndReplenishEnergy(int player);//����ī�� ����  
void performExperiment(int player); // ������ ���� 
void replenishEnergyAtHouse(int player); // �Ͽ콺 
void performRandomMission(int player); //�������� 

#endif // GAME_LOGIC_H
