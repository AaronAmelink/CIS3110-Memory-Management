#include "mmanager.h"


int allocateMemoryFirst(int size, int id, Node** head, void* startOfMemory, int memorySize, char paint) {
    Node* temp = (*head);
    Node* inserted = NULL;

    if (*head == NULL) {
        inserted = insertNode(startOfMemory, id, size, head, paint);
        return (inserted == NULL ? -1 : 1);
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

    if (inserted == NULL) {
        return -1;
    } else {
        return 1;
    }

}

int allocateMemoryBest(int size, int id, Node** head, void* startOfMemory, int memorySize, char paint) {

    if (*head == NULL) {
        Node* inserted = insertNode(startOfMemory, id, size, head, paint);
        return (inserted == NULL ? -1 : 1);
    }

    int minDifference = 10000000; //based on first item. i.e. if smallest size dif is between 2/3 node, minIndex would be 2
    int minIndex = 0;
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
            if (temp->next == NULL && sizeDif < 0) {
                return -1; //no available slot at all
            }
            temp = temp->next;
            index++;
        }


        
    }

    if (minIndex == -1) {
        insertNode(startOfMemory, id, size, head, paint);
        return 1;
    }
    
    index = 0;
    temp = (*head);

    while(index != minIndex) {
        temp = temp->next;
        index++;
    }

    insertNode(temp->location + temp->size + sizeof(Node), id, size, head, paint);
    return 1;
}

int allocateMemoryWorst(int size, int id, Node** head, void* startOfMemory, int memorySize, char paint) {
    
    if (*head == NULL) {
        Node* inserted = insertNode(startOfMemory, id, size, head, paint);
        return (inserted == NULL ? -1 : 1);
    }

    int maxDifference = 0; //based on first item. i.e. if smallest size dif is between 2/3 node, minIndex would be 2
    int maxIndex = 0;
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
            if (temp->next == NULL && sizeDif < 0) {
                return -1; //no available slot at all
            }
            temp = temp->next;
            index++;
        }


        
    }

    if (maxIndex == -1) {
        insertNode(startOfMemory, id, size, head, paint);
        return 1;
    }
    
    index = 0;
    temp = (*head);

    while(index != maxIndex) {
        temp = temp->next;
        index++;
    }

    insertNode(temp->location + temp->size + sizeof(Node), id, size, head, paint);
    return 1;
}