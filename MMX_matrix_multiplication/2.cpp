// 2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <ctime>
#include <windows.h>
#include <iomanip>
using namespace std;
extern "C"  {_int16 a[8][8]={{1,2,3,4,5,7,8,0}, {2,3,4,5,5,8,7,7}, {8,3,1,5,5,3,4,8}, {5,4,2,8,5,7,8,0}, {5,4,2,8,5,7,8,0},{5,4,2,8,5,7,8,0},{8,3,1,5,5,3,4,8}, {5,4,2,8,5,7,8,0}};}
extern "C"  {_int16 b[8][8]={{4,5,7,5,8,2,4,5}, {1,5,4,8,4,5,7,2}, {8,4,2,4,5,8,4,2},{8,4,1,2,3,5,4,5}, {7,8,1,2,5,4,5,6}, {7,8,6,5,4,7,6,2},{8,9,4,1,2,1,8,4}, {9,7,8,1,4,7,5,4}};}
extern "C"  {_int16 b_t[8][8];}
extern "C"  {_int16 q[8][8]={0};}
extern "C" void MUL_M();

void show(_int16 m[8][8])
{
	for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            cout << setw(4) << m[i][j];
        }
        cout << endl;
    }
	cout<<endl;
}
void fill(_int16 (&m)[8][8])
{	
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			m[i][j] = rand()%10;
        }
    }
}
void mul_m_c(_int16 a1[8][8],_int16 a2[8][8],_int16 (&r)[8][8])
{
	for(int i=0;i<8;i++)
		for(int j=0;j<8;j++)
		{
			b_t[i][j]=a2[j][i];
		}
for(int i = 0; i < 8; i++)
    for(int j = 0; j < 8; j++)
      for(int k = 0; k < 8; k++)
      {
                r[i][j] += a1[i][k]*b_t[j][k];
      }
}
int main()
{
	srand(time(0));
	__int64 c1, c2, fr; // измерение времени
	double t1;
	
	//fill(a);
	//fill(b);

	cout<<"A"<<endl;
	show(a);
	cout<<"B"<<endl;
	show(b);

QueryPerformanceCounter((LARGE_INTEGER*)&c1);
	mul_m_c(a,b,q);
QueryPerformanceCounter((LARGE_INTEGER*)&c2);
QueryPerformanceFrequency((LARGE_INTEGER*)&fr);
t1=(c2-c1)/(float)fr;

cout<<"C"<<endl;
show(q);

cout<<"A"<<endl;
show(a);

cout<<"B"<<endl;
show(b);	

QueryPerformanceCounter((LARGE_INTEGER*)&c1);
	MUL_M();
QueryPerformanceCounter((LARGE_INTEGER*)&c2);
QueryPerformanceFrequency((LARGE_INTEGER*)&fr);	
	cout<<"C"<<endl;
	show(q);

cout<<"\ntime c++ s = "<<t1<<endl;
cout<<"\ntime asm s =  "<<(c2-c1)/(float)fr<<endl<<endl;	
system("pause");
return 0;
}