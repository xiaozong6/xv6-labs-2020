#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void sieve(int read_fd) {
    int num;
    // 读取第一个数字（必然是素数）
    if (read(read_fd, &num, sizeof(num)) != sizeof(num)) {
        close(read_fd);
        exit(0);
    }

    // 打印第一个素数
    int prime = num;
    printf("prime %d\n", prime);

    // 创建新管道，准备传递剩余数字
    int p[2];
    pipe(p);
    int pid = fork();
    if (pid < 0) {
        printf("fork failed\n");
        exit(1);
    }
    if (pid == 0) { // 子进程
        close(read_fd);
        close(p[1]);
        sieve(p[0]);
        exit(0);
    } else { // 父进程
        close(p[0]);
        // 筛选不能被 prime 整除的数字
        while (read(read_fd, &num, sizeof(num)) == sizeof(num)) {
            if (num % prime != 0) {
                write(p[1], &num, sizeof(num));
            }
        }
        close(read_fd);
        close(p[1]);
        wait(0);
        exit(0);
    }
}

int main(int argc, char **argv) {
    int p[2];
    pipe(p);

    for (int i = 2; i <= 35; i++) {
        write(p[1], &i, sizeof(i));
    }
    close(p[1]);

    int pid = fork();
    if (pid < 0) {
        printf("fork failed\n");
        exit(1);
    }
    if (pid == 0) {
        close(p[1]);
        sieve(p[0]);
        exit(0);
    } else {
        close(p[0]);
        wait(0);
        exit(0);
    }
}