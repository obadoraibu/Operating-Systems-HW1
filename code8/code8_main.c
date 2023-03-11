#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "../lib.c"

const char *pipe1 = "pipe1.fifo";
const char *pipe2 = "pipe2.fifo";

int main(int argc, char *argv[]) {
    (void) umask(0);
    if (argc != 3) {
        printf("Please run the program with 2 arguments\n e.g.: \"./code read.txt write.txt\"\n");
        return 0;
    }

    int fd12;
    int fd23;
    int size;
    char buffer[mes_size];

    mknod(pipe1, S_IFIFO | 0666, 0);
    mknod(pipe2, S_IFIFO | 0666, 0);

    // Первый процесс - считать из файла и направить в поток один (в процесс 2)
    int input_file = open(argv[1], O_RDONLY, 0666);

    if (input_file < 0) {
        printf("Can\'t open file\n");
        exit(-1);
    }

    // Cчитывание из файла
    size = read(input_file, buffer, mes_size);

    if (close(input_file) < 0) {
        printf("Can\'t close file\n");
    }

    if ((fd12 = open(pipe1, O_WRONLY)) < 0) {
        printf("Can\'t open FIFO for writting\n");
        exit(-1);
    }

    // Запись в первый канал
    size = write(fd12, buffer, mes_size);

    if (size != mes_size) {
        printf("Can\'t write all string to FIFO\n");
        exit(-1);
    }
    if (close(fd12) < 0) {
        printf("parent: Can\'t close writing side of FIFO\n");
        exit(-1);
    }


    fd23 = open(pipe2, O_RDONLY);
    if (fd23 < 0) {
        printf("Can\'t open FIFO for reading\n");
        exit(-1);
    }

    // Считать второй канал (результат выполнения функции)
    // Напечатать результат выполнения в output-файл

    size = read(fd23, buffer, mes_size);

    if (size < 0) {
        printf("Can\'t read string from FIFO\n");
        exit(-1);
    }

    if (close(fd23) < 0) {
        printf("Can\'t close FIFO\n");
        exit(-1);
    }

    int output_file = open(argv[2], O_WRONLY | O_CREAT, 0666);

    if (output_file < 0) {
        printf("Can\'t open file\n");
        exit(-1);
    }

    // Запись в файл
    size = write(output_file, buffer, strlen(buffer));

    if (size != strlen(buffer)) {
        printf("Can\'t write all string\n");
        exit(-1);
    }

    if (close(output_file) < 0) {
        printf("Can\'t close file\n");
    }

    unlink(pipe1); // удаляем pipe из файловой системы
    unlink(pipe2); // удаляем pipe из файловой системы

    return 0;
}