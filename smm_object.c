//
//  smm_object.c
//  SMMarble
//
//  
//

#include <stdlib.h>
#include <string.h>
#include <time.h> 

#include "smm_object.h"
#include "smm_common.h"


#define MAX_NODETYPE    7 // ��Ʈ Ÿ���� 7����� ����  
#define MAX_GRADE       9 // 9���� ���  
#define MAX_NODE        100 // �ִ� ��� ������ 100��  
#define MAX_FOOD_CARDS 100 // �ִ� ���� ī�� ����  

static char smmNodeName[MAX_NODETYPE][MAX_CHARNAME] = {
    "Lecture",
    "Restaurant",
    "Lab",
    "House",
    "Experiment",
    "Snacktime",
    "Mission"
};

char* smmObj_getTypeName(int type)
{
      return(char*)smmNodeName[type];
}



typedef enum smmObjGrade {
    smmObjGrade_Ap = 0,
    smmObjGrade_A0,
    smmObjGrade_Am,
    smmObjGrade_Bp,
    smmObjGrade_B0,
    smmObjGrade_Bm,
    smmObjGrade_Cp,
    smmObjGrade_C0,
    smmObjGrade_Cm
} smmObjGrade_e;

//1. ����ü ���� ����
typedef struct smmObject {
       char name[MAX_CHARNAME];
       smmObjType_e objType; 
       int type;
       int credit;
       int energy;
       smmObjGrade_e grade;
} smmObject_t;

//smmObj_genFoodCard �Լ� ����  
smmFoodCard_t* smmObj_genFoodCard(char* name, int energy) {
    smmFoodCard_t* card = (smmFoodCard_t*)malloc(sizeof(smmFoodCard_t));
    if (card != NULL) {
        strcpy(card->name, name);
        card->energy = energy;
    }
    return card;
}

//���� ī�� ���� �Լ� ����  
static smmFoodCard_t* foodCards[MAX_FOOD_CARDS];
static int foodCardCount = 0;
       //1)���Ͽ��� ���� ī�� �ҷ����� 
void loadFoodCards(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char foodName[MAX_CHARNAME];
    int energy;
    while (foodCardCount < MAX_FOOD_CARDS && fscanf(file, "%s %d", foodName, &energy) == 2) {
        foodCards[foodCardCount++] = smmObj_genFoodCard(foodName, energy);
    }

    fclose(file);
}
       //2}���� ī�� �� ������ ���� (����ī��迭�� �̹� ä�����ִٴ� ���� �Ͽ�)
smmFoodCard_t* getRandomFoodCard() {
    if (foodCardCount == 0) return NULL;

    srand(time(NULL)); // �������� ->main���� �ű�� ����� ����  
    int index = rand() % foodCardCount;
    return foodCards[index];
}
       //3)���� ī�� ����� �ʿ����� ���� ��� (���� ����) = �޸� ����
void freeFoodCards() {
    for (int i = 0; i < foodCardCount; i++) {
        free(foodCards[i]);
    }
    foodCardCount = 0;
}       


static smmObject_t smm_node[MAX_NODE];
static int smmObj_noNode = 0;

//3. ���� �Լ� ���� 
//object generation
void* smmObj_genObject(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade)
{    
    smmObject_t* ptr;
    
    ptr = (smmObject_t*)malloc(sizeof(smmObject_t));
    
    strcpy(ptr->name, name);
    ptr->objType = objType;
    ptr->type = type;
    ptr->credit = credit;
    ptr->energy = energy;
    ptr->grade = grade;
    
    return ptr;
}

//3. ���� �Լ� ���� 
char* smmObj_getNodeName(void* obj)
{
    smmObject_t* ptr = (smmObject_t*)obj;
    
    return ptr->name;
}

int smmObj_getNodeType(int node_nr)
{
    return smm_node[node_nr].type;
}

int smmObj_getNodeCredit(int node_nr)
{
    return smm_node[node_nr].credit;
}

int smmObj_getNodeEnergy(int node_nr)
{
    return smm_node[node_nr].energy;
}
