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

	cout << "Введите максимальное число A: ";	//
	cin >> maxNumA;								// Вводим число A

	for (int i = 0; i < arraySize; i++) {	//
		int num = arrayInt[i];				//
		if (num < maxNumA) {				// Ищем элементы меньше A
			tmp[offset] = num;				// Записываем в лево
			offset++;						//
		}									//
		else {								//
			reverseArrayNum--;				//
			tmp[reverseArrayNum] = num;		// Записываем в право
		}									//
	}										//
	arrayInt = tmp;							// Возвращение значения из временного массива

	cout << "Массив после изменения: ";	//
	for (int i = 0; i < arraySize; i++)	// Выводим итоговый массив
		cout << arrayInt[i] << " ";		//

	cout << endl;
	system("pause");
}