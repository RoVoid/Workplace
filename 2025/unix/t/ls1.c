#define _DEFAULT_SOURCE

#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>

char * getType(unsigned char type) {
	if (type == DT_UNKNOWN) return "Unknown";
	if (type == DT_FIFO) return "FIFO";
	if (type == DT_CHR) return "Character Device";
	if (type == DT_DIR) return "Directory";
	if (type == DT_BLK) return "Block Device";
	if (type == DT_REG) return "File";
	if (type == DT_LNK) return "Link";
	if (type == DT_SOCK) return "Socket";
	if (type == DT_WHT) return "Whiteout";
	return "Error";
}

int main(int argc, char** argv) {
	DIR *dir;

	printf("%d\n", argc);

	if (argc > 1) dir = opendir(argv[1]);
	else dir = opendir(".");

	struct dirent *entry;

	while ((entry = readdir(dir)) != NULL) {
		printf("%20s (%s)\n", entry->d_name, getType(entry->d_type));
	}

	closedir(dir);

	return 0;
}