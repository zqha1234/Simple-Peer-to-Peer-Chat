#include "list.h"
#include <stdlib.h>

static List* heads[LIST_MAX_NUM_HEADS];
static Node* nodes[LIST_MAX_NUM_NODES];
static int numHeads = 0;
static int numNodes = 0;

// Makes a new, empty list, and returns its reference on success. 
// Returns a NULL pointer on failure.
List* List_create(){
    if (numHeads >= LIST_MAX_NUM_HEADS){
        return NULL;
    }
    List* pList = (List*)malloc(sizeof(List));
    
    pList->nodeData = NULL; 
    pList->currPtrStatus = LIST_OOB_START; 
    pList->count = 0; 
    pList->currNode = NULL; 
    pList->first = NULL; 
    pList->last = NULL; 
    pList->pre = NULL; 
    pList->next = NULL; 
    
    heads[numHeads] = pList;
    numHeads++;
    return pList;
}

// Returns the number of items in pList.
int List_count(List* pList){
    if (pList == NULL){
        return 0; // return 0 if the pList is NULL
    }
    return pList->count;
}

// Returns a pointer to the first item in pList and makes the first item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_first(List* pList){
    if (pList == NULL){
        return NULL;
    }
    if (pList->first == NULL){
        pList->currNode = NULL;
        pList->currPtrStatus = LIST_OOB_START;
        return NULL;
    }
    pList->currNode = pList->first;
    return pList->currNode->nodeData->data;
}

// Returns a pointer to the last item in pList and makes the last item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_last(List* pList){
    if (pList == NULL){
        return NULL;
    }
    if (pList->first == NULL){
        pList->currNode = NULL;
        pList->currPtrStatus = LIST_OOB_START;
        return NULL;
    }
    pList->currNode = pList->last;
    return pList->currNode->nodeData->data;
}

// Advances pList's current item by one, and returns a pointer to the new current item.
// If this operation advances the current item beyond the end of the pList, a NULL pointer 
// is returned and the current item is set to be beyond end of pList.
void* List_next(List* pList){
    // pList is NULL or currNode is NULL, return NULL
    if (pList == NULL){
        return NULL;
    }
    if (pList->currNode == NULL){
        if (pList->currPtrStatus == LIST_OOB_END){
            return NULL;
        }else if (pList->currPtrStatus == LIST_OOB_START){
            pList->currNode = pList->first;
            return pList->currNode->nodeData->data;
        }
    }
    // currNode is the last node in the list, return NULL
    if (pList->currNode == pList->last){
        pList->currPtrStatus = LIST_OOB_END;
        pList->currNode = NULL;
        return NULL;
    }
    pList->currNode = pList->currNode->next;
    return pList->currNode->nodeData->data;
}

// Backs up pList's current item by one, and returns a pointer to the new current item. 
// If this operation backs up the current item beyond the start of the pList, a NULL pointer 
// is returned and the current item is set to be before the start of pList.
void* List_prev(List* pList){
    // pList is NULL or currNode is NULL, return NULL
    if (pList == NULL){
        return NULL;
    }
    if (pList->currNode == NULL){
        if (pList->currPtrStatus == LIST_OOB_START){
            return NULL;
        }else if (pList->currPtrStatus == LIST_OOB_END){
            pList->currNode = pList->last;
            return pList->currNode->nodeData->data;
        }
    }
    // currNode is the first node in the list, return NULL
    if (pList->currNode == pList->first){
        pList->currNode = NULL;
        pList->currPtrStatus = LIST_OOB_START;
        return NULL;
    }
    pList->currNode = pList->currNode->pre;
    return pList->currNode->nodeData->data;
}

// Returns a pointer to the current item in pList.
void* List_curr(List* pList){
    if (pList == NULL){
        return NULL;
    }
    if (pList->currNode != NULL){
        return pList->currNode->nodeData->data;
    }
    // If currNode is NULL, then the currNode is out of bounds. Return NULL.
    return NULL;

}

