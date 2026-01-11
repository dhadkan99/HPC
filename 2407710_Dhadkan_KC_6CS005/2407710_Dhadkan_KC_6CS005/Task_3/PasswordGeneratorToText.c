#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char wrap_letter(char c) {
    return 'a' + (c - 'a' + 26) % 26;
}

char wrap_digit(char c) {
    return '0' + (c - '0' + 10) % 10;
}

char* cudaCrypt(char* rawPassword) {
    static char newPassword[11];

    newPassword[0] = rawPassword[0] + 2;
    newPassword[1] = rawPassword[0] - 2;
    newPassword[2] = rawPassword[0] + 1;
    newPassword[3] = rawPassword[1] + 3;
    newPassword[4] = rawPassword[1] - 3;
    newPassword[5] = rawPassword[1] - 1;
    newPassword[6] = rawPassword[2] + 2;
    newPassword[7] = rawPassword[2] - 2;
    newPassword[8] = rawPassword[3] + 4;
    newPassword[9] = rawPassword[3] - 4;
    newPassword[10] = '\0';

    for (int i = 0; i < 10; i++) {
        if (i < 6)
            newPassword[i] = wrap_letter(newPassword[i]);
        else
            newPassword[i] = wrap_digit(newPassword[i]);
    }
    return newPassword;
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        printf("Usage: %s <number_of_passwords>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n < 10000) {
        printf("Password count must be at least 10000\n");
        return 1;
    }

    FILE* fp = fopen("passwords.txt", "w");
    if (!fp) {
        printf("Error opening output file\n");
        return 1;
    }

    srand((unsigned)time(NULL));

    char raw[5];
    raw[4] = '\0';

    for (int i = 0; i < n; i++) {
        raw[0] = 'a' + rand() % 26;
        raw[1] = 'a' + rand() % 26;
        raw[2] = '0' + rand() % 10;
        raw[3] = '0' + rand() % 10;
        fprintf(fp, "%s\n", cudaCrypt(raw));
    }

    fclose(fp);
    return 0;
}
