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
        printf("./prog.out read.txt write.txt\n");
        return 0;
    }

    int fd1, fd2, result;
    int size;
    char str_buf[mes_size];

    mknod(name1, S_IFIFO | 0666, 0);
    mknod(name2, S_IFIFO | 0666, 0);
    fd2 = 0;

    result = fork(); // создаем первый и второй процессы
    if (result < 0) {
        printf("Can\'t fork child\n");
        exit(-1);
    } else if (result > 0) { // Первый процесс
        int file_to_read = 0;

        if ((file_to_read = open(argv[1], O_RDONLY, 0666)) < 0) {
            printf("Can\'t open file\n");
            exit(-1);
        }

        size = read(file_to_read, str_buf, mes_size); // считываем из файла

        if (close(file_to_read) < 0) {
            printf("Can\'t close file\n");
        }
        if ((fd1 = open(name1, O_WRONLY)) < 0) {
            printf("Can\'t open FIFO for writting\n");
            exit(-1);
        }

        size = write(fd1, str_buf, mes_size); // записываем в первый канал
        if (size != mes_size) {
            printf("Can\'t write all string to FIFO\n");
            exit(-1);
        }
        if (close(fd1) < 0) {
            printf("parent: Can\'t close writing side of FIFO\n");
            exit(-1);
        }
    } else {  // второй процесс
        int result1 = fork(); // создаем 3-ий процесс из второго
        if (result1 < 0) {
            printf("Can\'t fork child\n");
            exit(-1);
        } else if (result1 > 0) { // второй процесс
            if ((fd1 = open(name1, O_RDONLY)) < 0) {
                printf("Can\'t open FIFO for reading\n");
                exit(-1);
            }
            size = read(fd1, str_buf, mes_size); // считываем из первого канала

            if (size < 0) {
                printf("Can\'t read string from FIFO\n");
                exit(-1);
            }
            count_chars(str_buf); // выполняем преобразование строки
            if (close(fd1) < 0) {
                printf("child: Can\'t close FIFO\n");
                exit(-1);
            }
            if ((fd2 = open(name2, O_WRONLY)) < 0) {
                printf("Can\'t open FIFO for writting\n");
                exit(-1);
            }

            size = write(fd2, str_buf, mes_size); // записываем во второй канал

            if (size != mes_size) {
                printf("Can\'t write all string to FIFO\n (size = %d)", size);
                exit(-1);
            }
            if (close(fd2) < 0) {
                printf("parent: Can\'t close FIFO\n");
                exit(-1);
            }
        } else { // 3-ий процесс
            if ((fd2 = open(name2, O_RDONLY)) < 0) {
                printf("Can\'t open FIFO for reading\n");
                exit(-1);
            }

            size = read(fd2, str_buf, mes_size);// считываем из 2-го канала
            if (size < 0) {
                printf("Can\'t read string from FIFO\n");
                exit(-1);
            }
            if (close(fd2) < 0) {
                printf("Can\'t close FIFO\n");
                exit(-1);
            }
            int file_to_write = 0;

            if ((file_to_write = open(argv[2], O_WRONLY | O_CREAT, 0666)) < 0) {
                printf("Can\'t open file\n");
                exit(-1);
            }
            size = write(file_to_write, str_buf, strlen(str_buf)); // записываем в файл
            if (size != strlen(str_buf)) {
                printf("Can\'t write all string\n");
                exit(-1);
            }
            if (close(file_to_write) < 0) {
                printf("Can\'t close file\n");
            }
        }
    }
    return 0;
}
