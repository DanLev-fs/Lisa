﻿#include <iostream>
#include <windows.h>
#include <algorithm>
using namespace std;

DWORD WINAPI work(LPVOID array);
DWORD WINAPI MultiElement(LPVOID array);

int main()
{
	setlocale(LC_ALL, "russian");

	int arraySize;		// Размер массива
	float maxNumA;		// Число A 
	float k;			// Число k

	HANDLE hWork;				// Дескриптор потока work
	HANDLE hMultiElement;		// Дескриптор потока MultiElement
	HANDLE hSemaphore;			// Дескриптор семафора для потока work
	HANDLE hBinSemaphore;		// Дескриптор бинарного семафора для потока MultiElement
	HANDLE hBinSemaphorePrint;	// Дескриптор бинарного семафора для потока MultiElement

	cout << "Введите размерность массива: ";		//
	cin >> arraySize;								// Задаём размер массива
	arraySize += 6;									// и
	float* array = new float[arraySize];			// создаём массив
	array[0] = arraySize;							// В первом элементе массива указываем размер массива

	cout << "Введите " << arraySize - 6 << " чесел:" << endl;	//
	for (int i = 6; i < arraySize; i++)							// Вводим числа
		cin >> array[i];										//

	cout << "Размерность массива: " << arraySize - 5 << endl;	//
	cout << "Массив: ";											//
	for (int i = 6; i < arraySize; i++)							// Выводим массив
		cout << array[i] << " ";								//
	cout << endl;												//

	cout << "Введите число k <= " << arraySize - 6 << ": ";		//
	cin >> array[1];											// Вводим число k
	array[1] += 6;												//
	while (array[1] > arraySize) {								//
		cout << "Ошибка ввода" << endl;							//
		cout << "Введите число k <= " << arraySize- 6 << ": ";	//
		cin >> array[1];										// Вводим число k
	}

	cout << "Введите максимальное число A: ";	//
	cin >> array[2];							// Вводим число A

	hSemaphore = CreateSemaphore(NULL, 0, arraySize- 6, NULL);	//
	if (hSemaphore == NULL)										// Создаём семафор для потока work
		return GetLastError();									//
	array[3] = (int)hSemaphore;									// Сохраняем его в массив

	hWork = CreateThread(NULL, 0, work, (void*)array, 0, NULL);	//
	if (hWork == NULL)											// Создаём поток work
		return GetLastError();									//

	hBinSemaphore = CreateSemaphore(NULL, 0, 1, NULL);		//
	if (hBinSemaphore == NULL)								// Создаём бинарный семафор для потока MultiElement
		return GetLastError();								//
	array[4] = (int)hBinSemaphore;							// Сохраняем его в массив
	hBinSemaphorePrint = CreateSemaphore(NULL, 0, 1, NULL);	//
	if (hBinSemaphorePrint == NULL)							// Создаём бинарный семафор для потока MultiElement
		return GetLastError();								//
	array[5] = (int)hBinSemaphorePrint;						// Сохраняем его в массив

	hMultiElement = CreateThread(NULL, 0, MultiElement, (void*)array, 0, NULL);	//
	if (hMultiElement == NULL)													// Создаём поток MultiElement
		return GetLastError();													//

	for (int i = 6; i < arraySize; i++) {					//
		WaitForSingleObject((HANDLE)(int)array[3], INFINITE);	// Ждём сигнал нового элемента в массиве
		cout << array[i] << " \a" << flush;					// и выводм элемент
		if (i + 1 == array[1])								// Проверяем количество прошедших элементов
			ReleaseSemaphore((HANDLE)(int)array[4], 1, NULL);	// Запускаем поток MultiElement
	}														//
	cout << endl;											//

	cout << "Массив после изменения: ";	//
	for (int i = 6; i < arraySize; i++)	// Выводм иготовый массив
		cout << array[i] << " ";		//
	cout << endl;

	ReleaseSemaphore((HANDLE)(int)array[5], 1, NULL);	// Запускаем поток MultiElement

	WaitForSingleObject(hWork, INFINITE);			// Ждём завершения потоков
	WaitForSingleObject(hMultiElement, INFINITE);	//
	CloseHandle(hWork);								//
	CloseHandle(hMultiElement);						// Удаляем дескрипторы

	system("pause");
}

DWORD WINAPI work(LPVOID array) {
	int sleepTime;							//
	cout << "Время сна в милисекундах: ";	// Вводим время сна между итерациями
	cin >> sleepTime;						//

	float* arrayL = (float*)array;			// Приводим LPVOID к int*
	int reverseArrayNum = arrayL[0];		// 
	int offset = 6;							//
	float* tmp = new float[arrayL[0] - 6];	// Создаём временный массив

	for (int i = 0; i < arrayL[0] - 6; i++)	// Копируем данные во временный массив 
		tmp[i] = arrayL[i + 6];				//

	for (int i = 0; i < arrayL[0]- 6; i++) {					//
		float num = tmp[i];										//
		if (num < arrayL[2]) {									// Ищем элементы меньше A
			arrayL[offset] = num;								// Записываем в лево
			offset++;											//
			ReleaseSemaphore((HANDLE)(int)arrayL[3], 1, NULL);		// Сообщаем о завершении итерации
			Sleep(sleepTime);									// Спим указанное время
		}														//
	}															//

	for (int i = 0; i < arrayL[0] - 6; i++) {					//
		float num = tmp[i];										//
		if (num == arrayL[2]) {									// Проверяем элементы равные A
			arrayL[offset] = num;								// Записываем
			offset++;											//
			ReleaseSemaphore((HANDLE)(int)arrayL[3], 1, NULL);	// Сообщаем о завершении итерации
			Sleep(sleepTime);									// Спим указанное время
		}														//
	}															//

	for (int i = 0; i < arrayL[0] - 6; i++) {					//
		float num = tmp[i];										//
		if (num > arrayL[2]) {									// Ищем элементы больше A
			arrayL[offset] = num;								// Записываем в право
			offset++;											//
			ReleaseSemaphore((HANDLE)(int)arrayL[3], 1, NULL);	// Сообщаем о завершении итерации
			Sleep(sleepTime);									// Спим указанное время
		}														//
	}															//

	return 0;
}

DWORD WINAPI MultiElement(LPVOID array) {
	float* arrayL = (float*)array;	// Приводим LPVOID к int*
	int k = arrayL[1];			//

	WaitForSingleObject((HANDLE)(int)arrayL[4], INFINITE);	// Ждём сигнал

	float res = 1;					//
	for (int i = 6; i < k; i++)	// Произведение
		res *= arrayL[i];			//

	WaitForSingleObject((HANDLE)(int)arrayL[5], INFINITE);	// Ждём сигнал
	cout << "Итоговое произведение: " << res << endl;	// Выводм итоговое произведение
	return 0;
}