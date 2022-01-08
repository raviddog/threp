#include "common.h"

unsigned int th09decode(unsigned char **buffer, unsigned int flength) {
	unsigned char *buf = *buffer;
	unsigned char *rawdata = &buf[0xc0], *decodedata;
	unsigned int i, length, dlength, rlength, checksum;
	unsigned char base;
	base = *((unsigned char*) (buf[0x15]));
	length = *((unsigned int*) (buf[0x0c]));
	for (i = 24; i < length; ++i) {
		buf[i] -= base;
		base += 7;
	}
    
	dlength = *((unsigned int*) (&buf[0x1c]));
	decodedata = new unsigned char[dlength];
	rlength = decompress(rawdata, decodedata, length - 0xc0);

	delete[] buf;
	*buffer = decodedata;
	return dlength;
}