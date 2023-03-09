#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "../func.c"


const int max_size = 200;
const char *pipe1 = "pipe1.fifo";
const char *pipe2 = "pipe2.fifo";

int main(int argc, char *argv[]) {

    int fd12;
    int fd23;
    int size;
    char buffer[mes_size];

    mknod(pipe1, S_IFIFO | 0666, 0);
    mknod(pipe2, S_IFIFO | 0666, 0);
    fd23 = 0;

    // Второй процесс
    // Cчитать первый канал (информацию из input-файла)
    // Вызвать функцию и получить результат
    // Направить результат во второй канал
    ssize_t read_bytes;
    ssize_t write_bytes = 0;

    fd12 = open(pipe1, O_RDONLY);
    if (fd12 < 0) {
        printf("Can\'t open FIFO for reading\n");
        exit(-1);
    }

    // Считывание из первого канала
    do {
        char tmp_buffer[max_size];
        read_bytes = read(fd12, tmp_buffer, max_size);
        if (read_bytes == -1) {
            printf("Can\'t read from fd12\n");
            exit(-1);
        }
        tmp_buffer[read_bytes] = '\0';
        strcat(buffer, tmp_buffer);
        write_bytes = write_bytes + read_bytes;
        tmp_buffer[write_bytes] = '\0';
    } while (read_bytes == max_size);

    if (close(fd12) < 0) {
        printf("parent: Can\'t close writing side of FIFO\n");
        exit(-1);
    }

    // Вызов функции по заданию
    count_chars(buffer);

    fd23 = open(pipe2, O_WRONLY);
    if (fd23 < 0) {
        printf("Can\'t open FIFO-2 for writing\n");
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