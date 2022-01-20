void th06encode(unsigned char * buffer, unsigned int flength) {
	unsigned int i, checksum;
	unsigned char base;
	base = *((unsigned char*) (buffer[0x0e]));
	checksum = 0x3f000318;
	for (i = 0x0e; i < flength; ++i)
		checksum += buffer[i];
	for (i = 0x0f; i < flength; ++i) {
		buffer[i] += base;
		base += 7;
	}
	// printf("Encoding done.\nChecksum is %d", checksum);
}