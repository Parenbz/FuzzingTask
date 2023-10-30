# FuzzingTask

Фаззинг [cmft](https://github.com/dariomanesku/cmft/tree/master) для .dds файлов на Linux_x64. Бинарник собран с CC=afl-clang-fast CXX=afl-clang-fast++, с Address Sanitizer. 

Подключение кастомных мутаторов:
```
export AFL_CUSTOM_MUTATOR_LIBRARY="$/path/to/FuzzingTask/bin/custom_mutators/bit_flip.so;/path/to/FuzzingTask/bin/custom_mutators/arithmetics.so"
export AFL_CUSTOM_MUTATOR_ONLY=1
```

Фаззинг на одном процессе:
```
cd bin
afl-fuzz -i afl-in -o afl-out -s 123 -- ./cmftRelease --input @@ --outputNum 1 --output0 out
```

Результат через 15 минут
