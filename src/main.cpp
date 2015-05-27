#include <stdio.h>
#include "jit/jfunction.h"
#include "utils/bytebuffer.h"
#include "parser.h"

int writeToFile(Assembler::ByteBuffer const& buffer, char const* filename) {
	FILE* fout = fopen(filename, "wb");
	
	if (!fout) {
		printf("Could not open %s to write\n", filename);
		return -1;
	}

	if (buffer.current() != 0) {
		if (fwrite(buffer.raw(), buffer.current(), 1, fout) != 1) {
			printf("Could not write to file %s\n", filename);
			fclose(fout);
			return -1;
		}
	}

	fclose(fout);
	return 0;
}

char* readFromFile(char const* filename) {
	FILE* fin = fopen(filename, "rb");

	if (!fin) {
		printf("Could not open file %s\n", filename);
		return 0;
	}

	fseek(fin, 0, SEEK_END);
	unsigned long size = ftell(fin);
	fseek(fin, 0, SEEK_SET);

	char* buffer = new char[size + 1];
	if (fread(buffer, size, 1, fin) != 1) {
		//Clear the buffer and return 0 if the load fails
		printf("Failed to read the file %s into memory\n", filename);
		delete[] buffer;
		fclose(fin);
		return 0;
	}

	fclose(fin);
	buffer[size] = 0;
	return buffer;
}

int main(int argc, char** argv) {

	if (argc != 2) {
		printf("Incorrect number of arguments\n");
		return -1;
	}

	char* inputSource = readFromFile(argv[1]);

	if (!inputSource) {
		printf("Could not load input source\n");
		return -1;
	}

	Assembler::Parser parser;
	if (!parser.parse(inputSource)) {
		return -1;
	}

	delete[] inputSource;

	return 0;
}
