#include "common.h"

void th09decode(char *file, unsigned char * buffer, unsigned int flength) {
	char frec[50];
	char fraw[50];
	unsigned char *rawdata = &buffer[0xc0], *decodedata;
	unsigned int i, length, dlength, rlength, checksum;
	unsigned char base;
	base = *((unsigned char*) (&buffer[0x15]));
	length = *((unsigned int*) (&buffer[0x0c]));
	sprintf(frec, "%s.txt", file);
	sprintf(fraw, "%s.raw", file);
	for (i = 24; i < length; ++i) {
		buffer[i] -= base;
		base += 7;
	}
    
	dlength = *((unsigned int*) (&buffer[0x1c]));
	decodedata = new unsigned char[dlength];
    printf("%d %d", dlength, length);
	rlength = decompress(rawdata, decodedata, length - 0xc0);
	FILE *fpraw = fopen(fraw, "wb");
	fwrite(decodedata, rlength, 1, fpraw);
	fclose(fpraw);
	printf("Decompression done.");

	delete[] decodedata;
}