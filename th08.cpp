/*
 * th08.cpp
 *
 *  Created on: 2010-5-9
 *      Author: Argon
 */

#include "common.h"

unsigned int th08decode(unsigned char **buffer, unsigned int flength) {
	unsigned char *rawdata = buffer[0x68], *decodedata;
	unsigned int i, length, dlength, rlength;
	unsigned char base;
	base = *((unsigned char*) (buffer[0x15]));
	length = *((unsigned int*) (buffer[0x0c]));
	for (i = 24; i < length; ++i) {
		*buffer[i] -= base;
		base += 7;
	}
	// checksum = 0x3f000318;
	// for (i = 21; i < length; ++i)
	// 	checksum += *buffer[i];
	dlength = *((unsigned int*) (buffer[0x1c]));
	decodedata = new unsigned char[dlength];
	rlength = decompress(rawdata, decodedata, length - 0x68);

	delete[] buffer;
	*buffer = decodedata;
	return dlength;
}
