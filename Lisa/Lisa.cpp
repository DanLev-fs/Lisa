#include <iostream>
#include <windows.h>
using namespace std;

int main()
{
	setlocale(LC_ALL, "russian");

	int arraySize;			// Размер массива
	int maxNumA;			// Число A
	int reverseArrayNum;	//
	int offset = 0;			//
	int k;

	cout << "Введите размерность массива: ";	// Задаём размер масива
	cin >> arraySize;							//
	reverseArrayNum = arraySize;				//
	int* arrayInt = new int[arraySize];			// Создаём массивы
	int* tmp = new int[arraySize];				//

	cout << "Введите " << arraySize << " чесел:" << endl;	//
	for (int i = 0; i < arraySize; i++)						// Вводим числа
		cin >> arrayInt[i];									//

	cout << "Размерность массива: " << arraySize << endl;	//
	cout << "Массив: ";										//
	for (int i = 0; i < arraySize; i++)						// Выводим данные о массиве
		cout << arrayInt[i] << " ";							//
	cout << endl;											//

	cout << "Введите число k <= " << arraySize << ": ";		//
	cin >> k;													// Вводим число k
	while (k > arraySize) {										//
		cout << "Ошибка ввода" << endl;							//
		cout << "Введите число k <= " << arraySize << ": ";	//
		cin >> k;												// Вводим число k
	}

	cout << "Введите максимальное число A: ";	//
	cin >> maxNumA;								// Вводим число A

	int sleepTime;							//
	cout << "Время сна в милисекундах: ";	// Вводим время сна между итерациями
	cin >> sleepTime;						//

	for (int i = 0; i < arraySize; i++) {	//
		int num = arrayInt[i];				//
		if (num < maxNumA) {				// Ищем элементы меньше A
			tmp[offset] = num;				// Записываем в лево
			offset++;						//
			Sleep(sleepTime);				// Спим указанное время
		}									//
	}										//

	for (int i = 0; i < arraySize; i++) {	//
		int num = arrayInt[i];				//
		if (num == maxNumA) {				// Проверяем элементы равные A
			tmp[offset] = num;				// Записываем
			offset++;						//
			Sleep(sleepTime);				// Спим указанное время
		}									//
	}										//

	for (int i = 0; i < arraySize; i++) {	//
		int num = arrayInt[i];				//
		if (num > maxNumA) {				// Ищем элементы больше A
			tmp[offset] = num;				// Записываем в право
			offset++;						//
			Sleep(sleepTime);				// Спим указанное время
		}									//
	}										//
	arrayInt = tmp;							// Возвращение значения из временного массива

	int res = 1;					//
	for (int i = 0; i < k; i++)		// Произведение
		res *= arrayInt[i];			//

	cout << "Итоговое произведение: " << res << endl;	// Выводм итоговое произведение

	cout << "Массив после изменения: ";	//
	for (int i = 0; i < arraySize; i++)	// Выводим итоговый массив
		cout << arrayInt[i] << " ";		//

	cout << endl;
	system("pause");
}