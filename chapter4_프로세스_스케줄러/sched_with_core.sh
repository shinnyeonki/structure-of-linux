#!/bin/bash

# 사용할 코어의 갯수
CORES="$1"

# 프로그램 실행 명령어
PROGRAM="./sched"

# 코어를 바인딩하여 프로그램 실행
taskset -c 0-$((CORES-1)) $PROGRAM "${@:2}"
