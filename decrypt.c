#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PADLEN 512
#define BUFLEN 1024

unsigned char pad[] = ""
"\xda\x59\xb6\xfd\x3f\xe5\xcb\xa7\xc3\x69\xb3\xd9\x6d\xc7\xea\x90"
"\xcb\xad\x7f\x53\x83\xe0\x4d\x16\x5c\x39\xca\x43\x75\xe7\xc9\x8a"
"\xe1\x94\xb0\xd6\xfa\x27\xc6\x85\x72\xeb\x2f\x70\xba\xaa\x25\x9a"
"\x8d\xd5\x76\x7a\x5e\x37\xa2\x7d\x5f\x22\x3d\x1d\x7a\xde\xc0\x47"
"\x11\x04\xa9\xc0\xe4\xdd\x51\xe4\xda\xd2\x21\x56\xa8\x6c\x18\x83"
"\x92\x0e\x24\x50\x03\xdb\xff\xc9\xa6\x9d\x86\x24\x14\x17\xe8\x95"
"\x3f\x77\xa0\x91\xc4\x99\xc4\xd9\xc1\x20\xff\xc9\x04\x46\xd9\x0a"
"\x00\xa4\xce\x1d\xfc\x16\x84\xb1\xd7\xa4\x66\x70\x30\x68\x78\xfd"
"\x12\xf9\x1e\xdf\x00\xc0\x5e\x69\x38\x60\x90\x4f\x88\x1b\x99\x33"
"\x1a\xc6\x9b\xa9\x78\xfe\x79\x1b\x93\x28\x4d\xc0\x8a\x67\x4f\x7b"
"\xc8\x24\xb4\x2e\x1e\x06\xf4\x64\x9b\x80\x78\xa3\x5a\x61\xd0\xe4"
"\x64\xcc\xdc\x19\x3b\x70\x61\x1f\xea\x6c\x8e\xf1\xb9\x66\x91\x02"
"\xf3\x3a\x45\x22\x4a\xf0\x22\xcf\x34\x16\xcb\xd2\xfc\x31\xc1\xec"
"\xfb\x20\x92\x25\xb4\x13\x8b\x2b\xcf\x5e\x2b\xfa\x10\xa3\xf0\x1a"
"\x72\x69\xc1\xc4\xb6\x1a\x5e\xf0\xc6\xea\x60\x19\x96\x15\x2b\x3a"
"\x1c\xab\xb3\x65\x72\xee\x95\x37\x67\x4a\xff\xb6\x8c\xf6\xe0\x93"
"\x43\xbd\x90\x1d\x02\x5f\xf0\x35\xb3\x57\x86\xe7\xda\x01\x07\xc9"
"\x7d\x0a\x2a\x2d\xde\xc9\xf4\xbd\x78\xb8\x60\xc7\xc5\xf9\x16\x28"
"\x8c\x5d\xc6\x6a\x9b\xd1\x46\x33\x3d\x15\x5b\x2a\x9e\x38\x0c\xd2"
"\x6e\xc7\x6f\x5d\x06\xa5\xaa\x40\xdb\x63\x90\x87\x3a\x09\xac\xa9"
"\xba\x19\x6a\xed\xda\x84\xdb\x8b\x14\x0c\x52\x36\x67\xd0\xbb\xe1"
"\x54\xd8\x47\x5d\x0c\x1c\x6f\xeb\x84\x4b\x2f\x13\xf0\x53\x73\x23"
"\x0d\x00\x3a\xd0\x8d\xf0\xde\x86\xfe\x1d\x1a\xe8\x4d\xfa\x97\xdd"
"\x59\xc4\xe9\x66\xcc\x01\xd1\x7d\xd2\xcd\xd0\x92\x54\x32\xf7\x86"
"\x90\x12\x90\x1e\x34\xcf\xec\x74\x15\xac\x6d\x46\x90\x22\x95\xcf"
"\x91\xe0\x76\x99\x3e\x1e\x41\x24\x12\x81\x89\xcb\x5e\xab\x6f\x37"
"\x56\x71\x58\x60\x38\x3e\x23\x94\xa0\xca\x1f\xcf\xeb\xab\xb2\x22"
"\x9d\x68\x40\x2e\x94\x04\x78\x03\xfc\x21\xb7\x8d\xf2\x45\xe3\x49"
"\xde\xd0\xd4\xf7\xaa\x40\xda\x10\xfd\x76\x12\x6b\xd5\x0c\x13\x1d"
"\x3d\x15\x46\xc0\xfc\xef\x9f\xd1\xa5\x67\x90\xbc\x14\xdc\xb3\xb0"
"\xeb\xe1\x84\x4b\x7c\xb1\x56\x07\x37\xc7\x18\xac\xe9\x40\xb3\xf5"
"\x8d\x40\x71\xda\x95\x63\x16\xd4\xd2\xb1\xb9\xe5\x11\x19\xff\x3f";

unsigned char calc_magic_byte(unsigned char in, unsigned char pad, unsigned char known_value) {
	return in - (pad ^ known_value);
}

unsigned char decrypt_byte(unsigned char in, unsigned char pad, unsigned char magic) {
	return (in - magic) ^ pad;
}

