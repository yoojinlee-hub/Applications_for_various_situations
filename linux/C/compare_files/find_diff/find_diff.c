#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void compareFiles(const char* file1, const char* file2);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <file1> <file2>\n", argv[0]);
        return 1;
    }

    const char* file1 = argv[1];
    const char* file2 = argv[2];

    compareFiles(file1, file2);

    return 0;
}

void compareFiles(const char* file1, const char* file2) {
    FILE* fp1 = fopen(file1, "r");
    FILE* fp2 = fopen(file2, "r");

    if (fp1 == NULL || fp2 == NULL) {
        printf("Error: Failed to open files\n");
        return;
    }

    int line = 1;
    char line1[BUFSIZ];
    char line2[BUFSIZ];

    while (fgets(line1, sizeof(line1), fp1) && fgets(line2, sizeof(line2), fp2)) {
        if (strcmp(line1, line2) != 0) {
            printf("Difference found at line %d\n", line);
            printf("%s: %s", file1, line1);
            printf("%s: %s", file2, line2);
        }
        line++;
    }

    fclose(fp1);
    fclose(fp2);
}

