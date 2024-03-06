#include <stdio.h>
#include <time.h>

#define MAX 1000000

int main(){

    //MK: 시작/끝 시간을 측정하기 위해서 추가함 (time.h 필요)
    struct timespec  begin, end;
    double tmpValue = 0.0;


    //MK: 연산 시작과 함께 시간을 측정함
    clock_gettime(CLOCK_MONOTONIC, &begin);
    for(int i = 0; i < MAX; i++){ // cpu 시간 소모를 위한 덧셈 연산
        tmpValue = tmpValue / i + i;
    }

    /**
     * CLOCK_REALTIME
     * System-wide realtime clock. Setting this clock requires appropriate privileges.
     * CLOCK_MONOTONIC
     * Clock that cannot be set and represents monotonic time since some unspecified starting point.
     * CLOCK_PROCESS_CPUTIME_ID
     * High-resolution per-process timer from the CPU.
     * CLOCK_THREAD_CPUTIME_ID
     * Thread-specific CPU-time clock.
    */
    //MK: 연산이 끝나면 시간을 측정함
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("Value: %lf\n", tmpValue); // tmpValue 값은 1 + 2 + 3 + ... + MAX 의 결과이다

    printf("end.tv_sec - begin.tv_sec : %ld\n", end.tv_sec - begin.tv_sec);
    printf("end.tv_nsec - begin.tv_nsec : %ld\n", end.tv_nsec - begin.tv_nsec); //나노초 단위로 시간을 측정함
    /**
     * 1초 = 1,000,000,000 나노초
     * 1밀리초 = 1,000,000 나노초
     * 1마이크로초 = 1,000 나노초
    */
    printf("elapsed time : %lf\n", (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec) / 1000000000.0); //초 단위로 시간을 측정함

    return 0;
    
}