#include <stdio.h>
#include <string.h>

const int mes_size = 5000;

void count_chars(char *str) {
    int freq[128] = {0}; // инициализация массива нулями

    for (int i = 0; str[i] != '\0'; i++) {
        freq[(int) str[i]]++; // инкремент частоты символа
    }

    char output[mes_size]; // создаем пустую строку для вывода
    for (int i = 0; i < 128; i++) {
        if (freq[i] > 0) {
            char symbol[10];
            sprintf(symbol, "%c:%d\n", i, freq[i]); // записываем символ и его частоту в строку
            strcat(output, symbol); // добавляем символ и его частоту к строке вывода
        }
    }

    strcpy(str, output); // копируем строку вывода обратно во входную строку
}
