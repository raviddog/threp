/*
 * th10.cpp
 *
 *  Created on: 2010-5-9
 *      Author: Argon
 */

#include "common.h"

unsigned int th10decode(unsigned char **buffer, unsigned int flength) {
	unsigned int rlength;
	unsigned char *rawdata = buffer[0x24], *decodedata;
	unsigned int length = *((unsigned int*) (buffer[0x1c]));
	unsigned int dlength = *((unsigned int*) (buffer[0x20]));

	decodedata = new unsigned char[dlength];
	decode(rawdata, length, 0x400, 0xaa, 0xe1);
	decode(rawdata, length, 0x80, 0x3d, 0x7a);
	rlength = decompress(rawdata, decodedata, length);

	delete[] buffer;
	*buffer = decodedata;
	return dlength;
}
