#ifndef smm_object_h
#define smm_object_h

#include "smm_common.h"

#define SMMNODE_TYPE_LECTURE            0
#define SMMNODE_TYPE_RESTAURANT         1
#define SMMNODE_TYPE_LAB                2
#define SMMNODE_TYPE_HOUSE              3
#define SMMNODE_TYPE_EXPERIMENT         4
#define SMMNODE_TYPE_FOODCHANCE         5
#define SMMNODE_TYPE_MISSION            6

#define SMMNODE_TYPE_MAX                7

typedef enum smmObjType {
    smmObjType_board = 0,
    smmObjType_card,
    smmObjType_grade
} smmObjType_e;

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

typedef struct smmObject {
       char name[MAX_CHARNAME];
       smmObjType_e objType; 
       int type;
       int credit;
       int energy;
       smmObjGrade_e grade;
} smmObject_t;

// Placeholder for smmObjGrade_e definition
// Define smmObjGrade_e here or include the header where it's defined

typedef struct smmFoodCard {
    char name[MAX_CHARNAME];
    int energy;
} smmFoodCard_t;

smmFoodCard_t* smmObj_genFoodCard(char* name, int energy);

void loadFoodCards(const char* filename);
smmFoodCard_t* getRandomFoodCard();
void freeFoodCards();

// Object generation function
void* smmObj_genObject(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade);

// 노드 함수 
char* smmObj_getNodeName(int node_nr);
int smmObj_getNodeType(int node_nr);
int smmObj_getNodeCredit(int node_nr);
int smmObj_getNodeEnergy(int node_nr);

char* smmObj_getTypeName(int type);

#endif /* smm_object_h */
