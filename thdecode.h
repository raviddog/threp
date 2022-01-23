#ifndef THDECODE_H
#define THDECODE_H

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
unsigned int th09decode1(unsigned char **buffer, unsigned int flength);
unsigned int th09decode2(unsigned char **buffer, unsigned int flength);

#endif