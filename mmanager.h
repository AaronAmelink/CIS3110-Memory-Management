#ifndef	MEM_MANANGER_HEADER
#define	MEM_MANANGER_HEADER

#include <stdlib.h>
#include <stdio.h>

#define		DEFAULT_MEMORY_SIZE		4096

#define		STRAT_FIRST		0x01
#define		STRAT_BEST		0x02
#define		STRAT_WORST		0x03

#define		ACTION_ALLOCATE	0x10
#define		ACTION_RELEASE	0x20

typedef struct mmgr_action {
	int type;
	int id;
	int size;
	char paint;
} mmgr_action;

typedef struct node Node;

struct node {
	Node* next;
	Node* last;
	void* location;
	int size;
	int id;
};

int runModel(FILE *outputfp, FILE *inputfp,
		long numberOfBytes, int fitStrategy,
		int verbosity);

Node* insertNode(void* location, int id, int size, Node** headptr, char paint);

void* removeNode(int id, Node** head);

int getAction(mmgr_action *action, FILE *inputfp,
		FILE *outputfp, int verbosity);
int printAction(FILE *outputfp, mmgr_action *action);

Node* allocateMemoryFirst(int size, int id, Node** head, void* startOfMemory, int memorySize, char paint);

Node*  allocateMemoryBest(int size, int id, Node** head, void* startOfMemory, int memorySize, char paint);

Node*  allocateMemoryWorst(int size, int id, Node** head, void* startOfMemory, int memorySize, char paint);

int
memdbg_dump_map(FILE* fp,
		void *base, void *start,
		size_t nBytesToPrint, int indent);

#endif

