#include "common.h"
#include "string.h"

unsigned int th06decode(unsigned char **buffer, unsigned int flength) {
	unsigned int i;
	unsigned char base, *buf = *buffer;
	base = *((unsigned char*) (&buf[0x0e]));
	for (i = 0x0f; i < flength; ++i) {
		buf[i] -= base;
		base += 7;
	}
	buffer = &buf;
	return flength;
}

unsigned int th07decode(unsigned char **buffer, unsigned int flength) {
	unsigned char *buf = *buffer;
	unsigned char *rawdata = &buf[0x54], *decodedata;
	unsigned int i, length, dlength;
	unsigned char base;
	base = *(unsigned char*) (&buf[0x0d]);
	for (i = 0x10; i < flength; ++i) {
		buf[i] -= base;
		base += 7;
	}
	//  unsiugned int checksum = 0x3f000318;
	//  for (i = 0x0d; i < flength; ++i)
	// 	    checksum += &buf[i];
	length = *((unsigned int*) (&buf[0x14]));
	dlength = *((unsigned int*) (&buf[0x18]));
	decodedata = new unsigned char[dlength];
	decompress(rawdata, decodedata, length);

	unsigned char *newbuf = new unsigned char[dlength + 0x54];
	memcpy(newbuf, buf, 0x54);
	memcpy(newbuf + 0x54, decodedata, dlength);

	delete[] buf;
	delete[] decodedata;
	*buffer = newbuf;
	return dlength;
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
	unsigned int length, dlength;
	//  unsigned int checksum = 0x3f000318;
	//  for (i = 0x0d; i < flength; ++i)
	// 	    checksum += &buf[i];
	length = *((unsigned int*) (&buf[0x14]));
	dlength = *((unsigned int*) (&buf[0x18]));
	decodedata = new unsigned char[dlength];
	decompress(rawdata, decodedata, length);

	delete[] buf;
	*buffer = decodedata;
	return dlength;	
}

unsigned int th08decode(unsigned char **buffer, unsigned int flength) {
	unsigned char *buf = *buffer;
	unsigned char *rawdata = &buf[0x68], *decodedata;
	unsigned int i, length, dlength;
	unsigned char base;
	base = *((unsigned char*) (&buf[0x15]));
	length = *((unsigned int*) (&buf[0x0c]));
	for (i = 24; i < length; ++i) {
		buf[i] -= base;
		base += 7;
	}
	//  unsigned int checksum = 0x3f000318;
	//  for (i = 21; i < length; ++i)
	// 	    checksum += *buffer[i];
	dlength = *((unsigned int*) (&buf[0x1c]));
	decodedata = new unsigned char[dlength];
	decompress(rawdata, decodedata, length - 0x68);

	unsigned char *newbuf = new unsigned char[dlength + 0x68];
	memcpy(newbuf, buf, 0x68);
	memcpy(newbuf + 0x68, decodedata, dlength);

	delete[] buf;
	delete[] decodedata;
	*buffer = newbuf;
	return dlength;
}

unsigned int th08decode1(unsigned char **buffer, unsigned int flength) {
	unsigned char *buf = *buffer;
	unsigned int length;
	unsigned char base;
	base = *((unsigned char*) (&buf[0x15]));
	length = *((unsigned int*) (&buf[0x0c]));
	for (unsigned int i = 24; i < length; ++i) {
		buf[i] -= base;
		base += 7;
	}
	return flength;
}

unsigned int th08decode2(unsigned char **buffer, unsigned int flength) {
	unsigned char *buf = *buffer;
	unsigned char *rawdata = &buf[0x68], *decodedata;
	unsigned int length, dlength;
	length = *((unsigned int*) (&buf[0x0c]));
	//  unsigned int checksum = 0x3f000318;
	//  for (i = 21; i < length; ++i)
	// 	    checksum += *buffer[i];
	dlength = *((unsigned int*) (&buf[0x1c]));
	decodedata = new unsigned char[dlength];
	decompress(rawdata, decodedata, length - 0x68);

	delete[] buf;
	*buffer = decodedata;
	return dlength;

}

unsigned int th09decode(unsigned char **buffer, unsigned int flength) {
	unsigned char *buf = *buffer;
	unsigned char *rawdata = &buf[0xc0], *decodedata;
	unsigned int i, length, dlength;
	unsigned char base;
	base = *((unsigned char*) (&buf[0x15]));
	length = *((unsigned int*) (&buf[0x0c]));
	for (i = 24; i < length; ++i) {
		buf[i] -= base;
		base += 7;
	}
    
	dlength = *((unsigned int*) (&buf[0x1c]));
	decodedata = new unsigned char[dlength];
	decompress(rawdata, decodedata, length - 0xc0);

	unsigned char *newbuf = new unsigned char[dlength + 0xc0];
	memcpy(newbuf, buf, 0xc0);
	memcpy(newbuf + 0xc0, decodedata, dlength);

	delete[] buf;
	delete[] decodedata;
	*buffer = newbuf;
	return dlength;
}

