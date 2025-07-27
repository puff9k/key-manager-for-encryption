#include "keymanager.h"
#include "xor.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdint.h>


#define KEYSIZE 32

long getfilesize(const char *filename);
void encrypt(const char *filein, char *key, long keysize);
const char *getfileextension(const char *filename);

int main (int argc, char *argv[]) {
	if (argc != 3) {
		fprintf(stderr, "usage : ./main source key ("" to generate key)");
		return EXIT_FAILURE;
	}

	const char *filein = argv[1];

	if (strcmp(argv[2], "" != 0)) {
		char key[KEYSIZE];
		resizekey(key, argv[2]);
	}

	else {
		long keysize = getfilesize(filein);
		if (keysize == -1) {
			return EXIT_FAILURE;
		}
		char key[keysize];
		genkey(key, keysize);
	}

	encrypt(filein, key, keysize);

	printf("Done \n");

	return EXIT_SUCCESS;
}

const char *getfileextension(const char *filename) {
	const char* dot = strrchr(filename, '.');
	return (dot && dot != filename) ? dot : "";
}

void encrypt(const char *filein, char *key, long keysize) {
	FILE *fin = fopen(filein, "rb");
	if (fin == NULL) {
		perror("Could not open source file");
		return;
	}
	FILE *fout = fopen("fileout", "wb");
	if (fout == NULL) {
		fclose(fin);
		perror("Could not create target file");
		return;
	}

	const char* extension = getfileextension(filein);
	size_t ext_len = strlen(extension);

	fputc((unsigned char)ext_len, fout);
	if (ext_len > 0) {
		fwrite(extension, 1, ext_len, fout);
	}

	int ch;
	size_t keyindex = 0;
	while ((ch = fgetc(fin)) != EOF) {
		fputc(ch ^ key[keyindex], fout);
		keyindex = (keyindex + 1) % (long unsigned int)keysize; //quand index == size, index = 0
	}

	if (ferror(fin) || ferror(fout)) {
		perror("Error, somewhere...");
	}
	if (fclose(fin) != 0) {
		perror("Error while closing source file");
	}
	if (fclose(fout) != 0) {
		perror("Error while closing destination file");
	}
}
