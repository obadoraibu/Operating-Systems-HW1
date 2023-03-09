#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

const int mes_size = 5000;

void count_chars(char *str) {
    int freq[128] = {0}; // инициализация массива нулями

    for(int i = 0; str[i] != '\0'; i++) {
        freq[(int)str[i]]++; // инкремент частоты символа
    }

    char output[mes_size]; // создаем пустую строку для вывода
    for(int i = 0; i < 128; i++) {
        if(freq[i] > 0) {
            char symbol[10];
            sprintf(symbol, "%c:%d\n", i, freq[i]); // записываем символ и его частоту в строку
            strcat(output, symbol); // добавляем символ и его частоту к строке вывода
        }
    }

    strcpy(str, output); // копируем строку вывода обратно во входную строку
}


int main(int argc, char *argv[]) {
    char str_buf[mes_size];
    if (argc != 3) {
        printf("./prog.out read.txt write.txt\n");
        return 0;
    }

    int fd1[2], fd[2], result;
    int size;

    if (pipe(fd) < 0) { // Создаем первый канал(для передачи первый->второй процесс) и записываем его значение
        printf("Can\'t open pipe\n");
        exit(-1);
    }
    if (pipe(fd1) < 0) { // Создаем второй канал(для передачи второй->третий процесс) и записываем его значение
        printf("Can\'t open pipe\n");
        exit(-1);
    }

    result = fork(); // создаем первый и второй процессы
    if (result < 0) {
        printf("Can\'t fork child\n");
        exit(-1);
    } else if (result > 0) { // Первый процесс
        if (close(fd[0]) < 0) {
            printf("parent: Can\'t close reading side of pipe\n");
            exit(-1);
        }
        //char* fcontent = NULL;
        int file_to_read = 0;

        if ((file_to_read = open(argv[1], O_RDONLY, 0666)) < 0) {
            printf("Can\'t open file\n");
            exit(-1);
        }
        size = read(file_to_read, str_buf, mes_size); // считываем из файла
        if (close(file_to_read) < 0) {
            printf("Can\'t close file\n");
        }
        size = write(fd[1], str_buf, mes_size); // записываем в первый канал
        if (size != mes_size) {
            printf("Can\'t write all string to pipe\n (size = %d)", size);
            exit(-1);
        }
        if (close(fd[1]) < 0) {
            printf("parent: Can\'t close writing side of pipe\n");
            exit(-1);
        }

    } else {  // второй процесс
        int result1 = fork(); // создаем 3-ий процесс из второго
        if (result1 < 0) {
            printf("Can\'t fork child\n");
            exit(-1);
        } else if (result1 > 0) { // второй процесс
            if (close(fd[1]) < 0) {
                printf("child: Can\'t close writing side of pipe\n");
                exit(-1);
            }
            size = read(fd[0], str_buf, mes_size); // считываем из первого канала
            if (size < 0) {
                printf("Can\'t read string from pipe\n");
                exit(-1);
            }
            count_chars(str_buf); // выполняем преобразование строки
            if (close(fd[0]) < 0) {
                printf("child: Can\'t close reading side of pipe\n");
                exit(-1);
            }

            //mes_size = strlen(str_buf);
            size = write(fd1[1], str_buf, mes_size); // записываем во второй канал

            if (size != mes_size) {
                printf("Can\'t write all string to pipe\n (size = %d)", size);
                exit(-1);
            }
            if (close(fd1[1]) < 0) {
                printf("parent: Can\'t close writing side of pipe\n");
                exit(-1);
            }

        } else { // 3-ий процесс
            if (close(fd1[1]) < 0) {
                printf("child: Can\'t close writing side of pipe\n");
                exit(-1);
            }

            size = read(fd1[0], str_buf, mes_size);// считываем из 2-го канала
            if (size < 0) {
                printf("Can\'t read string from pipe\n");
                exit(-1);
            }
            if (close(fd1[0]) < 0) {
                printf("child: Can\'t close reading side of pipe\n");
                exit(-1);
            }
            int file_to_write = 0;

            if ((file_to_write = open(argv[2], O_WRONLY | O_CREAT, 0666)) < 0) {
                printf("Can\'t open file\n");
                exit(-1);
            }
            printf("%s \n", str_buf);
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