// Adds the new item to pList directly after the current item, and makes item the current item. 
// If the current pointer is before the start of the pList, the item is added at the start. If 
// the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_insert_after(List* pList, void* pItem){
    if (pList == NULL){
        return LIST_FAIL;
    }
    if (numNodes >= LIST_MAX_NUM_NODES){
        return LIST_FAIL;
    }       
    if (pList->currNode != NULL){  // pList->currNode is not NULL
        if (pList->currNode == pList->last){ 
            pList->currNode = (List*)malloc(sizeof(List)); 
            pList->currNode->nodeData = (Node*)malloc(sizeof(Node));
            pList->currNode->nodeData->data = pItem;
            pList->currNode->pre = pList->last;
            pList->currNode->next = NULL;
            pList->last->next = pList->currNode;
            pList->last = pList->currNode;
            nodes[numNodes] = pList->currNode->nodeData;
        }else{
            List* temp = (List*)malloc(sizeof(List));
            temp->nodeData = (Node*)malloc(sizeof(Node));
            temp->nodeData->data = pItem;
            pList->currNode->next->pre = temp;
            temp->next = pList->currNode->next;
            temp->pre = pList->currNode;
            pList->currNode->next = temp;
            pList->currNode = temp;
            nodes[numNodes] = pList->currNode->nodeData;
        }
    }
    if (pList->currNode == NULL){
        // the list is empty (pList->first == NULL)
        if (pList->first == NULL) {
            pList->first = (List*)malloc(sizeof(List)); 
            pList->first->nodeData = (Node*)malloc(sizeof(Node));
            pList->first->nodeData->data = pItem;
            pList->first->pre = NULL;
            pList->first->next = NULL;
            pList->last = pList->first;
            pList->currNode = pList->first;
            nodes[numNodes] = pList->currNode->nodeData;
        }
        else if (pList->currPtrStatus == LIST_OOB_START){
            pList->currNode = (List*)malloc(sizeof(List)); 
            pList->currNode->nodeData = (Node*)malloc(sizeof(Node));
            pList->currNode->nodeData->data = pItem;
            nodes[numNodes] = pList->currNode->nodeData;
            pList->currNode->next = pList->first;
            pList->currNode->pre = NULL;
            pList->first->pre = pList->currNode;
            pList->first = pList->currNode;
        }
        else if (pList->currPtrStatus == LIST_OOB_END){
            pList->currNode = (List*)malloc(sizeof(List)); 
            pList->currNode->nodeData = (Node*)malloc(sizeof(Node));
            pList->currNode->nodeData->data = pItem;
            nodes[numNodes] = pList->currNode->nodeData;
            pList->currNode->pre = pList->last;
            pList->currNode->next = NULL;
            pList->last->next = pList->currNode;
            pList->last = pList->currNode;
        }
    }
    numNodes++;
    pList->count++;
    return LIST_SUCCESS;
}

