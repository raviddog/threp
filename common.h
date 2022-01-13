/*
 * common.h
 *
 *  Created on: 2010-5-9
 *      Author: Argon
 */

#ifndef COMMON_H_
#define COMMON_H_

// #include <io.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <memory.h>

unsigned int th06decode(unsigned char **buffer, unsigned int flength);
unsigned int th07decode(unsigned char **buffer, unsigned int flength);
unsigned int th08decode(unsigned char **buffer, unsigned int flength);
unsigned int th09decode(unsigned char **buffer, unsigned int flength);
unsigned int th10decode(unsigned char **buffer, unsigned int flength);
unsigned int th11decode(unsigned char **buffer, unsigned int flength);
unsigned int th12decode(unsigned char **buffer, unsigned int flength);
unsigned int th125decode(unsigned char **buffer, unsigned int flength);
unsigned int th128decode(unsigned char **buffer, unsigned int flength);
unsigned int th13decode(unsigned char **buffer, unsigned int flength);

unsigned int th07decode1(unsigned char **buffer, unsigned int flength);
unsigned int th07decode2(unsigned char **buffer, unsigned int flength);
unsigned int th08decode1(unsigned char **buffer, unsigned int flength);
unsigned int th08decode2(unsigned char **buffer, unsigned int flength);

void th06encode(unsigned char * buffer, unsigned int flength);

unsigned int get_bit(unsigned char* buffer, unsigned int &pointer, unsigned char &filter, unsigned char length);
unsigned int decompress(unsigned char * buffer, unsigned char * decode, unsigned int length);
void decode(unsigned char * buffer, int length, int block_size, unsigned char base, unsigned char add);


#endif /* COMMON_H_ */
