#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <sys/types.h>

// 자식 프로세스가 실행하는 함수
static void child() {
    printf("I am the child! My PID is %d.\n", getpid());
    exit(EXIT_SUCCESS);
}

// 부모 프로세스가 실행하는 함수
static void parent(pid_t pid_c) {
    printf("I am the parent! My PID is %d and the PID of my child is %d.\n", getpid(), pid_c);
    exit(EXIT_SUCCESS);
}

int main(){
    pid_t ret; //자식 프로세스의 PID를 저장할 변수
    ret = fork(); // 자식입장에서는 0, 부모입장에서는 자식의 PID를 반환받는다
    if(ret == -1){
        err(EXIT_FAILURE, "fork() Faliure");
    }
    if(ret == 0){
        // 자식 프로세스는 여기에 도달한다. 자식 프로세스 입장에서는 fork() 함수는 0을 반환 했기 때문에
        child();
    }
    else{
        // 부모 프로세스는 여기에 도달한다. 부모 프로세스 입장에서는 fork() 함수는 자식 프로세스의 PID를 반환 했기 때문에
        parent(ret);
    }

    // 이곳에는 도달하지 않아야 한다.
    err(EXIT_FAILURE, "이곳에는 도달하지 않아야 합니다.");
}