unsigned int th09decode1(unsigned char **buffer, unsigned int flength) {
	unsigned char *buf = *buffer;
	unsigned int i, length;
	unsigned char base;
	base = *((unsigned char*) (&buf[0x15]));
	length = *((unsigned int*) (&buf[0x0c]));
	for (i = 24; i < length; ++i) {
		buf[i] -= base;
		base += 7;
	}

	return flength;
}

unsigned int th09decode2(unsigned char **buffer, unsigned int flength) {
	unsigned char *buf = *buffer;
	unsigned char *rawdata = &buf[0xc0], *decodedata;
	unsigned int length, dlength;
	length = *((unsigned int*) (&buf[0x0c]));
	dlength = *((unsigned int*) (&buf[0x1c]));
	decodedata = new unsigned char[dlength];
	decompress(rawdata, decodedata, length - 0xc0);

	delete[] buf;
	*buffer = decodedata;
	return dlength;
}

unsigned int th10decode(unsigned char **buffer, unsigned int flength) {
	unsigned char *buf = *buffer;
	unsigned char *rawdata = &buf[0x24], *decodedata;
	unsigned int length = *(unsigned int*) (&buf[0x1c]);
	unsigned int dlength = *(unsigned int*) (&buf[0x20]);

	decodedata = new unsigned char[dlength];
	decode(rawdata, length, 0x400, 0xaa, 0xe1);
	decode(rawdata, length, 0x80, 0x3d, 0x7a);
	decompress(rawdata, decodedata, length);

	delete[] buf;
	*buffer = decodedata;
	return dlength;
}

unsigned int th11decode(unsigned char **buffer, unsigned int flength) {
	unsigned char *buf = *buffer;
	unsigned char *rawdata = &buf[0x24], *decodedata;
	unsigned int length = *((unsigned int*) (&buf[0x1c]));
	unsigned int dlength = *((unsigned int*) (&buf[0x20]));

	decodedata = new unsigned char[dlength];
	decode(rawdata, length, 0x800, 0xaa, 0xe1);
	decode(rawdata, length, 0x40, 0x3d, 0x7a);
	decompress(rawdata, decodedata, length);

	delete[] buf;
	*buffer = decodedata;
	return dlength;
}

unsigned int th12decode( unsigned char **buffer, unsigned int flength) {
	unsigned char *buf = *buffer;
	unsigned char *rawdata = &buf[0x24], *decodedata;
	unsigned int length = *((unsigned int*) (&buf[0x1c]));
	unsigned int dlength = *((unsigned int*) (&buf[0x20]));

	decodedata = new unsigned char[dlength];
	decode(rawdata, length, 0x800, 0x5e, 0xe1);
	decode(rawdata, length, 0x40, 0x7d, 0x3a);
	decompress(rawdata, decodedata, length);
	
	delete[] buf;
	*buffer = decodedata;
	return dlength;
}

unsigned int th13decode(unsigned char **buffer, unsigned int flength) {
	unsigned char *buf = *buffer;
	unsigned char *rawdata = &buf[0x24], *decodedata;
	unsigned int length = *((unsigned int*) (&buf[0x1c]));
	unsigned int dlength = *((unsigned int*) (&buf[0x20]));

	decodedata = new unsigned char[dlength];
	decode(rawdata, length, 0x400, 0x5c, 0xe1);
	decode(rawdata, length, 0x100, 0x7d, 0x3a);
	decompress(rawdata, decodedata, length);

	delete[] buf;
	*buffer = decodedata;
	return dlength;
}

unsigned int th125decode(unsigned char **buffer, unsigned int flength) {
	unsigned char *buf = *buffer;
	unsigned char *rawdata = &buf[0x24], *decodedata;
	unsigned int length = *((unsigned int*) (&buf[0x1c]));
	unsigned int dlength = *((unsigned int*) (&buf[0x20]));

	decodedata = new unsigned char[dlength];
	decode(rawdata, length, 0x800, 0x5e, 0xe1);
	decode(rawdata, length, 0x40, 0x7d, 0x3a);
	decompress(rawdata, decodedata, length);
	
	delete[] buf;
	*buffer = decodedata;
	return dlength;
}

unsigned int th128decode(unsigned char **buffer, unsigned int flength) {
	unsigned char *buf = *buffer;
	unsigned char *rawdata = &buf[0x24], *decodedata;
	unsigned int length = *((unsigned int*) (&buf[0x1c]));
	unsigned int dlength = *((unsigned int*) (&buf[0x20]));

	decodedata = new unsigned char[dlength];
	decode(rawdata, length, 0x800, 0x5e, 0xe7);
	decode(rawdata, length, 0x80, 0x7d, 0x36);
	decompress(rawdata, decodedata, length);
	
	delete[] buf;
	*buffer = decodedata;
	return dlength;
}
