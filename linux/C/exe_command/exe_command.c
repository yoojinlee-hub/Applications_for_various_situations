#include <stdio.h>
#include <stdlib.h>

int main() {
    char command[100] = " mkdir folder";
    
    fgets(command, sizeof(command), stdin);  // 터미널에서 IPMI 명령을 입력 받음
    
    // 명령 실행
    int result = system(command);
    
    if (result == -1) {
        printf("Failed to execute the command.\n");
    } else {
        printf("Command executed successfully.\n");
    }
    
    return 0;
}

