#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#define column  4
#define row  4

int main()
{
	int  sum = 0, c;
	int matrix[row][column] = { {1,2,3,4},{5,6,7,8} }; //= (int **)malloc(row * sizeof(int*));
	LARGE_INTEGER timerFrequency, timerStart, timerStop;
	double time;
	QueryPerformanceFrequency(&timerFrequency);
	QueryPerformanceCounter(&timerStart);

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++)
		{
			matrix[i][j] = rand() % 4000;
			sum += matrix[i][j];
			
		}

	}
	QueryPerformanceCounter(&timerStop);
	time = (double)(timerStop.QuadPart - timerStart.QuadPart) / (double)timerFrequency.QuadPart;
	printf_s("sum(C)=%d\n",sum);
	printf("time %f\n", time);

	int iterations = row*column;
	sum = 0;

	QueryPerformanceFrequency(&timerFrequency);
	QueryPerformanceCounter(&timerStart);

	_asm
	{
		pusha//; сохранить в стек все регистры
		xor     esi, esi  //
		xor     edi, edi  //
		xor     ecx, ecx
		xor     ebx,ebx
		l1:
        mov ax,[matrix+esi]
		add cx,ax
		add esi,4
		sub iterations,1
		jnz l1
		mov sum,ecx
	}
	QueryPerformanceCounter(&timerStop);
	time = (double)(timerStop.QuadPart - timerStart.QuadPart) / (double)timerFrequency.QuadPart;
	printf_s("sum(without MMX)=%d\n",sum);
	printf("time %f\n", time);
	short str1[column];
	short str2[column];

	QueryPerformanceFrequency(&timerFrequency);
	QueryPerformanceCounter(&timerStart);

	iterations = row*column;
	sum = 0;
	_asm
	{
		pusha             //; сохранить в стек все регистры
		xor     esi, esi
		xor     ebx, ebx
		xor     edx, edx
		pxor    MM7, MM7
		mov     ecx, row*column/2 
		l2:
		paddd  MM7,[matrix+esi]
		add esi,8
		loop l2
		
		movd eax, MM7 //перемещение двойного слова
		psrlq MM7, 1 //логический сдвиг вправо на 4 байта
		movd ebx, MM7 //

		add eax, ebx //в eax - сумма по текущей строке
		add sum, eax //заполнение результирующего массива
		emms  //отключение режима mmx
		popa //возврат регистров общего назначения из стека
	}
	QueryPerformanceCounter(&timerStop);
	time = (double)(timerStop.QuadPart - timerStart.QuadPart) / (double)timerFrequency.QuadPart;
	printf_s("sum(MMX)=%d\n", sum);
	printf("time %f\n", time);
	_getch();
	return 0;
}