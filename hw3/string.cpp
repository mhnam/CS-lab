#include "String.h"
#include <string>
#include <cstring>
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
{
    string tmp(neyong);
    str = neyong;
	len = str.length();
}

Str::~Str()
{
	// 소멸자; 할당된 메모리 해제
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
	return strcmp(str, a.str);
}

int Str::compare(char *a)
{
	// a의 내용과 strcmp
    return strcmp(str, a);
}

void Str::operator=(char *a)
{
	// string의 값을 대입
    Str tmp(a);
    str = a;
	len = tmp.length();
}

void Str::operator=(class Str& a)
{
	// Str의 내용을 대입
	str = a.str;
	len = a.len;
}