# hw1

Нужно реализовать класс Process, согласно интерфейсу. Класс представляет собой процесс операционной системы Linux.
При создании экземпляра класса создается дочерний процесс, при уничтожении переменной все ресурсы освобождаются, процесс завершается.
Класс позволяет отправлять данные на стандартный поток ввода процесса и читать данные с его стандартного потока вывода.
Все исключительные ситуации должны быть корректно описаны и передавать управление вызывающему коду

### Интерфейс процесса

```c++
#ifndef PROCESS_H
#define PROCESS_H


class Process
{
public:
    explicit Process(const std::string& path);
    ~Process();


    size_t write(const void* data, size_t len);
    void writeExact(const void* data, size_t len);
    size_t read(void* data, size_t len);
    void readExact(void* data, size_t len);


    bool isReadable() const;
    void closeStdin();


    void close();
};


#endif //PROCESS_H
```

