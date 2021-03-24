#include "String.h"
#include <cstdlib>
#include <iostream>
using namespace std;
Str::Str(int leng)
{
	if(leng<0)
	{
		cout<<"Size is too small!"<<endl;
	}
	else
	{
    str = new char[leng];
		len = leng;
	}
}

Str::Str(char* neyong)
{//예외처리?
	str = new char[strlen(neyong)];
	strcpy(str, neyong);
	len = strlen(neyong);
}

Str::~Str()
{
	// 소멸자; 할당된 메모리 해제
	delete(str);
  str = NULL;
}

int Str::length()
{
	// 배열의 크기 리턴
	return len;
}

char* Str::contents(void)
{
	// string의 내용을 리턴하는 함수.
	return str;
}

int Str::compare(class Str& a)
{
	// a의 내용과 strcmp
	// char tmp1[strlen(*str)];
	// char tmp2[strlen(*a)];
	return strcmp(*str, *a.str);
}

int Str::compare(char *a)
{
	// a의 내용과 strcmp.
}

void Str::operator=(char *a)
{
	// string의 값을 대입
	str = a;
	len = strlen(a);
}

void Str::operator=(class Str& a)
{
	// Str의 내용을 대입.
	str = a.str;
	len = strlen(a.str);
}




// 배열에 원소를 대입하거나 값을 반환하는 부분으로 []연산자의 오버로딩이다
int& Array::operator[](int i) // 배열에 원소 삽입
{
	static int tmp;

	// 배열의 인덱스가 범위 내에 있으면 값 리턴, 그렇지 않으면 에러메세지 출력하고 tmp리턴
	if(i >= 0 && i < len)
	{
        return data[i];
	}
	else
	{
		cout<<"Array bound error!"<<endl;
		return tmp;
	}
}

int Array::operator[](int i) const // 배열의 원소값 반환
{
	//배열의 인덱스가 범위 내에 잇으면 값을 리턴, 그렇지 않으면 에러메세지 출력하고 0을 리턴
	if(i >= 0 && i<len)
	{
		return data[i];
	}
	else
	{
		cout<<"Array bound error!"<<endl;
		return 0;
	}
}
