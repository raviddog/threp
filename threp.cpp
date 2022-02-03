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

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Usage : %s [filename]", argv[0]);
		return 0;
	}

	for(int i = 1; i < argc; i++) {
		char * file = argv[i];
		FILE * fp;
		fp = fopen(file, "rb");
		if (!fp) {
			printf("%s not found.\n", file);
			return 0;
		}
		unsigned int flength;
		fseek(fp, 0L, SEEK_END);
		flength = ftell(fp);
		fseek(fp, 0L, SEEK_SET);
		unsigned char *buf = new unsigned char[flength];
		fread(buf, flength, 1, fp);
		fclose(fp);

		unsigned char **buffer = &buf;
		int magic = *(int*) buf;

		switch(magic) {
			case 0x50523654:  //"T6RP"
				flength = th06decode(buffer, flength);
				break;
			case 0x50523754:  //"T7RP"
				flength = th07decode(buffer, flength);			
				break;
			case 0x50523854:  //"T8RP"
				flength = th08decode(buffer, flength);
				break;
			case 0x50523954: 	//"T9RP"
				flength = th09decode(buffer, flength);
				break;
			case 0x72303174:  //"t10r"
				flength = th10decode(buffer, flength);
				break;
			case 0x72313174:  //"t11r"
				flength = th11decode(buffer, flength);
				break;
			case 0x72323174:  //"t12r"
				flength = th12decode(buffer, flength);
				break;
			case 0x35323174:  //"t125"
				flength = th125decode(buffer, flength);
				break;
			case 0x72383231:  //"128r"
				flength = th128decode(buffer, flength);
				break;
			case 0x72333174: 	//"t13r"
			case 0x72353174: 	//"t15r"
			case 0x72363174: 	//"t16r"
			case 0x72373174: 	//"t17r"
			case 0x72383174: 	//"t18r"
				flength = th13decode(buffer, flength);
				break;
			default:
				printf("%s not supported format.\n", file);
				delete[] buf;
				return 0;
				break;
		}

		buf = *buffer;
		int nl = strlen(file);
		//	rename extension to .raw
		file[nl - 3] = 'r';
		file[nl - 2] = 'a';
		file[nl - 1] = 'w';
		fp = fopen(file, "wb");
		if(!fp) {
			printf("couldnt open output file %s for writing\n", file);
		} else {
			fwrite(*buffer, flength, 1, fp);
			fclose(fp);
		}
		delete[] buf;
	}

	return 0;
}
