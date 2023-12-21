//
//  smm_database.c
//  Sookmyung Marble
//  Database platform code for storing marble board elements
// 
//

#include "smm_common.h"
#include "smm_database.h"

#define LIST_END            -1
#define MAX_LIST            LISTNO_OFFSET_GRADE + MAX_PLAYER

// Node definition for linked list
typedef struct node {
    int index;        // Index of the node
    struct node* next;     // Pointer to the next node
    struct node* prev;     // Pointer to the previous node
    void* obj;        // Object data
} node_t;

static node_t* list_database[MAX_LIST];
static node_t* listPtr[MAX_LIST];
static int list_cnt[MAX_LIST];

// Inner functions (cannot be used outside of this file)
static node_t* genNode(void) {
    node_t* ndPtr = (node_t*)malloc(sizeof(node_t));
    if (!ndPtr) {
        perror("[ERROR] Failed to allocate memory for node");
        return NULL;
    }

    ndPtr->next = NULL;
    ndPtr->prev = NULL;
    ndPtr->obj = NULL;

    return ndPtr;
}

static node_t* smmList(int list_nr, int index) {
    if (list_nr < 0 || list_nr >= MAX_LIST || index < -1 || index >= list_cnt[list_nr]) {
        printf("[ERROR] Invalid list number or index (list_nr: %d, index: %d)\n", list_nr, index);
        return NULL;
    }

    node_t* ndPtr = list_database[list_nr];
    while (ndPtr != NULL && ndPtr->index != index) {
        ndPtr = ndPtr->next;
    }

    return ndPtr;
}

static void updateIndex(int list_nr) {
    int index = 0;
    node_t* ndPtr = list_database[list_nr];
    while (ndPtr != NULL) {
        ndPtr->index = index++;
        ndPtr = ndPtr->next;
    }
}

// API functions
int smmdb_addTail(int list_nr, void* obj) {
    if (!obj) {
        printf("[ERROR] Object to add is NULL\n");
        return -1;
    }

    node_t* newNdPtr = genNode();
    if (!newNdPtr) {
        return -1;  // Memory allocation failed
    }

    newNdPtr->obj = obj;

    if (!list_database[list_nr]) {
        list_database[list_nr] = newNdPtr;
    } else {
        node_t* lastNdPtr = smmList(list_nr, list_cnt[list_nr] - 1);
        lastNdPtr->next = newNdPtr;
        newNdPtr->prev = lastNdPtr;
    }

    newNdPtr->index = list_cnt[list_nr]++;
    listPtr[list_nr] = newNdPtr;

    return 0;
}

int smmdb_deleteData(int list_nr, int index) {
    node_t* delNdPtr = smmList(list_nr, index);
    if (!delNdPtr) {
        printf("[ERROR] Node to delete not found (index: %d)\n", index);
        return -1;
    }

    if (delNdPtr->prev) {
        delNdPtr->prev->next = delNdPtr->next;
    } else {
        list_database[list_nr] = delNdPtr->next;  // Update head if first node is deleted
    }

    if (delNdPtr->next) {
        delNdPtr->next->prev = delNdPtr->prev;
    }

    free(delNdPtr->obj);
    free(delNdPtr);

    list_cnt[list_nr]--;
    updateIndex(list_nr);

    return 0;
}

int smmdb_len(int list_nr) {
    if (list_nr < 0 || list_nr >= MAX_LIST) {
        printf("[ERROR] Invalid list number (list_nr: %d)\n", list_nr);
        return -1;
    }

    return list_cnt[list_nr];
}

void* smmdb_getData(int list_nr, int index) {
    node_t* ndPtr = smmList(list_nr, index);
    if (!ndPtr) {
        printf("[ERROR] Data not found (index: %d)\n", index);
        return NULL;
    }

    return ndPtr->obj;
}
