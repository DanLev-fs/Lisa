#include <iostream>
#include <windows.h>
#include <algorithm>
using namespace std;

DWORD WINAPI work(LPVOID array);
DWORD WINAPI MultiElement(LPVOID array);

struct threadParametrs {
    float* array;
    float maxNumA;
    int arraySize;
    int k;
    HANDLE hWorkSemaphore;
    HANDLE hMultiElementBinSemaphore;
    HANDLE hMultiElementBinSemaphorePrint;
};

int main()
{
    setlocale(LC_ALL, "russian");

    threadParametrs* params = (threadParametrs*)malloc(sizeof(threadParametrs));

    HANDLE hWork;           // Дескриптор потока work
    HANDLE hMultiElement;   // Дескриптор потока MultiElement

    cout << "Введите размерность массива: ";        //
    cin >> params->arraySize;                       // Задаём размер массива и
    while (params->arraySize <= 0) {                //
        cout << "Ошибка ввода" << endl;             //
        cout << "Введите размерность массива: ";    //
        cin >> params->arraySize;                   // Задаём размер массива и
    }                                               //
    params->array = new float[params->arraySize];   // создаём массив

    cout << "Введите " << params->arraySize << " чесел:" << endl;   //
    for (int i = 0; i < params->arraySize; i++)                     // Вводим числа
        cin >> params->array[i];                                    //

    cout << "Размерность массива: " << params->arraySize << endl;   //
    cout << "Массив: ";                                             //
    for (int i = 0; i < params->arraySize; i++)                     // Выводим массив
        cout << params->array[i] << " ";                            //
    cout << endl;                                                   //

    cout << "Введите число k от 0 до " << params->arraySize << ": ";        // Вводим число k
    cin >> params->k;                                                       //
    while (params->k > params->arraySize && params->k >= 0) {               //
        cout << "Ошибка ввода" << endl;                                     //
        cout << "Введите число k от 0 до " << params->arraySize << ": ";    //
        cin >> params->k;                                                   // Вводим число k
    }                                                                       //

    cout << "Введите максимальное число A: ";   //
    cin >> params->maxNumA;                     // Вводим число A

    params->hWorkSemaphore = CreateSemaphore(NULL, 0, params->arraySize, NULL); //
    if (params->hWorkSemaphore == NULL)                                         // Создаём семафор для потока work
        return GetLastError();                                                  //
    hWork = CreateThread(NULL, 0, work, (void*)params, 0, NULL);                //
    if (hWork == NULL)                                                          // Создаём поток work
        return GetLastError();                                                  //

    params->hMultiElementBinSemaphore = CreateSemaphore(NULL, 0, 1, NULL);          //
    if (params->hMultiElementBinSemaphore == NULL)                                  // Создаём бинарный семафор для потока MultiElement
        return GetLastError();                                                      //
    params->hMultiElementBinSemaphorePrint = CreateSemaphore(NULL, 0, 1, NULL);     //
    if (params->hMultiElementBinSemaphorePrint == NULL)                             // Создаём бинарный семафор для потока MultiElement
        return GetLastError();                                                      //
    hMultiElement = CreateThread(NULL, 0, MultiElement, (void*)params, 0, NULL);    //
    if (hMultiElement == NULL)                                                      // Создаём поток MultiElement
        return GetLastError();                                                      //

    for (int i = 0; i < params->arraySize; i++) {                           //
        WaitForSingleObject(params->hWorkSemaphore, INFINITE);              // Ждём сигнал нового элемента в массиве
        cout << params->array[i] << " \a" << flush;                         // и выводм элемент
        if (i + 1 == params->k)                                             // Проверяем количество прошедших элементов
            ReleaseSemaphore(params->hMultiElementBinSemaphore, 1, NULL);   // Запускаем поток MultiElement
    }                                                                       //
    cout << endl;                                                           //

    cout << "Массив после изменения: ";         //
    for (int i = 0; i < params->arraySize; i++) // Выводм иготовый массив
        cout << params->array[i] << " ";        //
    cout << endl;                               //

    ReleaseSemaphore(params->hMultiElementBinSemaphorePrint, 1, NULL);  // Запускаем поток MultiElement

    WaitForSingleObject(hWork, INFINITE);                   // Ждём завершения потоков
    WaitForSingleObject(hMultiElement, INFINITE);           //
    CloseHandle(hWork);                                     //
    CloseHandle(hMultiElement);                             //
    CloseHandle(params->hWorkSemaphore);                    //
    CloseHandle(params->hMultiElementBinSemaphore);         // Удаляем дескрипторы
    CloseHandle(params->hMultiElementBinSemaphorePrint);    //
    free(params);

    system("pause");
}

DWORD WINAPI work(LPVOID array) {
    int sleepTime;                              //
    cout << "Время сна в милисекундах: ";       // Вводим время сна между итерациями
    cin >> sleepTime;                           //
    while (sleepTime <= 0) {                    //
        cout << "Ошибка ввода" << endl;         //
        cout << "Время сна в милисекундах: ";   //
        cin >> sleepTime;                       //
    }                                           //

    threadParametrs* params = (threadParametrs*)array;
    float* tmp = new float[params->arraySize];
    int offset = 0;
    int reverseArrayNum = params->arraySize;

    sort(params->array, params->array + params->arraySize);

    for (int i = 0; i < params->arraySize; i++)	// Копируем данные во временный массив 
        tmp[i] = params->array[i];              //

    for (int i = 0; i < params->arraySize; i++) {   //
        float num = tmp[i];                         //
        if (num < params->maxNumA) {                // Ищем элементы меньше A
            params->array[offset] = num;            // Записываем в лево
            offset++;                               //
        }                                           //
        else {                                      //
            reverseArrayNum--;                      // Записываем в право
            params->array[reverseArrayNum] = num;   //
        }                                           //
    }

    for (int i = 0; i < params->arraySize; i++) {           //
        ReleaseSemaphore(params->hWorkSemaphore, 1, NULL);	// Сообщаем о завершении итерации
        Sleep(sleepTime);								    // Спим указанное время
    }                                                       //

    return 0;
}

DWORD WINAPI MultiElement(LPVOID array) {
    threadParametrs* params = (threadParametrs*)array;  //

    WaitForSingleObject(params->hMultiElementBinSemaphore, INFINITE);   // Ждём сигнал

    float res = 1;                      //
    for (int i = 0; i < params->k; i++) // Произведение
        res *= params->array[i];        //

    WaitForSingleObject(params->hMultiElementBinSemaphorePrint, INFINITE);  // Ждём сигнал
    cout << "Итоговое произведение: " << res << endl;                       // Выводм итоговое произведение

    return 0;
}
