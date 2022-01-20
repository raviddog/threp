/*
 * threp.cpp
 *
 *  Created on: 2010-1-3
 *      Author: Argon.Sun
 *      Version: 1.2.8
 */

#include "common.h"
#include "thdecode.h"

#include <cstdio>
#include <cstring>
#include "io.h"

int main(int argc, char *argv[]) {
	unsigned int flength;
	unsigned char *buf = new unsigned char[0x100000];
	unsigned char **buffer = &buf;
	if (argc < 2) {
		printf("Usage : %s [filename]", argv[0]);
		return 0;
	}
	char * file = argv[1];
	FILE * fp;
	fp = fopen(file, "rb");
	if (!fp) {
		printf("%s not found.\n", file);
		return 0;
	}
	fread(buf, 0x100000, 1, fp);
	flength = _filelength(fp->_file);
	fclose(fp);
	if (**(int **) buffer == 0x50523654) { //"T6RP"
		flength = th06decode(buffer, flength);
	} else if (**(int **) buffer == 0x50523754) { //"T7RP"
		flength = th07decode(buffer, flength);
	} else if (**(int **) buffer == 0x50523854) { //"T8RP"
		flength = th08decode(buffer, flength);
	} else if (**(int **) buffer == 0x50523954) {	//"T9RP"
		flength = th09decode(buffer, flength);
	} else if (**(int **) buffer == 0x72303174) { //"t10r"
		flength = th10decode(buffer, flength);
	} else if (**(int **) buffer == 0x72313174) { //"t11r"
		flength = th11decode(buffer, flength);
	} else if (**(int **) buffer == 0x72323174) { //"t12r"
		flength = th12decode(buffer, flength);
	} else if (**(int **) buffer == 0x35323174) { //"t125"
		flength = th125decode(buffer, flength);
	} else if (**(int **) buffer == 0x72383231) { //"128r"
		flength = th128decode(buffer, flength);
	} else if (**(int **) buffer == 0x72333174) { //"t13r"
		flength = th13decode(buffer, flength);
	} else if (**(int **) buffer == 0x72353174) {	//"t15r"
		flength = th13decode(buffer, flength);
	} else if (**(int **) buffer == 0x72363174) {	//"t16r"
		flength = th13decode(buffer, flength);
	} else if (**(int **) buffer == 0x72373174) {	//"t17r"
		flength = th13decode(buffer, flength);
	} else if (**(int **) buffer == 0x72383174) {	//"t18r"
		flength = th13decode(buffer, flength);
	} else {
		printf("not supported format.\n");
		return 0;
	}
	buf = *buffer;
	int nl = strlen(file);
	file[nl - 2] = 'a';
	file[nl - 1] = 'w';
	fp = fopen(file, "wb");
	if(!fp) {
		printf("couldnt open output file for writing\n");
	} else {
		fwrite(*buffer, flength, 1, fp);
		fclose(fp);
	}
	delete[] buf;
	return 0;
}
