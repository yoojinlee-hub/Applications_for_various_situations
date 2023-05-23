#include <stdio.h>

int compare_binary_files(const char* file_path1, const char* file_path2) {
    FILE* file1 = fopen(file_path1, "rb");
    FILE* file2 = fopen(file_path2, "rb");

    if (file1 == NULL || file2 == NULL) {
        printf("Error: Failed to open files\n");
        return -1;
    }

    int is_equal = 1; // 초기값을 같다고 설정

    int byte1, byte2;
    while ((byte1 = fgetc(file1)) != EOF && (byte2 = fgetc(file2)) != EOF) {
        if (byte1 != byte2) {
            is_equal = 0;
            break;
        }
    }

    // 두 파일 중 하나라도 EOF에 도달하지 않은 경우에는 다른 파일 크기 또는 내용이 있음을 의미함
    if (byte1 != EOF || byte2 != EOF) {
        is_equal = 0;
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

