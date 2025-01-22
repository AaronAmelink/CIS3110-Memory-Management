//#include "mmanager.h"
#include <stdlib.h>
#include <stdio.h>
typedef struct node Node;

struct node {
    Node* next;
    Node* last;
    void* location;
    int size;
    int id;
};

int memdbg_dump_map(FILE* fp, void *base, void *start, size_t nBytesToPrint, int indent);

/*
 * Hamilton-Wright, Andrew (2011)
 *
 * Memory Map Block Inspection
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define	HALF_BYTES_PER_LINE	8
#define	BYTES_PER_LINE	(HALF_BYTES_PER_LINE * 2)


int
memdbg_dump_map(FILE* fp,
		void *base, void *start,
		size_t nBytesToPrint, int indent)
{
	unsigned char *cBase, *cStart;
	unsigned char buffer[BYTES_PER_LINE];
	size_t bytesThisLine, bytesRemain;
	size_t i, j;

	cBase = (unsigned char *) base;
	cStart = (unsigned char *) start;

	i = 0;
	while (i < nBytesToPrint)
	{
		bytesRemain = nBytesToPrint - i;
		/** figure out how many bytes to print */
		bytesThisLine = (bytesRemain < BYTES_PER_LINE)
					? bytesRemain : BYTES_PER_LINE;

		/** copy the data into our working buffer */
		memcpy(buffer, &cStart[i], bytesThisLine);

		fprintf(fp, "%*s", indent, "");

		fprintf(fp, "0x%04lx", ((unsigned long) (cStart - cBase)) + i);

		/** print the hex values */
		for (j = 0; j < bytesThisLine; j++)
		{
			if (j == HALF_BYTES_PER_LINE)
				fprintf(fp, " ");
			fprintf(fp, " %02x", cStart[i+j]);
		}

		/** pad if we are short */
		for ( ; j < BYTES_PER_LINE; j++)
		{
			if (j == HALF_BYTES_PER_LINE)
				fprintf(fp, " ");
			fprintf(fp, "   ");
		}

		/** print as chars */
		fprintf(fp, " ");
		for (j = 0; j < bytesThisLine; j++)
			fprintf(fp, "%c", isprint(cStart[i+j]) ? cStart[i+j] : '.');

		fprintf(fp, "\n");

		/** update i by the amount we have printed */
		i += bytesThisLine;
	}

	if (ferror(fp)) return -1;

	return 1;
}



Node* insertNode(void* location, int id, int size, Node** headptr, char paint) {
    Node* temp = headptr == NULL ? NULL : (*headptr);
    if (temp != NULL) {
        while (temp->next != NULL && (temp->location + temp->size + sizeof(Node)) < location) {
            if (temp->next->location > location) {
                break;
            }
            temp = temp->next;
        }

        if (location + size + sizeof(Node) >= temp->location && (*headptr) == temp) {
            return NULL;
        }

        if (temp->location <= location && temp->location + temp->size + sizeof(Node) >= location) {
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

void removeNode(int id, Node** head) {
    Node* temp = *head;
    Node* headptr = *head;
    while (temp->id != id && temp != NULL) {
        temp = temp->next;
    }

    char* i = (char*)temp->location + sizeof(Node);
    char* end = (char*)(temp->location + temp->size + sizeof(Node));
    while (i < end) {
        *i = 'R';
        i++;
    }

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
}

int main() {
    void* memory = malloc(200);
    Node* head = insertNode(memory + 10 + sizeof(Node), 1, 100, NULL, 'b');
    printf("\n\n");
    memdbg_dump_map(stdout, NULL, memory, 200, 1);

    return 1;
}