// Adds item to pList directly before the current item, and makes the new item the current one. 
// If the current pointer is before the start of the pList, the item is added at the start. 
// If the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_insert_before(List* pList, void* pItem){
    if (pList == NULL){
        return LIST_FAIL;
    }
    if (numNodes >= LIST_MAX_NUM_NODES){
        return LIST_FAIL;
    }
    if (pList->currNode != NULL){  // pList->currNode is not NULL
        if (pList->currNode == pList->first){
            pList->currNode = (List*)malloc(sizeof(List)); 
            pList->currNode->nodeData = (Node*)malloc(sizeof(Node));
            pList->currNode->nodeData->data = pItem;
            pList->currNode->next = pList->first;
            pList->currNode->pre = NULL;
            pList->first->pre = pList->currNode;
            pList->first = pList->currNode;
            nodes[numNodes] = pList->currNode->nodeData;
        }else{
            List* temp = (List*)malloc(sizeof(List));
            temp->nodeData = (Node*)malloc(sizeof(Node));
            temp->nodeData->data = pItem;
            pList->currNode->pre->next = temp;
            temp->next = pList->currNode;
            temp->pre = pList->currNode->pre;
            pList->currNode->pre = temp;
            pList->currNode = temp;
            nodes[numNodes] = pList->currNode->nodeData;
        }
    }
    if (pList->currNode == NULL){
        // the list is empty (pList->first == NULL)
        if (pList->first == NULL) {
            pList->first = (List*)malloc(sizeof(List)); 
            pList->first->nodeData = (Node*)malloc(sizeof(Node));
            pList->first->nodeData->data = pItem;
            pList->first->pre = NULL;
            pList->first->next = NULL;
            pList->last = pList->first;
            pList->currNode = pList->first;
            nodes[numNodes] = pList->currNode->nodeData;
        }
        else if (pList->currPtrStatus == LIST_OOB_START){
            pList->currNode = (List*)malloc(sizeof(List)); 
            pList->currNode->nodeData = (Node*)malloc(sizeof(Node));
            pList->currNode->nodeData->data = pItem;
            nodes[numNodes] = pList->currNode->nodeData;
            pList->currNode->next = pList->first;
            pList->currNode->pre = NULL;
            pList->first->pre = pList->currNode;
            pList->first = pList->currNode;
        }
        else if (pList->currPtrStatus == LIST_OOB_END){
            pList->currNode = (List*)malloc(sizeof(List)); 
            pList->currNode->nodeData = (Node*)malloc(sizeof(Node));
            pList->currNode->nodeData->data = pItem;
            nodes[numNodes] = pList->currNode->nodeData;
            pList->currNode->pre = pList->last;
            pList->currNode->next = NULL;
            pList->last->next = pList->currNode;
            pList->last = pList->currNode;
        }
    }
    numNodes++;
    pList->count++;
    return LIST_SUCCESS;
}

// Adds item to the end of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_append(List* pList, void* pItem){
    if (pList == NULL){
        return LIST_FAIL;
    }
    if (numNodes >= LIST_MAX_NUM_NODES){
        return LIST_FAIL;
    }
    // the list is empty (pList->first == NULL)
    if (pList->first == NULL) {
        pList->first = (List*)malloc(sizeof(List)); 
        pList->first->nodeData = (Node*)malloc(sizeof(Node));
        pList->first->nodeData->data = pItem;
        pList->first->pre = NULL;
        pList->first->next = NULL;
        pList->last = pList->first;
        pList->currNode = pList->first;
        nodes[numNodes] = pList->currNode->nodeData;
    }else {
        pList->currNode = (List*)malloc(sizeof(List)); 
        pList->currNode->nodeData = (Node*)malloc(sizeof(Node));
        pList->currNode->nodeData->data = pItem;
        pList->currNode->pre = pList->last;
        pList->currNode->next = NULL;
        pList->last->next = pList->currNode;
        pList->last = pList->currNode;
        nodes[numNodes] = pList->currNode->nodeData;
    }
    numNodes++;
    pList->count++;
    return LIST_SUCCESS;
}

// Adds item to the front of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_prepend(List* pList, void* pItem){
    if (pList == NULL){
        return LIST_FAIL;
    }
    if (numNodes >= LIST_MAX_NUM_NODES){
        return LIST_FAIL;
    }
    // the list is empty (pList->first == NULL)
    if (pList->first == NULL) {
        pList->first = (List*)malloc(sizeof(List)); 
        pList->first->nodeData = (Node*)malloc(sizeof(Node));
        pList->first->nodeData->data = pItem;
        pList->first->pre = NULL;
        pList->first->next = NULL;
        pList->last = pList->first;
        pList->currNode = pList->first;
        nodes[numNodes] = pList->currNode->nodeData;
    }else {
        pList->currNode = (List*)malloc(sizeof(List)); 
        pList->currNode->nodeData = (Node*)malloc(sizeof(Node));
        pList->currNode->nodeData->data = pItem;
        pList->currNode->next = pList->first;
        pList->currNode->pre = NULL;
        pList->first->pre = pList->currNode;
        pList->first = pList->currNode;
        nodes[numNodes] = pList->currNode->nodeData;
    }
    numNodes++;
    pList->count++;
    return LIST_SUCCESS;
}

