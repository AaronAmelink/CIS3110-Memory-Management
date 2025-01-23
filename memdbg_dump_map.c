#include "mmanager.h"
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