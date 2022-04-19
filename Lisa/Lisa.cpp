#include <iostream>
#include <windows.h>
using namespace std;

int main()
{
	setlocale(LC_ALL, "russian");
	int arraySize;
	int maxNumA;
	int reverseArrayNum;
	int offset = 0;

	cout << "Введите размерность массива: ";
	cin >> arraySize;
	reverseArrayNum = arraySize;
	int* arrayInt = new int[arraySize];
	int* tmp = new int[arraySize];

	cout << "Введите " << arraySize << " чесел:" << endl;
	for (int i = 0; i < arraySize; i++)
		cin >> arrayInt[i];

	cout << "Размерность массива: " << arraySize << endl;
	cout << "Массив: ";
	for (int i = 0; i < arraySize; i++)
		cout << arrayInt[i] << " ";
	cout << endl;

	cout << "Введите максимальное число A: ";
	cin >> maxNumA;

	for (int i = 0; i < arraySize; i++) {
		int num = arrayInt[i];
		if (num < maxNumA) {
			tmp[offset] = num;
			offset++;
		}
		else {
			reverseArrayNum--;
			tmp[reverseArrayNum] = num;
		}
	}
	arrayInt = tmp;

	cout << "Массив после изменения: ";
	for (int i = 0; i < arraySize; i++)
		cout << arrayInt[i] << " ";

	cout << endl;
	system("pause");
}