// Return current item and take it out of pList. Make the next item the current one.
// If the current pointer is before the start of the pList, or beyond the end of the pList,
// then do not change the pList and return NULL.
void* List_remove(List* pList){
    if (pList == NULL){
        return NULL;
    }
    // If pList->currNode is NULL then 
    // pList->currPtrStatus is LIST_OOB_START or pList->currPtrStatus is LIST_OOB_END
    // return NULL;
    if (pList->currNode == NULL){
        return NULL;
    }else{
        // Use a temp pointer to store the currNode
        List* temp = pList->currNode;
        // if the list contains only one item
        if (pList->currNode == pList->last && pList->first == pList->last){
            pList->currPtrStatus = LIST_OOB_END;
            pList->currNode = NULL;
            pList->first = NULL;
            pList->last = NULL;
        // if pList->currNode points to pList->last
        }else if (pList->currNode == pList->last){
            pList->currPtrStatus = LIST_OOB_END;
            pList->last = pList->currNode->pre;
            pList->last->next = NULL;
            pList->currNode->pre = NULL;
            pList->currNode = NULL;
        // if pList->currNode points to pList->first
        }else if (pList->currNode == pList->first){
            pList->currNode = pList->currNode->next;
            pList->currNode->pre = NULL;
            temp->next = NULL;
            pList->first = pList->currNode;
        }else{
            pList->currNode = pList->currNode->next;
            temp->pre->next = pList->currNode;
            pList->currNode->pre = temp->pre;
            temp->pre = NULL;
            temp->next = NULL;
        }
        return temp->nodeData->data;
    }
}

// Return last item and take it out of pList. Make the new last item the current one.
// Return NULL if pList is initially empty.
void* List_trim(List* pList){
    if (pList == NULL){
        return NULL;
    }
    if (pList->first == NULL){
        return NULL;
    }
    if (pList->first == pList->last){
        pList->currPtrStatus = LIST_OOB_END;
        List* temp = pList->last;
        pList->currNode = NULL;
        pList->first = NULL;
        pList->last = NULL;
        pList->pre = NULL;
        pList->next = NULL;
        return temp;
    }else {
        List* temp = pList->last;
        pList->currNode = pList->last->pre;
        pList->last->pre = NULL;
        pList->currNode->next = NULL;
        pList->last = pList->currNode;
        return temp;
    }
}

// Adds pList2 to the end of pList1. The current pointer is set to the current pointer of pList1. 
// pList2 no longer exists after the operation; its head is available for future operations.
void List_concat(List* pList1, List* pList2){
    if (pList2 == NULL || pList2->first == NULL){
        return;
    }
    // Assume pList1 will never be NULL
    pList1->last->next = pList2->first;
    pList2->first->pre = pList1->last;
    pList1->last = pList2->last;
    free(pList2);
}

// pList and all its nodes no longer exists after the operation; its head and nodes are 
// available for future operations.
typedef void (*FREE_FN)(void* pItem);
void List_free(List* pList, FREE_FN pItemFreeFn){
    if (pList == NULL){
        return;
    }
    List* p = pList->first;
     while (p != NULL){
        List* temp = p;
        p = p->next;
        (*pItemFreeFn)(temp->nodeData);
        free(temp);
        // temp = NULL;
     }
    free(pList);
    //pList = NULL;
}

// If a match is found, the current pointer is left at the matched item and the pointer to 
// that item is returned. If no match is found, the current pointer is left beyond the end of 
// the list and a NULL pointer is returned.
// If the current pointer is before the start of the pList, then start searching from
// the first node in the list (if any).
typedef bool (*COMPARATOR_FN)(void* pItem, void* pComparisonArg);
void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg){
    if (pList == NULL){
        return NULL;
    }
    List* temp = NULL;
    if (pList->currNode == NULL){
        temp = pList->first;
    }else{
        temp = pList->currNode;
    }
    while (temp != NULL){
        if ((*pComparator)(temp->nodeData->data, pComparisonArg)){
            pList->currNode = temp;
            return pList->currNode->nodeData->data;
        }
        temp = temp->next;
    }
    pList->currNode = NULL;
    pList->currPtrStatus = LIST_OOB_END;
    return NULL;
}
