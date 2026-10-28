# Учебный репозиторий

Место для хранения решений, заметок и прочих следов учебного процесса

Ничего серьёзного - просто учёба, код и немного академического хаоса

## 2024

### Численные методы (C)

- **Числа с плавающей точкой / IEEE 754**: [биты](2024/ieee754/ieee754_bits.c), [минимум / эпсилон / шаг](2024/ieee754/ieee754_12.c), [своя реализация exp](2024/ieee754/ieee754_15.c), [свои cos / sin](2024/ieee754/ieee754_19.c), [cos без приведения аргумента](2024/ieee754/ieee754_19-.c), [ноль / бесконечность / NaN](2024/ieee754/ieee754_exceptions.c)
- **Поиск корней (метод хорд, метод Ньютона)**: [03.03.c](2024/03.03.c)
- **Минимизация функции одной переменной**: [15.05.c](2024/15.05.c)
- **Численное интегрирование**: [Ньютон - Котес](2024/derivation/kotes.c), [Марков](2024/derivation/markov.c), [составные квадратурные формулы с фиксированным N](2024/derivation/composite_quadrature_07.04.c), [составные адаптивные квадратурные формулы](2024/derivation/composite_quadrature_18.04.c)
- **Интерполяция (линейный сплайн)**: [linear_spline.c](2024/linear_spline.c)
- **Линейная алгебра / матрицы**: [метод Гаусса](2024/matrix/gaussian_elimination.c), [матрица Гильберта](2024/matrix/t.c), [схемы заполнения и обратные матрицы](2024/matrix/30.04.c), [операции с матрицами](2024/matrix/05.05.c)
- **Итоговая работа (адаптивное интегрирование для $\displaystyle\int_0^x\frac{\sin(t)+1}{\sqrt{t}}=\alpha $**: [код](2024/final/code.c), [анализ](2024/final/analysis.md)

### Алгоритмы и геометрия (C)

- **Сортировка**: [quickSort.c](2024/quickSort.c)
- **Вычислительная геометрия (точка в многоугольнике)**: [1-8-11.c](2024/1-8-11.c)

## 2025

### Основы C++ / ООП

- **Классы, конструкторы, дружественные функции**: [02.10.cpp](2025/02.10.cpp), [19.09.cpp](2025/19.09.cpp), [22.09.cpp](2025/22.09.cpp), [25.09-test.cpp](2025/25.09-test.cpp)
- **Класс динамического массива, обработка ошибок**: [29.09.cpp](2025/29.09.cpp)
- **Работа с файлами, пространства имён**: [15.09.cpp](2025/15.09.cpp)
- **Черновики (побитовые сдвиги)**: [test.cpp](2025/test.cpp)

### Структуры данных (C++)

- **Стек / очередь / список**: [stack.cpp](2025/stack.cpp), [queue.cpp](2025/queue.cpp), [list.cpp](2025/list.cpp)
- **Битовые множества**: [bitset.cpp](2025/bitset.cpp), [bitset1.cpp](2025/bitset1.cpp)
- **Хеширование**: [hashing.cpp](2025/hashing.cpp), [hashing03.cpp](2025/hashing03.cpp)
- **Бинарное дерево**: [binary-tree.cpp](2025/binary-tree/binary-tree.cpp), [binary-tree.hpp](2025/binary-tree/binary-tree.hpp), [tree.cpp](2025/binary-tree/tree.cpp), [экспорт в HTML](2025/binary-tree/binary-tree-html.cpp), [заголовочный файл](2025/binary-tree/binary-tree-html.hpp)
- **Красно-чёрное дерево**: [red-black-tree.cpp](2025/red-black-tree/red-black-tree.cpp)
- **Обработка текста**: [getword.cpp](2025/get-word/getword.cpp)

### Решение задач (C++)

- **Школьные задачи**: [1](2025/vos/school/1.cpp), [2](2025/vos/school/2.cpp), [3](2025/vos/school/3.cpp), [4](2025/vos/school/4.cpp)
- **Задачи с файлами**: [1.4](2025/vos/content/1.4.cpp), [2.10](2025/vos/content/2.10.cpp), [3.5](2025/vos/content/3.5.cpp), [4.1](2025/vos/content/4.1.cpp), [5.6 (наименьшая охватывающая окружность)](2025/vos/content/5.6.cpp)

### Алгоритмы на графах (Python)

- **Минимальное остовное дерево (алгоритм Краскала)**: [final4.py](2025/python/final4.py)
- **Алгоритм Дейкстры (файл содержит копию кода Краскала)**: [dijkstra's-algorithm.py](2025/python/dijkstra%27s-algorithm.py)
- **Чтение файлов и статистика**: [file-reader.py](2025/python/file-reader.py)

### Unix / системное программирование (C)

- **Процессы и сигналы**: [fork.c](2025/unix/fork.c), [wait_perror_signal.c](2025/unix/wait_perror_signal.c)
- **Потоки и синхронизация**: [thread.c](2025/unix/thread.c), [sync.c](2025/unix/sync.c), [задача об обедающих философах](2025/unix/dining_philosophers.c), [задача об обедающих философах (версия с блокировками)](2025/unix/dining_philosophers_l.c), [c5.c](2025/unix/t/c5.c)
- **Работа с файлами и каталогами**: [rights.c](2025/unix/rights.c), [ls1.c](2025/unix/t/ls1.c)
- **Каналы и FIFO**: [pipe.c](2025/unix/t/pipe.c), [test.c](2025/unix/t/test.c), [test2.c](2025/unix/t/test2.c)
- **Разделяемая память и семафоры**: [c3.c](2025/unix/t/c3.c), [c4.c](2025/unix/t/c4.c), [c6.c](2025/unix/t/c6.c), [c6a.c](2025/unix/t/c6a.c), [c7.c](2025/unix/t/c7.c), [c7a.c](2025/unix/t/c7a.c), [c8.c](2025/unix/t/c8.c)
- **Файлы, отображаемые в память**: [c10.c](2025/unix/t/c10.c)
- **Сокеты**: [client.c](2025/unix/t/sockets/client.c), [server.c](2025/unix/t/sockets/server.c)

## 2026

- **Первое использование CMAKE** [first_cmake](first_cmake)
