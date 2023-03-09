#include "../func.c"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    char buffer[mes_size];
    if (argc != 3) {
        printf("Please run the program with 3 arguments\n e.g.: \"./code read.txt write.txt\"\n");
        return 0;
    }

    int fd23[2];
    int fd12[2];
    int process;
    int size;

    // Создаем первый канал (для передачи первый->второй процесс) и записываем его значение
    if (pipe(fd12) < 0) {
        printf("Can\'t open pipe\n");
        exit(-1);
    }

    // Создаем второй канал (для передачи второй->третий процесс) и записываем его значение
    if (pipe(fd23) < 0) {
        printf("Can\'t open pipe\n");
        exit(-1);
    }

    // Cоздаем процессы (первый и второй)
    process = fork();
    if (process < 0) {
        printf("Can\'t fork child\n");
        exit(-1);
    } else if (process == 0) {
        // Второй процесс

        // Разделяем второй процесс на второй и третий
        int process2 = fork();

        if (process2 < 0) {
            printf("Can\'t fork child\n");
            exit(-1);
        } else if (process2 == 0) {
            // Третий процесс - считать второй канал (результат выполнения функции)
            // Напечатать результат выполнения в output-файл
            if (close(fd23[1]) < 0) {
                printf("child: Can\'t close writing side of pipe\n");
                exit(-1);
            }

            // Считываем второй канал
            size = read(fd23[0], buffer, mes_size);

            if (size < 0) {
                printf("Can\'t read string from pipe\n");
                exit(-1);
            }

            if (close(fd23[0]) < 0) {
                printf("child: Can\'t close reading side of pipe\n");
                exit(-1);
            }

            int file_to_write = 0;

            if ((file_to_write = open(argv[2], O_WRONLY | O_CREAT, 0666)) < 0) {
                printf("Can\'t open file\n");
                exit(-1);
            }

            // Запись в файл
            size = write(file_to_write, buffer, strlen(buffer));

            if (size != strlen(buffer)) {
                printf("Can\'t write all string\n");
                exit(-1);
            }

            if (close(file_to_write) < 0) {
                printf("Can\'t close file\n");
            }
        } else {
            // Второй процесс - считать первый канал (информацию из input-файла)
            // Вызвать функцию и получить результат
            // Направить результат во второй канал для передачи в третий процесс
            if (close(fd12[1]) < 0) {
                printf("child: Can\'t close writing side of pipe\n");
                exit(-1);
            }

            // Считывание из первого канала
            size = read(fd12[0], buffer, mes_size);

            if (size < 0) {
                printf("Can\'t read string from pipe\n");
                exit(-1);
            }

            // Вызов функции по заданию
            count_chars(buffer);

            if (close(fd12[0]) < 0) {
                printf("child: Can\'t close reading side of pipe\n");
                exit(-1);
            }

            // Запись во второй канал
            size = write(fd23[1], buffer, mes_size);

            if (size != mes_size) {
                printf("Can\'t write all string to pipe\n (size = %d)", size);
                exit(-1);
            }

            if (close(fd23[1]) < 0) {
                printf("parent: Can\'t close writing side of pipe\n");
                exit(-1);
            }
        }
    } else {
        // process > 0
        // Первый процесс - считать из файла и направить в поток один (в процесс 2)
        if (close(fd12[0]) < 0) {
            printf("parent: Can\'t close reading side of pipe\n");
            exit(-1);
        }

        int input_file = open(argv[1], O_RDONLY, 0666);

        if (input_file < 0) {
            printf("Can\'t open file\n");
            exit(-1);
        }

        // Считываем файл
        size = read(input_file, buffer, mes_size);

        if (close(input_file) < 0) {
            printf("Can\'t close file\n");
        }

        // Запись в первый канал
        size = write(fd12[1], buffer, mes_size);

        if (size != mes_size) {
            printf("Can\'t write all string to pipe\n (size = %d)", size);
            exit(-1);
        }

        if (close(fd12[1]) < 0) {
            printf("parent: Can\'t close writing side of pipe\n");
            exit(-1);
        }
    }

    return 0;
}
