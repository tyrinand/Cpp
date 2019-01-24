#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <bitset>
using namespace std;

HANDLE sync;	//������� �������������
HANDLE e[8];	//������� ��������

DWORD Get_M() // ������� ������ ��� ����������
{
	bitset<8> temp; // ������� ���� 8 ���
	while(true) // ���������� ���� ���� �������� ����������
	{
		WaitForSingleObject(sync,INFINITE); // �������� ������� ������������� �������� ������������� 
		for (int i=0; i<8; i++) // ����������� ������ �������
		{
			if (WaitForSingleObject(e[i],0)==WAIT_OBJECT_0) // i-��� ������� ����������� 
				temp[i]=1;// i- ��� ���������� 1
			else
				temp[i]=0;// i-��� � ����
		}
		char a=temp.to_ulong(); // �������������� � ����� ������� �����
		cout<<a; // ����� �� ����� ��� ������� 
		//Sleep(100);	
		ResetEvent(sync);	// ������ ����������� �������� ����� �������� 
	}	
	return 0;
}

int main(int argc, char** argv)
{
	for(int i=0;i<8;i++) // �������� ������� ��� ��������
	{
		char name[]="E_n"; // ��� �������
		name[2]=i+'0'; // �������������� �����	int � char name= E_0 E_1 � ��....
		// �������� 8 ������������ �������
		e[i]=CreateEvent(NULL, 	// ������� ������
						TRUE,	// ��� ������ TRUE - ������
						FALSE, // ��������� ��������� 
						(LPCSTR)name // ��� �������
						);
		if(e[i]==NULL) //������� �� ��������� 																	
			{
				cout << "Error Event Creation = " << GetLastError() << endl; // ����� ������				
				cin.get();																
				return 0; // ����� �� ���������
			}					
	}
	// ������� ������������� �������� 
	sync = CreateEvent(NULL, // ������� ������    //������� �������������
						TRUE,// ��� ������ TRUE - ������
						FALSE,// ��������� ���������
						(LPCSTR)"Sync"// ��� �������
						);							
	if(sync==NULL) //������� �� ��������� 																	
			{
				cout << "Error Event Creation = " << GetLastError() << endl; // ����� ������				
				cin.get();																
				return 0; // ����� �� ���������
			}	
	/* ����� ��� ������  */			
	HANDLE h;		//���������� ������
	DWORD ID = NULL;	// id ������										
	h=CreateThread(NULL,// ���������� ������
					NULL,//������ �����
					(LPTHREAD_START_ROUTINE)Get_M	//��������� �� ������� WINAPI
					,NULL//�������� �� �������
					,NULL,//���� ������������ ������
					&ID);		//������������� ������
					
	if(h==NULL)		// �������� �������� ������																
	{
		cout << "Error Thread Creation = " << GetLastError() << endl;	// ����� ������				
		cin.get();																		
		return 0;
	}
	
	string str;
	cout<<("Enter message.\n");	
	cout<<"Enter the exit to close application \n";	
	while(true) // ����������� ���������� ����� �� ��������
	{
		getline(cin,str); // ���������� ������
		str=str+"\n"; // � ����� ������ ������ �������� �������
		if(str=="exit\n")  // ������� ������
			{
				return 0;
			}
		bitset<8> bits; // ������� ���� ������� 8
		
		for(int j=0;j<str.size();j++) // ���� �� ���� �������� 
		{ 
			bits=str[j]; // j - ������ � ������� ����
			for(int i=0;i<8;i++) // ������ ��� ��������
			{	
				if(1==bits[i]) // i- ��� =1 ���������� i- ���������
					SetEvent(e[i]); // ���� 1 ��������� ������� 
				else
					ResetEvent(e[i]); // ������ ������� � 0 ���������� ��� ��������� ��������			
			}
			SetEvent(sync); // ��������� � ���������� ��������� ������� �������������
			Sleep(100); // �������
		}	
	}
	return 0;
}
