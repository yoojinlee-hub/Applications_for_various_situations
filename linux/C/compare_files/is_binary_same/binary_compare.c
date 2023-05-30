#include <stdio.h>

int compare_binary_files(const char* file_path1, const char* file_path2) {
    FILE* file1 = fopen(file_path1, "rb");
    FILE* file2 = fopen(file_path2, "rb");

    if (file1 == NULL || file2 == NULL) {
        printf("Error: Failed to open files\n");
        return -1;
    }

    // 파일 크기 비교
    fseek(file1, 0, SEEK_END);
    fseek(file2, 0, SEEK_END);
    long size1 = ftell(file1);
    long size2 = ftell(file2);
    rewind(file1);
    rewind(file2);

    if (size1 != size2) {
        fclose(file1);
        fclose(file2);
        return 0; // 크기가 다르면 다른 파일로 판단
    }

    int is_equal = 1; // 초기값을 같다고 설정

    int byte1, byte2;
    while ((byte1 = fgetc(file1)) != EOF && (byte2 = fgetc(file2)) != EOF) {
        if (byte1 != byte2) {
            is_equal = 0;
            break;
        }
    }

    fclose(file1);
    fclose(file2);

    return is_equal;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <file1> <file2>\n", argv[0]);
        return 1;
    }

    const char* file_path1 = argv[1];
    const char* file_path2 = argv[2];

    int result = compare_binary_files(file_path1, file_path2);
    if (result == 1) {
        printf("The binary files are equal.\n");
    } else if (result == 0) {
        printf("The binary files are different.\n");
    } else {
        printf("Error: Failed to compare files.\n");
        return 1;
    }

    return 0;
}


