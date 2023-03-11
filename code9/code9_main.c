#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int mes_size = 200;

const int max_size = 20;
const char *pipe1 = "pipe1.fifo";
const char *pipe2 = "pipe2.fifo";

int main(int argc, char *argv[]) {
    mes_size = 200;

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
    int file_to_read = open(argv[1], O_RDONLY, 0666);

    if (file_to_read < 0) {
        printf("Can\'t open file\n");
        exit(-1);
    }

    ssize_t read_bytes;
    fd12 = open(pipe1, O_WRONLY);
    if (fd12 < 0) {
        printf("Can\'t open FIFO for writting\n");
        exit(-1);
    }

    do {
        char temple_buffer[max_size];
        read_bytes = read(file_to_read, temple_buffer, max_size);
        if (read_bytes == -1) {
            printf("Can\'t read this file\n");
            exit(-1);
        }

        temple_buffer[read_bytes] = '\0';

        // Запись в первый канал
        size = write(fd12, temple_buffer, max_size);

        if (size != max_size) {
            printf("Can\'t write all string to FIFO\n");
            exit(-1);
        }

    } while (read_bytes == max_size);

    if (close(fd12) < 0) {
        printf("parent: Can\'t close writing side of FIFO\n");
        exit(-1);
    }

    if (close(file_to_read) < 0) {
        printf("Can\'t close file\n");
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

    return 0;
}