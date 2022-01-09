/*
 * th13.cpp
 *
 *  Created on: 2011-4-16
 *      Author: Argon
 */

#include "common.h"

unsigned int th13decode(unsigned char **buffer, unsigned int flength) {
	unsigned char *buf = *buffer;
	unsigned int rlength;
	unsigned char *rawdata = &buf[0x24], *decodedata;
	unsigned int length = *((unsigned int*) (&buf[0x1c]));
	unsigned int dlength = *((unsigned int*) (&buf[0x20]));

	decodedata = new unsigned char[dlength];
	decode(rawdata, length, 0x400, 0x5c, 0xe1);
	decode(rawdata, length, 0x100, 0x7d, 0x3a);
	rlength = decompress(rawdata, decodedata, length);

	delete[] buf;
	*buffer = decodedata;
	return dlength;
}
