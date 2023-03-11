#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "../lib.c"

const char *pipe1 = "pipe1.fifo";
const char *pipe2 = "pipe2.fifo";

int main(int argc, char *argv[]) {
    int fd12;
    int fd23;
    int size;
    char buffer[mes_size];

    mknod(pipe1, S_IFIFO | 0666, 0);
    mknod(pipe2, S_IFIFO | 0666, 0);

    fd12 = open(pipe1, O_RDONLY);
    // Второй процесс
    // Cчитать первый канал (информацию из input-файла)
    // Вызвать функцию и получить результат
    // Направить результат во второй канал
    if (fd12 < 0) {
        printf("Can\'t open FIFO for reading\n");
        exit(-1);
    }

    // Считывание из первого канала
    size = read(fd12, buffer, mes_size);

    if (size < 0) {
        printf("Can\'t read string from FIFO\n");
        exit(-1);
    }

    // Вызов функции по заданию
    count_chars(buffer);

    if (close(fd12) < 0) {
        printf("child: Can\'t close FIFO\n");
        exit(-1);
    }

    fd23 = open(pipe2, O_WRONLY);

    if (fd23 < 0) {
        printf("Can\'t open FIFO for writing\n");
        exit(-1);
    }

    // Запись во второй канал
    size = write(fd23, buffer, mes_size);

    if (size != mes_size) {
        printf("Can\'t write all string to FIFO\n (size = %d)", size);
        exit(-1);
    }

    if (close(fd23) < 0) {
        printf("parent: Can\'t close FIFO\n");
        exit(-1);
    }

    return 0;
}