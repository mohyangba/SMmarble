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


//smmObj_genFoodCard 함수 구현  
smmFoodCard_t* smmObj_genFoodCard(char* name, int energy) {
    smmFoodCard_t* card = (smmFoodCard_t*)malloc(sizeof(smmFoodCard_t));
    if (card != NULL) {
        strcpy(card->name, name);
        card->energy = energy;
    }
    return card;
}

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
