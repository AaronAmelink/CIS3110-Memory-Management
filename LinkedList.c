#include "mmanager.h"


Node* insertNode(void* location, int id, int size, Node** headptr, char paint) {
    Node* temp = headptr == NULL ? NULL : (*headptr);
    if (temp != NULL) {
        while (temp->next != NULL && (temp->location + temp->size + sizeof(Node)) < location) {
            if (temp->next->location > location) {
                break;
            }
            temp = temp->next;
        }

        if (location + size + sizeof(Node) > temp->location && (*headptr) == temp && location < temp->location) {
            return NULL;
        }

        if (location >= temp->location && temp->location + temp->size + sizeof(Node) > location) {
            //inside current allocated block
            return NULL;
        }

        if (temp->next != NULL && location + size + sizeof(Node) > temp->next->location) {
            //inside next allocated block
            return NULL;
        }
    }


    Node* newNode = (Node*)location;
    newNode->location = location; // any time comparing the actual blocks, must add sizeof node to this
    newNode->size = size; //NOT including node size
    newNode->id = id;
    newNode->last = temp;

    if (paint != ' ') {
        char* i = (char*)location + sizeof(Node);
        char* end = (char*)(location + size + sizeof(Node));
        while (i < end) {
            *i = paint;
            i++;
        }
    }

    if (temp == NULL) {
        (*headptr) = newNode;
        return newNode;
    }
    if (location < (*headptr)->location) {
        //maintain ordering
        newNode->next = (*headptr);
        newNode->next->last = newNode;
        (*headptr) = newNode;
        newNode->last = NULL;
        return newNode;
    }
    if (temp->next == NULL) {
        newNode->next = NULL;
        temp->next = newNode;
    } else {
        newNode->next = temp->next;
        temp->next->last = newNode;
        temp->next = newNode;
    }

    return newNode;
}

int removeNode(int id, Node** head) {
    Node* temp = *head;
    Node* headptr = *head;
    while (temp->id != id) {
        if (temp->next == NULL) {
            return -1;
        }
        temp = temp->next;
    }

    char* i = (char*)temp->location;
    char* end = (char*)(temp->location + temp->size + sizeof(Node));



    if (temp->next != NULL) {
        if (headptr == temp) {
            *head = temp->next;
            temp->next->last = NULL;
        } else {
            temp->last->next = temp->next;
        }
    } else {
        if (headptr == temp) {
            *head = NULL;
        } else {
            temp->last->next = NULL;
        }

    }

    while (i < end) {
        *i = 0;
        i++;
    }
    return 1;
}