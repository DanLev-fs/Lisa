#include <iostream>
#include <windows.h>
using namespace std;

struct paramsStruct {
	int arraySize;
	int maxNumA;
	int* arrayInt;
	HANDLE hSemaphore;
};

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
	int* arrayInt = new int[arraySize];

	cout << "Введите " << arraySize << " чесел:" << endl;
	for (int i = 0; i < arraySize; i++)
		cin >> arrayInt[i];

	cout << "Размерность массива: " << arraySize << endl;
	cout << "Массив: ";
	for (int i = 0; i < arraySize; i++)
		cout << arrayInt[i] << " ";
	cout << endl;

	cout << "Введите число k <= размеру массива: ";
	cin >> k;
	if (k > arraySize)
		k = arraySize;

	cout << "Введите максимальное число A: ";
	cin >> maxNumA;

	hSemaphore = CreateSemaphore(NULL, 0, arraySize, NULL);
	if (hSemaphore == NULL)
		return GetLastError();

	struct paramsStruct* params = (struct paramsStruct*)malloc(sizeof(struct paramsStruct));
	params->arraySize = arraySize;
	params->maxNumA = maxNumA;
	params->arrayInt = new int[arraySize];
	params->arrayInt = arrayInt;
	params->hSemaphore = hSemaphore;

	hWork = CreateThread(NULL, 0, work, (void*)params, 0, &IdThread);
	if (hWork == NULL)
		return GetLastError();

	hBinSemaphore = CreateSemaphore(NULL, 0, 1, NULL);
	if (hBinSemaphore == NULL)
		return GetLastError();

	struct paramsStruct* params1 = (struct paramsStruct*)malloc(sizeof(struct paramsStruct));
	params1->arraySize = arraySize;
	params1->maxNumA = k;
	params1->arrayInt = new int[arraySize];
	params1->arrayInt = arrayInt;
	params1->hSemaphore = hBinSemaphore;

	hMultiElement = CreateThread(NULL, 0, MultiElement, (void*)params1, 0, &IdThread);
	if (hMultiElement == NULL)
		return GetLastError();

	for (int i = 0; i < arraySize; i++) {
		WaitForSingleObject(hSemaphore, INFINITE);
		cout << params->arrayInt[i] << " \a" << flush;
		if (i + 1 == k)
			ReleaseSemaphore(hBinSemaphore, 1, NULL);
	}
	cout << endl;

	arrayInt = params->arrayInt;

	cout << "Массив после изменения: ";
	for (int i = 0; i < arraySize; i++)
		cout << arrayInt[i] << " ";

	WaitForSingleObject(hWork, INFINITE);
	CloseHandle(hWork);
	WaitForSingleObject(hMultiElement, INFINITE);
	CloseHandle(hMultiElement);
	free(params);
	free(params1);

	cout << endl;
	system("pause");
}

DWORD WINAPI work(LPVOID array) {
	int sleepTime;
	cout << "Время сна в милисекундах: ";
	cin >> sleepTime;
	struct paramsStruct* params = (struct paramsStruct*)array;
	int reverseArrayNum = params->arraySize;
	int offset = 0;
	int* tmp = new int[params->arraySize];
	for (int i = 0; i < params->arraySize; i++) {
		int num = params->arrayInt[i];
		if (num < params->maxNumA) {
			params->arrayInt[offset] = num;
			offset++;
		}
		else {
			reverseArrayNum--;
			tmp[reverseArrayNum] = num;
		}
		ReleaseSemaphore(params->hSemaphore, 1, NULL);
		Sleep(sleepTime);
	}
	for (offset; offset < params->arraySize; offset++)
		params->arrayInt[offset] = tmp[offset];
	return 0;
}

DWORD WINAPI MultiElement(LPVOID array) {
	struct paramsStruct* params = (struct paramsStruct*)array;
	int k = params->maxNumA;
	WaitForSingleObject(params->hSemaphore, INFINITE);
	int res = 1;
	for (int i = 0; i <= k; i++) {
		res *= params->arrayInt[i];
	}
	cout << "Итоговое произведение: " << res << endl;
	return 0;
}