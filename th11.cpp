/*
 * th11.cpp
 *
 *  Created on: 2010-5-9
 *      Author: Argon
 */

#include "common.h"

unsigned int th11decode(unsigned char **buffer, unsigned int flength) {
	unsigned char *buf = *buffer;
	unsigned int rlength;
	unsigned char *rawdata = &buf[0x24], *decodedata;
	unsigned int length = *((unsigned int*) (buf[0x1c]));
	unsigned int dlength = *((unsigned int*) (buf[0x20]));

	decodedata = new unsigned char[dlength];
	decode(rawdata, length, 0x800, 0xaa, 0xe1);
	decode(rawdata, length, 0x40, 0x3d, 0x7a);
	rlength = decompress(rawdata, decodedata, length);

	delete[] buf;
	*buffer = decodedata;
	return dlength;
}
