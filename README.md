# Operating-Systems-HW1 <br/> Выполнил Федоров Артём Олегович БПИ217 (на 8 баллов). <br/> Вариант 19

## Использование каналов (pipes) на C. Выполнена на 9 баллов <br/>

__Условие. Вариант 19:__ </br>
Разработать программу, вычисляющую число вхождений
различных отображаемых символов в заданной ASCII-строке.


---- 

## Структура программы:

* Выполнено 6 программ, каждая расположена в своей папке ___code<_N_>___, где _N_ = [4, 9]. </br>
* Каждая программа соответствует задаче на N баллов. </br>
* Функция из задания вынесена в отдельный файл: ___lib.c___

### Запуск программ:

Для тестирования и запуска программ необходимо,
находясь на самом высоком уровне, ввести в терминал:

> ./code<_N_>/code<_N_>.o ./tests/input<_i_>.txt ./code4/outputs/output<_i_>.txt </br>

Где _N_ = [4, 9], а _i_ = [1, 5]

<!-- __Заметка 1:__ -->

# Отчёт 4 балла:

### 1. Приведено решение задачи на C:

> [code4.c](https://github.com/ArtemFed/Operating-Systems-HW1/tree/main/code4)

### 2. Запуск программы:

Для компиляции, находясь в самой верхней папке проекта, нужно ввести (необязательно):
> gcc code4/code4.c -o code4/code4.exe <br>

Для запуска уже есть готовый исполняемый файл, его можно запустить так:
> ./code4/code4.exe ./tests/input1.txt ./code4/outputs/output1.txt <br>

### 3. Схема решаемой задачи:

> __Первый процесс:__ читает текстовые данные из заданного файла и через неименованный канал 1
> передает их __Второму процессу__. </br></br>
> __Второй процесс:__ (ребёнок первого)осуществляет обработку полученных из канала 1 данных в
> соответствии с заданием и передает результат обработки через неименованный канал 2 __Третьему
> процессу__. </br></br>
> __Третий процесс:__ (ребёнок второго) осуществляет вывод полученных из канала 2 данных в заданный
> файл вывода. </br>
<br>

----

# Отчёт 5 баллов:

### 1. Приведено решение задачи на C:

> [code5.c](https://github.com/ArtemFed/Operating-Systems-HW1/tree/main/code5)

### 2. Запуск программы:

Для компиляции, находясь в самой верхней папке проекта, нужно ввести (необязательно):
> gcc code5/code5.c -o code5/code5.exe <br>

Для запуска уже есть готовый исполняемый файл, его можно запустить так:
> ./code5/code5.exe ./tests/input2.txt ./code4/outputs/output2.txt <br>

### 3. Схема решаемой задачи:

> __Первый процесс:__ читает текстовые данные из заданного файла и через именованный канал
> (pipe1.fifo) передает их __Второму процессу__. </br></br>
> __Второй процесс:__ (ребёнок первого) осуществляет обработку полученных из канала (pipe1.fifo)
> данных в соответствии с заданием и передает результат обработки через именованный канал
> (pipe2.fifo) __Третьему процессу__. </br></br>
> __Третий процесс:__ осуществляет вывод полученных из канала (pipe2.fifo) данных в заданный
> файл вывода. </br>

----

# Отчёт 6 баллов:

### 1. Приведено решение задачи на C:

> [code6.c](https://github.com/ArtemFed/Operating-Systems-HW1/tree/main/code6)

### 2. Запуск программы:

Для компиляции, находясь в самой верхней папке проекта, нужно ввести (необязательно):
> gcc code6/code6.c -o code6/code6.exe <br>

Для запуска уже есть готовый исполняемый файл, его можно запустить так:
> ./code6/code6.exe ./tests/input3.txt ./code4/outputs/output3.txt <br>

### 3. Схема решаемой задачи:

> __Первый процесс:__ читает текстовые данные из заданного файла
> и через неименованный канал 1 передает их __Второму процессу__. </br></br>
> __Второй процесс:__ (ребёнок первого) осуществляет обработку данных в соответствии
> с заданием и передает результат обработки через неименованный канал 2 обратно __Первому
> процессу__. </br></br>
> __Опять первый процесс:__ висит, пока его ребенок - __Второй Процесс__ не запишет что-то во второй
> канал, затем он считает данные из второго канала и запишет их в заданный файл вывода.

---- 

# Отчёт 7 баллов:

### 1. Приведено решение задачи на C:

> [code7.c](https://github.com/ArtemFed/Operating-Systems-HW1/tree/main/code7)

### 2. Запуск программы:

Для компиляции, находясь в самой верхней папке проекта, нужно ввести (необязательно):
> gcc code7/code7.c -o code7/code7.exe <br>

Для запуска уже есть готовый исполняемый файл, его можно запустить так:
> ./code7/code7.exe ./tests/input4.txt ./code4/outputs/output4.txt <br>

### 3. Схема решаемой задачи:

> __Первый процесс:__ читает текстовые данные из заданного файла и через именованный канал
> (pipe1.fifo) передает их __Второму процессу__. </br></br>
> __Второй процесс:__ (ребёнок первого) осуществляет обработку полученных из канала (pipe1.fifo)
> данных в соответствии с заданием и передает результат обработки через именованный канал
> (pipe2.fifo) обратно __Первому процессу__. </br></br>
> __Опять первый процесс:__ висит, пока его ребенок - __Второй Процесс__  не запишет что-то во
> __Второй канал__ (pipe2.fifo), затем он считает данные из второго канала (pipe2.fifo) и запишет их
> в заданный файл вывода.

---- 

# Отчёт 8 баллов:

### 1. Приведено решение задачи на C:

> [code8_main.c и code8_help.c](https://github.com/ArtemFed/Operating-Systems-HW1/tree/main/code8)

### 2. Запуск программы:

Для компиляции, находясь в самой верхней папке проекта, нужно ввести (необязательно):
> gcc code8/code8_main.c -o code8/code8_main.exe <br>
> gcc code8/code8_help.c -o code8/code8_help.exe

Для запуска уже есть готовые исполняемые файлы, их можно запустить в любом порядке:
> ./code8/code8_main.exe ./tests/input5.txt ./code4/outputs/output5.txt <br>
> ./code8/code8_help.exe

### 3. Схема решаемой задачи:

> __Первый процесс (файл code8_main.c)__: читает текстовые данные из заданного файла
> и через именованный канал (pipe1.fifo) передает их __Второму процессу__. </br></br>
> __Второй процесс (файл code8_help.c)__: (независимый процесс в отдельном терминале) осуществляет
> обработку полученных из канала (pipe1.fifo) данных в соответствии с заданием и передает результат обработки через
> именованный канал (pipe2.fifo) обратно __Первому процессу__. </br></br>
> __Опять первый процесс:__ висит, пока __Второй незвисимый Процесс__
> не запишет что-то во второй канал (pipe2.fifo), затем он считает данные из второго канала
> (pipe2.fifo) и запишет их в заданный файл вывода.

# Отчёт 9 баллов:

### 1. Приведено решение задачи на C:

> [code9_main.c и code9_help.c](https://github.com/ArtemFed/Operating-Systems-HW1/tree/main/code9)

### 2. Запуск программы:

Для компиляции, находясь в самой верхней папке проекта, нужно ввести (необязательно):
> gcc code9/code9_main.c -o code9/code9_main.exe <br>
> gcc code9/code9_help.c -o code9/code9_help.exe

Для запуска уже есть готовые исполняемые файлы, их можно запустить в любом порядке:
> ./code9/code9_main.exe ./tests/input6.txt ./code4/outputs/output6.txt <br>
> ./code9/code9_help.exe

### 3. Схема решаемой задачи:

> __Первый процесс (файл code9_main.c)__: читает текстовые данные из заданного файла
> и через именованный канал (pipe1.fifo) порционно по 20 символов передает их __Второму процессу__.
> </br></br>
> __Второй процесс (файл code9_help.c)__: (независимый процесс в отдельном терминале) порционно получает
> данные по 20 символов из канала (pipe1.fifo) и сразу же обрабатывает порции в соответствии с заданием и
> после полной обработки передает результат обработки через именованный канал (pipe2.fifo) обратно 
> __Первому процессу__. </br></br>
> __Опять первый процесс:__ висит, пока __Второй незвисимый Процесс__
> не запишет что-то во второй канал (pipe2.fifo), затем он считает данные из второго канала
> (pipe2.fifo) и запишет их в заданный файл вывода.

<br>
Всё!
