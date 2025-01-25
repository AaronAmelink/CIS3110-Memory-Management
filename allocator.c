#include "mmanager.h"


Node* allocateMemoryFirst(int size, int id, Node** head, void* startOfMemory, int memorySize, char paint) {
    Node* temp = (*head);
    Node* inserted = NULL;

    if (*head == NULL) {
        inserted = insertNode(startOfMemory, id, size, head, paint);
        return inserted;
    }

    while (temp->next != NULL) {
        if (temp->location + temp->size + 2*sizeof(Node) + size < temp->next->location) {
            break;
        }
        temp = temp->next;
    }
    
    if (temp->location + temp->size + 2*sizeof(Node) + size < startOfMemory + memorySize) {
        inserted = insertNode(temp->location+temp->size+sizeof(Node), id, size, head, paint);
    }

    return inserted;

}

Node* allocateMemoryBest(int size, int id, Node** head, void* startOfMemory, int memorySize, char paint) {
    Node* inserted = NULL;
    if (*head == NULL) {
        inserted = insertNode(startOfMemory, id, size, head, paint);
        return inserted;
    }

    int minDifference = 10000000; //based on first item. i.e. if smallest size dif is between 2/3 node, minIndex would be 2
    int minIndex = -1;
    int index = -1;
    int sizeDif;
    Node* temp = (*head);
    while (temp != NULL)
    {
        if (index == -1) {
            sizeDif = temp->location - (startOfMemory) - (size + sizeof(Node));
            if (sizeDif >= 0 && sizeDif < minDifference) {
                minDifference = sizeDif;
                minIndex = index;
            }
            index++;

        } else {
            sizeDif = (temp->next == NULL ? startOfMemory + memorySize : temp->next->location) - (temp->location + temp->size + sizeof(Node)) - (size + sizeof(Node));
            if (sizeDif >= 0 && sizeDif < minDifference) {
                minDifference = sizeDif;
                minIndex = index;
            }
            if (temp->next == NULL && minIndex == -1) {
                return NULL; //no available slot at all
            }
            temp = temp->next;
            index++;
        }


        
    }

    if (minIndex == -1) {
        inserted = insertNode(startOfMemory, id, size, head, paint);
        return inserted;
    }
    
    index = 0;
    temp = (*head);

    while(index != minIndex) {
        temp = temp->next;
        index++;
    }

    inserted = insertNode(temp->location + temp->size + sizeof(Node), id, size, head, paint);
    return inserted;
}

Node* allocateMemoryWorst(int size, int id, Node** head, void* startOfMemory, int memorySize, char paint) {
    
    Node* inserted = NULL;
    if (*head == NULL) {
        inserted = insertNode(startOfMemory, id, size, head, paint);
        return inserted;
    }

    int maxDifference = 0; //based on first item. i.e. if smallest size dif is between 2/3 node, minIndex would be 2
    int maxIndex = -1;
    int index = -1;
    int sizeDif;
    Node* temp = (*head);
    while (temp != NULL)
    {
        if (index == -1) {
            sizeDif = temp->location - (startOfMemory) - (size + sizeof(Node));
            if (sizeDif >= 0 && sizeDif > maxDifference) {
                maxDifference = sizeDif;
                maxIndex = index;
            }
            index++;

        } else {
            sizeDif = (temp->next == NULL ? startOfMemory + memorySize : temp->next->location) - (temp->location + temp->size + sizeof(Node)) - (size + sizeof(Node));
            if (sizeDif >= 0 && sizeDif > maxDifference) {
                maxDifference = sizeDif;
                maxIndex = index;
            }
            if (temp->next == NULL && maxIndex == -1) {
                return NULL; //no available slot at all
            }
            temp = temp->next;
            index++;
        }


        
    }

    if (maxIndex == -1) {
        inserted = insertNode(startOfMemory, id, size, head, paint);
        return inserted;
    }
    
    index = 0;
    temp = (*head);

    while(index != maxIndex) {
        temp = temp->next;
        index++;
    }

    inserted = insertNode(temp->location + temp->size + sizeof(Node), id, size, head, paint);
    return inserted;
}