int decrypt_by_brute_force(FILE* infd) {
	unsigned char * buf = malloc((BUFLEN+1) * sizeof(unsigned char));
	if (NULL == buf) {
		exit(-1);
	}
	memset(buf, 0, BUFLEN+1);

	fread(buf, 1024, 1, infd);

	unsigned char magic[4];
	for (int bufpos = 0; bufpos < BUFLEN; bufpos++) {
		for (int padpos = 0; padpos < PADLEN; padpos++) {
			// deduce magic from first PDF header word
			magic[0] = calc_magic_byte(buf[bufpos+0], pad[(padpos+0)%PADLEN], '%');
			magic[1] = calc_magic_byte(buf[bufpos+1], pad[(padpos+1)%PADLEN], 'P');
			magic[2] = calc_magic_byte(buf[bufpos+2], pad[(padpos+2)%PADLEN], 'D');
			magic[3] = calc_magic_byte(buf[bufpos+3], pad[(padpos+3)%PADLEN], 'F');

			if (decrypt_byte(buf[bufpos+4], pad[(padpos+4)%PADLEN], magic[0]) != '-') {
				// no match on first char
				continue;
			}
			//fprintf(stderr, "potential match 1\n");
			if (decrypt_byte(buf[bufpos+5], pad[(padpos+5)%PADLEN], magic[1]) != '1') {
				continue;
			}
			// fprintf(stderr, "potential match 2\n");

			// putc(decrypt_byte(buf[bufpos+0], pad[padpos+0], magic[0]), stderr);
			// putc(decrypt_byte(buf[bufpos+1], pad[padpos+1], magic[1]), stderr);
			// putc(decrypt_byte(buf[bufpos+2], pad[padpos+2], magic[2]), stderr);
			// putc(decrypt_byte(buf[bufpos+3], pad[padpos+3], magic[3]), stderr);
			// putc(decrypt_byte(buf[bufpos+4], pad[padpos+4], magic[0]), stderr);
			// putc(decrypt_byte(buf[bufpos+5], pad[padpos+5], magic[1]), stderr);
			// putc(decrypt_byte(buf[bufpos+6], pad[padpos+6], magic[2]), stderr);
			// putc(decrypt_byte(buf[bufpos+7], pad[padpos+7], magic[3]), stderr);
			// putc('\n', stderr);

			if (decrypt_byte(buf[bufpos+6], pad[(padpos+6)%PADLEN], magic[2]) != '.') {
				continue;
			}

			char versionByte = decrypt_byte(buf[bufpos+7], pad[(padpos+7)%PADLEN], magic[3]);
			if (versionByte < '0' || versionByte > '9') {
				continue;
			}

			fprintf(stderr, "Found PDF header at bufpos %d, padpos %d, magic %02X %02X %02X %02X\n", bufpos, padpos, magic[0], magic[1], magic[2], magic[3]);

			fseek(infd, bufpos, SEEK_SET);
			int magicpos = 0;
			while (!feof(infd)) {
				putc(decrypt_byte(fgetc(infd), pad[padpos], magic[magicpos]), stdout);
				padpos = (padpos+1) % PADLEN;
				magicpos = (magicpos+1) % 4;
			}
			exit(0);
		}
	}
	fprintf(stderr, "NO MATCH\n");
	exit(1);
}

int decrypt_by_header(FILE* infd, int deviceKey) {
	unsigned char * buf = malloc((BUFLEN+1) * sizeof(unsigned char));
	if (NULL == buf) {
		exit(-1);
	}
	memset(buf, 0, BUFLEN+1);

	fread(buf, 1024, 1, infd);

	int expectedHeader = 0x19781201;
	if (memcmp(buf, (char *) &expectedHeader, 4) != 0) {
		fprintf(stderr, "Unexpected header %08x, bailing!\n", *((int *)buf));
		return 1;
	}

	// // this chunk only really serves to check if the MAC matches?
	// int perFileKey = * ((int *)buf+2); // from file header
	// int padPart = * ((int *)pad);
	// int key1 = perFileKey ^ padPart;
	// fprintf(stderr, "per-file key 1: %08X\n", key1);

	// // the modified libezpdf has a check like this, but it doesn't catch all failures
	// if (key1 < deviceKey) {
	// 	fprintf(stderr, "WARNING: per-device key seems to be incorrect\n");
	// }

	int special = * ((int *)buf+3) ^ * ((int *)pad+1);

	//fprintf(stderr, "second buf^pad word: %08X\n", special);

	int calculatedMagic = deviceKey - special;
	fprintf(stderr, "calculated magic: %08X\n", calculatedMagic);

	int bufpos = 24;
	int padpos = calculatedMagic%512+24;

	unsigned char * magic = (unsigned char *) &calculatedMagic;

	fprintf(stderr, "bufpos %d, padpos %d, magic %02X %02X %02X %02X\n", bufpos, padpos, magic[0], magic[1], magic[2], magic[3]);

	fseek(infd, bufpos, SEEK_SET);
	while (!feof(infd)) {
		putc(decrypt_byte(fgetc(infd), pad[padpos], magic[padpos%4]), stdout);
		padpos = (padpos+1) % PADLEN;
	}
	return 0;
}

int main (int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Usage: decrypt <file.pdf>\n");
		return 1;
	}

	//open file, read first 1k into char[]
	FILE* infd = fopen(argv[1], "rb");
	if (infd < 0) {
		fprintf(stderr, "File '%s' not found!\n", argv[1]);
		return 1;
	}

	// int deviceKey = 0x00000000; // fill this in if you want to calculate stuff from header instead of bruteforcing
	// return decrypt_by_header(infd, deviceKey);

	return decrypt_by_brute_force(infd);
}
