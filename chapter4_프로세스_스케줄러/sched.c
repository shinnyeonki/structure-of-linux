/**
 * 특정 시점의 논리 프로세서(코어) 에 어떠한 프로세스가 실행되고 있는지 확인
 * 각각의 프로세스의 진행도를 확인
 * n, total, resol 을 입력
 * n : 프로세스 개수
 * total : 총 실행 시간
 * resol : 진행도를 기록할 시간 간격
*/
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

#define NLOOP_FOR_ESTIMATION 1000000000UL // 성능 부하를 위해 측정할 루프 횟수
#define NSECS_PER_MICROSEC 1000UL // 마이크로초당 나노초 1(μs) = 1,000(ns)
#define NSECS_PER_MSEC 1000000UL // 밀리초당 나노초 1(ms) = 1,000,000(ns)
#define NSECS_PER_SEC 1000000000UL // 초당 나노초 1(s) = 1,000,000,000(ns)

/*
    before와 after의 시간 차이를 ns 단위로 리턴
    빠른 실행을 위해 inline 키워드 사용
*/
static inline unsigned long diff_nsec(struct timespec before, struct timespec after)
{
    // unsigned long before_nsec = before.tv_sec * NSECS_PER_SEC + before.tv_nsec;
    // unsigned long after_nsec = after.tv_sec * NSECS_PER_SEC + after.tv_nsec;
    // printf("before : %lu\n", before_nsec);
    // printf("after : %lu\n", after_nsec);
    // printf("diff : %lu\n", after_nsec - before_nsec);
    return ((after.tv_sec * NSECS_PER_SEC + after.tv_nsec) \
            - (before.tv_sec * NSECS_PER_SEC + before.tv_nsec));
}

// 1ms 당 반복문을 몇번 돌 수 있는지 측정 (성능 기준치 측정)
static unsigned long loops_per_msec()
{
    struct timespec before, after;

    clock_gettime(CLOCK_MONOTONIC, &before); // Monotonic system-wide clock id 사용 => 시스템 시작 이후 경과 시간을 나타내는 시계
    for (unsigned long i = 0; i < NLOOP_FOR_ESTIMATION; ++i){
        ;
    }
    clock_gettime(CLOCK_MONOTONIC, &after); 

    // 1ms 당 반복 횟수 = 측정 횟수 / 걸린 시간[ms]
    // return NLOOP_FOR_ESTIMATION / (diff_nsec(before, after) / NSECS_PER_MSEC);
    return NLOOP_FOR_ESTIMATION / diff_nsec(before, after) * NSECS_PER_MSEC ;
}

// 단순히 반복문으로 부하를 주는 함수
static inline void load(unsigned long nloop)
{
    for (unsigned long i = 0; i < nloop; ++i){
        ;
    }
}

static void child_fn(int id, struct timespec *buf, int nrecord, \
                    unsigned long nloop_per_resol, struct timespec start)
{
    for (int i = 0; i < nrecord; ++i)
    {
        struct timespec ts;

        load(nloop_per_resol);
        clock_gettime(CLOCK_MONOTONIC, &ts);
        buf[i] = ts;
    }
    for (int i = 0; i < nrecord; ++i)
        // 프로세스고유ID(0 ~ nproc-1), 프로그램시작시점부터경과한시간, 진행도[%]
        printf("%d\t%lu\t%d\n", id, diff_nsec(start, buf[i]) / NSECS_PER_MSEC, \
            100 * (i + 1) / nrecord);

    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    // 인수 적절성 평가
    if (argc != 4)       // 끝에 : Success 뜨는 이유??
        err(EXIT_FAILURE, \
            "usage: %s <nproc> <total[ms]> <resolution[ms]>", argv[0]);

    int nproc = atoi(argv[1]);
    int total = atoi(argv[2]);
    int resol = atoi(argv[3]);

    if (nproc < 1 || total < 1 || resol < 1) 
        err(EXIT_FAILURE, "parameters must be positive value");
    else if (total % resol)
        err(EXIT_FAILURE, "<total> must be multiple of <resolution>");

    /**
     * 총 실행 시간 / 진행도 기록 간격 ==> 총 기록 횟수
     * 총 기록 횟수만큼의 시간을 기록할 수 있는 timespec 구조체 배열(포인터) 할당
     * ex) total = 1000ms, resol = 100ms => nrecord = 10 
     *    => 10개의 시간을 기록할 수 있는 timespec 구조체 배열 할당
     * 예외처리
     */
    int nrecord = total / resol;  
    struct timespec *logbuf = malloc(nrecord * sizeof(struct timespec)); // timespec 구조체 => 시간을 나타내는 구조체
    if (!logbuf)
        err(EXIT_FAILURE, "malloc(logbuf) failed");

    // resolution 당 반복 횟수
    unsigned long nloop_per_msec = loops_per_msec(); // 1ms 당 반복 횟수
    unsigned long nloop_per_resol = nloop_per_msec * resol; // resolution 당 반복 횟수
    // printf("nloop_per_msec : %lu\n", nloop_per_msec);
    // printf("nloop_per_resol : %lu\n", nloop_per_resol);

    pid_t *pids = malloc(nproc * sizeof(pid_t));
    if (!pids)
        err(EXIT_FAILURE, "malloc(pids) failed");

    struct timespec start;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int ret = EXIT_SUCCESS;
    int ncreated = 0;
    for (int i = 0; i < nproc; ++i, ++ncreated)
    {
        pids[i] = fork();
        if (pids[i] < 0)
        {
            for (int j = 0; j < ncreated; ++j)
                kill(pids[j], SIGKILL);
            ret = EXIT_FAILURE;
            break ;
        }
        else if (pids[i] == 0)
            child_fn(i, logbuf, nrecord, nloop_per_resol, start);
    }
    
    for (int i = 0; i < ncreated; ++i)
        if (wait(NULL) < 0)
            warn("wait() failed");

    return ret;
}
