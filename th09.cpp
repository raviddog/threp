#include "common.h"

unsigned int th09decode(unsigned char **buffer, unsigned int flength) {
	unsigned char *rawdata = buffer[0xc0], *decodedata;
	unsigned int i, length, dlength, rlength, checksum;
	unsigned char base;
	base = *((unsigned char*) (buffer[0x15]));
	length = *((unsigned int*) (buffer[0x0c]));
	for (i = 24; i < length; ++i) {
		*buffer[i] -= base;
		base += 7;
	}
    
	dlength = *((unsigned int*) (buffer[0x1c]));
	decodedata = new unsigned char[dlength];
	rlength = decompress(rawdata, decodedata, length - 0xc0);

	delete[] buffer;
	*buffer = decodedata;
	return dlength;
}