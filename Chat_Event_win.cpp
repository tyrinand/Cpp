#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <bitset>
using namespace std;

HANDLE sync;	//событие синхронизации
HANDLE e[8];	//события передачи

DWORD Get_M() // функция потока для считывания
{
	bitset<8> temp; // битовое поле 8 бит
	while(true) // бескончный цикл пока запущено приложение
	{
		WaitForSingleObject(sync,INFINITE); // ожидание события синхронизации отправки синхронизации 
		for (int i=0; i<8; i++) // расшифровка одного символа
		{
			if (WaitForSingleObject(e[i],0)==WAIT_OBJECT_0) // i-тое событие сингнальное 
				temp[i]=1;// i- бит установить 1
			else
				temp[i]=0;// i-бит в ноль
		}
		char a=temp.to_ulong(); // преобразование в число массива битов
		cout<<a; // вывод на экран как символа 
		//Sleep(100);	
		ResetEvent(sync);	// символ расшифрован ожидание новой отправки 
	}	
	return 0;
}

int main(int argc, char** argv)
{
	for(int i=0;i<8;i++) // создание событий для передачи
	{
		char name[]="E_n"; // имя событий
		name[2]=i+'0'; // преобразование типов	int в char name= E_0 E_1 и тд....
		// создание 8 несигнальных событий
		e[i]=CreateEvent(NULL, 	// атрибут защиты
						TRUE,	// тип сброса TRUE - ручной
						FALSE, // начальное состояние 
						(LPCSTR)name // имя события
						);
		if(e[i]==NULL) //события не создались 																	
			{
				cout << "Error Event Creation = " << GetLastError() << endl; // вывод ошибок				
				cin.get();																
				return 0; // выход из программы
			}					
	}
	// событие синхронизации отправки 
	sync = CreateEvent(NULL, // атрибут защиты    //событие синхронизации
						TRUE,// тип сброса TRUE - ручной
						FALSE,// начальное состояние
						(LPCSTR)"Sync"// имя события
						);							
	if(sync==NULL) //событие не создались 																	
			{
				cout << "Error Event Creation = " << GetLastError() << endl; // вывод ошибок				
				cin.get();																
				return 0; // выход из программы
			}	
	/* поток для чтения  */			
	HANDLE h;		//дескриптор потока
	DWORD ID = NULL;	// id потока										
	h=CreateThread(NULL,// дескриптор защиты
					NULL,//размер стека
					(LPTHREAD_START_ROUTINE)Get_M	//указатель на функцию WINAPI
					,NULL//параметр дл¤ функции
					,NULL,//флаг приостановки потока
					&ID);		//идентификатор потока
					
	if(h==NULL)		// проверка создания потока																
	{
		cout << "Error Thread Creation = " << GetLastError() << endl;	// вывод ошибок				
		cin.get();																		
		return 0;
	}
	
	string str;
	cout<<("Enter message.\n");	
	cout<<"Enter the exit to close application \n";	
	while(true) // бесконечное считывание стоки на отправку
	{
		getline(cin,str); // считывание строки
		str=str+"\n"; // в конец строки символ перевода каретки
		if(str=="exit\n")  // команда выхода
			{
				return 0;
			}
		bitset<8> bits; // битовое поле размера 8
		
		for(int j=0;j<str.size();j++) // цикл по всем символам 
		{ 
			bits=str[j]; // j - символ в битовое поле
			for(int i=0;i<8;i++) // каждый бит отдельно
			{	
				if(1==bits[i]) // i- бит =1 установить i- сообщение
					SetEvent(e[i]); // если 1 установим событие 
				else
					ResetEvent(e[i]); // сборос события в 0 необходимо при повторной отправке			
			}
			SetEvent(sync); // установка в сигнальное положение события синхронизации
			Sleep(100); // задежка
		}	
	}
	return 0;
}
