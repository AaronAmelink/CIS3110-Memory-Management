#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "mmanager.h"

int runModel(FILE *outputfp, FILE *inputfp,
		long totalMemorySize, int fitStrategy,
		int verbosity
	)
{
	char *memoryBlock = NULL;
	int nSuccessfulActions = 0;
	mmgr_action action;

	fprintf(outputfp,
			"Running a %s-fit model in %ld (0x%lx) bytes of memory.\n",
			(fitStrategy == STRAT_FIRST ? "first" :
				fitStrategy == STRAT_BEST ? "best" :
					fitStrategy == STRAT_WORST ? "worst" : "unknown"),
			totalMemorySize, totalMemorySize);

	/**
	 * this is the only allocation you should need -- all requests
	 * from your model should come from this allocated block
	 */
	memoryBlock = (char *) malloc(totalMemorySize);
	if (memoryBlock == NULL) {
		perror("allocation failed!");
		return -1;
	}

	/**
	 *	+++ Set up anything else you will need for your memory management
	 */

	//head size denotes the size of the linked list

	Node* head = NULL;
	Node* inserted = NULL;
	void* removed = NULL;
	while (getAction(&action, inputfp, outputfp, verbosity) > 0)
	{
		inserted = NULL;
		removed = NULL;
		if (action.type == ACTION_ALLOCATE)
		{
			if (fitStrategy == STRAT_FIRST) {
				inserted = allocateMemoryFirst(action.size, action.id, &head, memoryBlock, totalMemorySize, action.paint);
			} else if (fitStrategy == STRAT_WORST) {
				inserted = allocateMemoryWorst(action.size, action.id, &head, memoryBlock, totalMemorySize, action.paint);
			} else if (fitStrategy == STRAT_BEST) {
				inserted = allocateMemoryBest(action.size, action.id, &head, memoryBlock, totalMemorySize, action.paint);
			}
			printf("ALLOCATE %d (+%ld) bytes (ID %d)", action.size, sizeof(Node), action.id);
			switch (verbosity) {
				case 2:
					if (inserted == NULL) {
						printf(": FAIL\n");
						memdbg_dump_map(stdout, NULL, memoryBlock, totalMemorySize, 1);
					} else {
						printf(": ALLOCATE SUCCESS\n");
						memdbg_dump_map(stdout, NULL, inserted->location, 500, 1);
					}
					printf("\n");
					break;
				case 1:
					if (inserted != NULL) {
						printf(" - return location %ld", inserted->location-(void*)memoryBlock);
					}
				default:
					if (inserted == NULL) {
						printf(": FAIL");
					} else {
						printf(": ALLOCATE SUCCESS");
					}
					break;
			}
			printf("\n");

		} else
		{
			removed = removeNode(action.id, &head);
			if (removed == NULL) {
				printf("COULD NOT FREE - %d\n", action.id);
				continue;
			}
			switch (verbosity)
			{
			case 2:
				memdbg_dump_map(stdout, NULL, memoryBlock, 500, 1);
				printf("\n");
			default:
				printf("FREE - location %ld (ID %d)", removed-(void*)memoryBlock, action.id);
				break;
			}
			printf("\n");
		}

		/** increment our count of work we did */
		nSuccessfulActions++;
	}
	printf("-----------------\n");
	int count = 0;
	Node* temp = head;
	while (temp != NULL)
	{
		printf("Chunk %d - ID %d: %ld-%ld ~ %d bytes\n", count,temp->id, temp->location-(void*)memoryBlock, temp->location+temp->size+sizeof(Node)-(void*)memoryBlock, temp->size);
		temp = temp->next;
		count++;
	}
	
	free(memoryBlock);


	return nSuccessfulActions;
}

