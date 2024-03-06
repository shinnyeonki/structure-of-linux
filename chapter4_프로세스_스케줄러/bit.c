#include <stdio.h>
#include <time.h>

int main() {

    printf("size of char (byte) : %ld\n",sizeof(char));
    printf("size of short (byte) : %ld\n",sizeof(short));
    printf("size of int (byte) : %ld\n",sizeof(int));
    printf("size of long (byte) : %ld\n",sizeof(long));
    printf("size of long long (byte) : %ld\n",sizeof(long long));
    printf("size of float (byte) : %ld\n",sizeof(float));
    printf("size of double (byte) : %ld\n",sizeof(double));
    printf("size of long double (byte): %ld\n",sizeof(long double));

    unsigned short number = (short)16; // 확인하고 싶은 숫자
    int bitCount = sizeof(number) * 8; // unsigned int의 비트 수 = 바이트 * 8

    printf("Number: %u\n", number);
    printf("size of byte : %ld\n",sizeof(number));
    printf("Binary: ");

    // 각 비트를 확인하는 반복문
    for (int i = bitCount - 1; i >= 0; i--) {
        // i번째 비트를 오른쪽으로 이동시키고, 이를 1과 AND 연산하여 비트 값을 얻습니다.
        // 이때 bit 의 값을 담기 위해 unsigned int를 자료형으로 사용
        unsigned int bit = (number >> i) & 1; 
        printf("%u", bit);
    }
    printf("\n");

    return 0;
}

