#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main (int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Usage: trim_pdf <file.pdf>\n");
		return 1;
	}

	FILE* infd = fopen(argv[1], "r+b");
	if (infd < 0) {
		fprintf(stderr, "File '%s' not found!\n", argv[1]);
		return 1;
	}

	fseek(infd, 1, SEEK_END);
	int fsize = ftell(infd)-1;

	unsigned char buf[6];
	memset(buf, 0, 6);
	int eofpos;

	while (strcmp((const char *) buf, "%%EOF")) {
		// step back and check for EOF
		fseek(infd, -6, SEEK_CUR);
		eofpos = ftell(infd);

		if (eofpos == 0) {
			fprintf(stderr, "No %%%%EOF, this isn't a PDF!?\n");
			exit(1);
		}

		fread(buf, 5, 1, infd);
		// printf("%s\n", buf);
	}

	// fprintf(stderr, "Found PDF EOF\n");

	// check for post-EOF garbage/watermarks
	int truncAt = eofpos+5;

	fseek(infd, truncAt, SEEK_SET);
	// fprintf(stderr, "len: %d, pos: %ld\n", fsize, ftell(infd));

	while (!feof(infd) && isspace(fgetc(infd))) {
		// not at actual EOF yet and next byte is whitespace
		truncAt++;
	}

	if (fsize == truncAt) {
		fprintf(stderr, "No non-whitespace bytes after PDF EOF, no need to trim\n");
		exit(1);
	}

	fprintf(stderr, "Trimming %d non-whitespace bytes after EOF\n", fsize-truncAt);

	ftruncate(fileno(infd), (off_t) truncAt);
	exit(0);
}