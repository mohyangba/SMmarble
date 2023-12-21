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


#define MAX_NODETYPE    7 // 노트 타입이 7개라고 가정  
#define MAX_GRADE       9 // 9개의 등급  
#define MAX_NODE        100 // 최대 노드 개수가 100개  
#define MAX_FOOD_CARDS 100 // 최대 음식 카드 개수  

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

//1. 구조체 형식 정의
typedef struct smmObject {
       char name[MAX_CHARNAME];
       smmObjType_e objType; 
       int type;
       int credit;
       int energy;
       smmObjGrade_e grade;
} smmObject_t;

//smmObj_genFoodCard 함수 구현  
smmFoodCard_t* smmObj_genFoodCard(char* name, int energy) {
    smmFoodCard_t* card = (smmFoodCard_t*)malloc(sizeof(smmFoodCard_t));
    if (card != NULL) {
        strcpy(card->name, name);
        card->energy = energy;
    }
    return card;
}

//음식 카드 관련 함수 정의  
static smmFoodCard_t* foodCards[MAX_FOOD_CARDS];
static int foodCardCount = 0;
       //1)파일에서 음식 카드 불러오기 
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
       //2}음식 카드 중 무작위 선택 (음식카드배열이 이미 채워져있다는 가정 하에)
smmFoodCard_t* getRandomFoodCard() {
    if (foodCardCount == 0) return NULL;

    srand(time(NULL)); // 난수생성 ->main으로 옮기고 여기는 삭제  
    int index = rand() % foodCardCount;
    return foodCards[index];
}
       //3)음식 카드 사용이 필요하지 않은 경우 (게임 종료) = 메모리 정리
void freeFoodCards() {
    for (int i = 0; i < foodCardCount; i++) {
        free(foodCards[i]);
    }
    foodCardCount = 0;
}       


static smmObject_t smm_node[MAX_NODE];
static int smmObj_noNode = 0;

//3. 관련 함수 변경 
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

//3. 관련 함수 변경 
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
