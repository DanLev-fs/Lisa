#include <iostream>
#include <windows.h>
#include <algorithm>
using namespace std;

DWORD WINAPI work(LPVOID array);
DWORD WINAPI MultiElement(LPVOID array);

int main()
{
	setlocale(LC_ALL, "russian");

	int arraySize;
	int maxNumA;
	int k;

	HANDLE hWork;
	HANDLE hMultiElement;
	HANDLE hSemaphore;
	HANDLE hBinSemaphore;
	DWORD IdThread;

	cout << "Введите размерность массива: ";
	cin >> arraySize;
	arraySize += 5;
	int* array = new int[arraySize];
	array[0] = arraySize;

	cout << "Введите " << arraySize-5 << " чесел:" << endl;
	for (int i = 5; i < arraySize; i++)
		cin >> array[i];

	cout << "Размерность массива: " << arraySize-5 << endl;
	cout << "Массив: ";
	for (int i = 5; i < arraySize; i++)
		cout << array[i] << " ";
	cout << endl;

	cout << "Введите число k <= размеру массива: ";
	cin >> array[1];
	array[1] += 5;
	if (array[1] > arraySize)
		array[1] = arraySize;

	cout << "Введите максимальное число A: ";
	cin >> array[2];

	hSemaphore = CreateSemaphore(NULL, 0, arraySize-5, NULL);
	if (hSemaphore == NULL)
		return GetLastError();
	array[3] = (int)hSemaphore;

	hWork = CreateThread(NULL, 0, work, (void*)array, 0, &IdThread);
	if (hWork == NULL)
		return GetLastError();

	hBinSemaphore = CreateSemaphore(NULL, 0, 1, NULL);
	if (hBinSemaphore == NULL)
		return GetLastError();
	array[4] = (int)hBinSemaphore;

	hMultiElement = CreateThread(NULL, 0, MultiElement, (void*)array, 0, &IdThread);
	if (hMultiElement == NULL)
		return GetLastError();

	for (int i = 5; i < arraySize; i++) {
		WaitForSingleObject((HANDLE)array[3], INFINITE);
		cout << array[i] << " \a" << flush;
		if (i + 1 == array[1])
			ReleaseSemaphore((HANDLE)array[4], 1, NULL);
	}
	cout << endl;

	cout << "Массив после изменения: ";
	for (int i = 5; i < arraySize; i++)
		cout << array[i] << " ";

	WaitForSingleObject(hWork, INFINITE);
	CloseHandle(hWork);
	WaitForSingleObject(hMultiElement, INFINITE);
	CloseHandle(hMultiElement);

	cout << endl;
	system("pause");
}

DWORD WINAPI work(LPVOID array) {
	int sleepTime;
	cout << "Время сна в милисекундах: ";
	cin >> sleepTime;
	int* arrayL = (int*)array;
	int reverseArrayNum = arrayL[0];
	int offset = 5;
	int* tmp = new int[arrayL[0]-5];
	for (int i = 0; i < arrayL[0] - 5; i++)
		tmp[i] = arrayL[i + 5];
	for (int i = 0; i < arrayL[0]-5; i++) {
		int num = tmp[i];
		if (num < arrayL[2]) {
			arrayL[offset] = num;
			offset++;
		}
		else {
			reverseArrayNum--;
			arrayL[reverseArrayNum] = num;
		}
		ReleaseSemaphore((HANDLE)arrayL[3], 1, NULL);
		Sleep(sleepTime);
	}
	return 0;
}

DWORD WINAPI MultiElement(LPVOID array) {
	int* arrayL = (int*)array;
	int k = arrayL[1]-1;
	WaitForSingleObject((HANDLE)arrayL[4], INFINITE);
	int res = 1;
	for (int i = 5; i <= k; i++) {
		res *= arrayL[i];
	}
	cout << "Итоговое произведение: " << res << endl;
	return 0;
}