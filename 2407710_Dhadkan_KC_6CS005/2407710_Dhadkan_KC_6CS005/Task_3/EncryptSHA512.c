#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crypt.h>
#include <unistd.h>

#define SALT "$6$AS$"
#define MAX_LEN 256

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    FILE *in = fopen(argv[1], "r");
    FILE *out = fopen(argv[2], "w");

    if (!in || !out) {
        printf("Error opening files\n");
        return 1;
    }

    char line[MAX_LEN];

    while (fgets(line, MAX_LEN, in)) {
        line[strcspn(line, "\n")] = '\0';
        char *hash = crypt(line, SALT);
        if (hash)
            fprintf(out, "%s\n", hash);
    }

    fclose(in);
    fclose(out);
    return 0;
}
