// cppLearning.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string>


int _tmain(int argc, _TCHAR* argv[])
{
	using namespace std;
	//练习2.3
	/*
	unsigned u =10, u2=42;
	cout<<u2-u<<endl;
	cout<<u-u2<<endl;

	int i=10,i2=42;
	cout<<i2-i<<endl;
	cout<<i-i2<<endl;
	cout<<i-u<<endl;
	cout<<u-i<<endl;
	*/
	string sCin;
	//while(1)
	{
	getline(cin,sCin);
	cout<<sCin;
	}
	return 0;
}

