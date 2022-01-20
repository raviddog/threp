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

unsigned int get_bit(unsigned char* buffer, unsigned int &pointer, unsigned char &filter, unsigned char length);
unsigned int decompress(unsigned char * buffer, unsigned char * decode, unsigned int length);
void decode(unsigned char * buffer, int length, int block_size, unsigned char base, unsigned char add);


#endif /* COMMON_H_ */
