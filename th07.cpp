/*
 * th07.cpp
 *
 *  Created on: 2010-5-9
 *      Author: Argon
 */

#include "common.h"

unsigned int th07decode(unsigned char **buffer, unsigned int flength) {
	unsigned char *buf = *buffer;
	unsigned char *rawdata = &buf[0x54], *decodedata;
	unsigned int i, length, dlength, rlength, checksum;
	unsigned char base;
	base = *(unsigned char*) (&buf[0x0d]);
	for (i = 0x10; i < flength; ++i) {
		buf[i] -= base;
		base += 7;
	}
	// checksum = 0x3f000318;
	// for (i = 0x0d; i < flength; ++i)
	// 	checksum += &buf[i];
	length = *((unsigned int*) (&buf[0x14]));
	dlength = *((unsigned int*) (&buf[0x18]));
	decodedata = new unsigned char[dlength];
	rlength = decompress(rawdata, decodedata, length);
	
	delete[] buf;
	*buffer = decodedata;
	return dlength;	

	// return flength;
}

unsigned int th07decode1(unsigned char **buffer, unsigned int flength) {
	unsigned char *buf = *buffer;
	unsigned char base;
	base = *(unsigned char*) (&buf[0x0d]);
	for (unsigned int i = 0x10; i < flength; ++i) {
		buf[i] -= base;
		base += 7;
	}

	return flength;
}

unsigned int th07decode2(unsigned char **buffer, unsigned int flength) {
	unsigned char *buf = *buffer;
	unsigned char *rawdata = &buf[0x54], *decodedata;
	unsigned int length, dlength, rlength, checksum;
	// checksum = 0x3f000318;
	// for (i = 0x0d; i < flength; ++i)
	// 	checksum += &buf[i];
	length = *((unsigned int*) (&buf[0x14]));
	dlength = *((unsigned int*) (&buf[0x18]));
	decodedata = new unsigned char[dlength];
	rlength = decompress(rawdata, decodedata, length);

	delete[] buf;
	*buffer = decodedata;
	return dlength;	
}