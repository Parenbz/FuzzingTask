# FuzzingTask

Фаззинг [cmft](https://github.com/dariomanesku/cmft/tree/master) для .dds файлов на Linux_x64. Бинарник собран с CC=afl-clang-fast CXX=afl-clang-fast++, с Address Sanitizer. 

Подключение кастомных мутаторов:
```
export AFL_CUSTOM_MUTATOR_LIBRARY="/path/to/FuzzingTask/bin/custom_mutators/bit_flip.so;/path/to/FuzzingTask/bin/custom_mutators/byte_flip.so;/path/to/FuzzingTask/bin/custom_mutators/arithmetics.so"
export AFL_CUSTOM_MUTATOR_ONLY=1
```

Первый DWORD обозначает DDS, второй DWORD всегда равен 124, 20-й DWORD всегда равен 32. Мутаторы оставляют эти значения неизменными.

Фаззинг на одном процессе:
```
cd bin
afl-fuzz -i afl-in -o afl-out -s 123 -- ./cmftRelease --input @@ --outputNum 1 --output0 out
```

Результат через 14 минут

<img src="https://github.com/Parenbz/FuzzingTask/blob/master/res/Single.jpg">

Фаззинг на 5 процессах (каждая команда в отдельном терминале)

```
afl-fuzz -i afl-in -o afl-out -s 123 -M master -- ./cmftRelease --input @@ --outputNum 1 --output0 out
afl-fuzz -i afl-in -o afl-out -s 124 -S variant0 -- ./cmftRelease --input @@ --outputNum 1 --output0 out
afl-fuzz -i afl-in -o afl-out -s 125 -S variant1 -- ./cmftRelease --input @@ --outputNum 1 --output0 out
afl-fuzz -i afl-in -o afl-out -s 126 -S variant2 -- ./cmftRelease --input @@ --outputNum 1 --output0 out
afl-fuzz -i afl-in -o afl-out -s 127 -S variant3 -- ./cmftRelease --input @@ --outputNum 1 --output0 out
```

Результат через 14 минут:

<img src="https://github.com/Parenbz/FuzzingTask/blob/master/res/Parallel.jpg">

Виден прирост числа найденных путей.
