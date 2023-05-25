#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

int main() {
    // UART 장치 파일 경로들
    const char* uartDevices[] = {"/dev/ttyS0", "/dev/ttyS1", "/dev/ttyS2", "/dev/ttyS3", "/dev/ttyS4"};
    const int numUartDevices = sizeof(uartDevices) / sizeof(uartDevices[0]);

    for (int i = 0; i < numUartDevices; ++i) {
        const char* uartDevice = uartDevices[i];

        // UART 장치 파일 열기
        int uartFD = open(uartDevice, O_RDWR | O_NOCTTY);
        if (uartFD == -1) {
            printf("Error: Failed to open UART device %s\n", uartDevice);
            continue;
        }

        // 현재 UART 설정 읽기
        struct termios options;
        if (tcgetattr(uartFD, &options) == -1) {
            printf("Error: Failed to get UART attributes for %s\n", uartDevice);
            close(uartFD);
            continue;
        }

        // UART가 enable 상태인지 확인
        if (options.c_cflag & CREAD) {
            printf("UART %s is enabled\n", uartDevice);
        } else {
            printf("UART %s is disabled\n", uartDevice);
        }

        // UART 장치 파일 닫기
        close(uartFD);
    }

    return 0;
}

