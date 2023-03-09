#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "../func.c"

const char *name1 = "pipe1.fifo";
const char *name2 = "pipe2.fifo";

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Please run the program with 2 arguments\n e.g.: \"./code read.txt write.txt\"\n");
        return 0;
    }

    int fd23;
    int fd12;
    int process;
    int size;
    char buffer[mes_size];

    mknod(name1, S_IFIFO | 0666, 0);
    mknod(name2, S_IFIFO | 0666, 0);
    fd12 = 0;

    // Cоздаем процессы (первый и второй)
    process = fork();
    if (process < 0) {
        printf("Can\'t fork child\n");
        exit(-1);
    } else if (process == 0) {
        // Второй процесс

        // Разделяем второй процесс на второй и третий
        int result1 = fork();

        if (result1 < 0) {
            printf("Can\'t fork child\n");
            exit(-1);
        } else if (result1 == 0) {
            // Третий процесс - считать второй канал (результат выполнения функции)
            // Напечатать результат выполнения в output-файл
            fd12 = open(name2, O_RDONLY);
            if (fd12 < 0) {
                printf("Can\'t open FIFO for reading\n");
                exit(-1);
            }

            // Считываем второй канал
            size = read(fd12, buffer, mes_size);
            if (size < 0) {
                printf("Can\'t read string from FIFO\n");
                exit(-1);
            }

            if (close(fd12) < 0) {
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
        } else {
            // Второй процесс - считать первый канал (информацию из input-файла)
            // Вызвать функцию и получить результат
            // Направить результат во второй канал для передачи в третий процесс
            if ((fd23 = open(name1, O_RDONLY)) < 0) {
                printf("Can\'t open FIFO for reading\n");
                exit(-1);
            }

            // Считывание из первого канала
            size = read(fd23, buffer, mes_size);

            if (size < 0) {
                printf("Can\'t read string from FIFO\n");
                exit(-1);
            }

            // Вызов функции по заданию
            count_chars(buffer);

            if (close(fd23) < 0) {
                printf("child: Can\'t close FIFO\n");
                exit(-1);
            }

            if ((fd12 = open(name2, O_WRONLY)) < 0) {
                printf("Can\'t open FIFO for writting\n");
                exit(-1);
            }

            // Запись во второй канал
            size = write(fd12, buffer, mes_size);

            if (size != mes_size) {
                printf("Can\'t write all string to FIFO\n (size = %d)", size);
                exit(-1);
            }

            if (close(fd12) < 0) {
                printf("parent: Can\'t close FIFO\n");
                exit(-1);
            }
        }
    } else {
        // process > 0
        // Первый процесс - считать из файла и направить в поток один (в процесс 2)
        int file_to_read = 0;

        if ((file_to_read = open(argv[1], O_RDONLY, 0666)) < 0) {
            printf("Can\'t open file\n");
            exit(-1);
        }

        // Cчитываем из файла
        size = read(file_to_read, buffer, mes_size);

        if (close(file_to_read) < 0) {
            printf("Can\'t close file\n");
        }

        if ((fd23 = open(name1, O_WRONLY)) < 0) {
            printf("Can\'t open FIFO for writting\n");
            exit(-1);
        }

        // Записываем в первый канал
        size = write(fd23, buffer, mes_size);

        if (size != mes_size) {
            printf("Can\'t write all string to FIFO\n");
            exit(-1);
        }

        if (close(fd23) < 0) {
            printf("parent: Can\'t close writing side of FIFO\n");
            exit(-1);
        }
    }

    return 0;
}
