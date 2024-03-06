// 프로그램의 상태코드를 인수로 받아서 그 상태코드로 종료하는 프로그램
// 사용법: ./exit_code 3
// 상태코드 3으로 종료한다.
// 확인 방법: echo $?
// 상태코드 3이 출력된다.


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// argv 가 적절히 입력되었는지 확인하는 함수
void print_all_argv(char *argv[]) {
    for (int i = 0; argv[i] != NULL; i++) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }
}

int main(int argc, char *argv[]) {
    // 모든 argv를 출력한다.
    print_all_argv(argv);

    // argv[1]이 NULL이 아니라면, argv[1] 의 값을 정수로 변환하여 상태 코드로 반환한다
    if (argv[1] != NULL) {
        _exit(atoi(argv[1]));
    }
}
