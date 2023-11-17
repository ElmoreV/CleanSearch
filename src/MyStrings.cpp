#include"MyStrings.h"
#include<iostream>
/*
// These functions convert const char* to wchar_t* and other way around. It seems I don't need these anymore
// They also don't work, since strlen gets undefined when unicode is enabled.
unsigned int CtoW(wchar_t* _Dest,const char* _Source)
{
	unsigned int x=strlen(_Source);
	if (wcslen(_Dest)<x)
	{
		return x-wcslen(_Dest);
	}

	unsigned int i;
	for (i=0;i<x;i++)
	{
		_Dest[i]=_Source[i];
	}
	_Dest[i]='\0';
	return 0;
}
unsigned int WtoC(char* _Dest,const wchar_t* _Source)
{
	unsigned int x=wcslen(_Source);
	if (strlen(_Dest)<x)
	{
		return x-strlen(_Dest);
	}

	unsigned int i;
	for (i=0;i<x;i++)
	{
#pragma warning(disable: 4244)
		_Dest[i]=_Source[i];
#pragma warning(default: 4244)
	}
	_Dest[i]='\0';
	return 0;
};
*/
std::wstring ItoW(unsigned int _i)
{
	if (_i==0){std::wstring w=L"0"; return w;};
	unsigned int x=_i;
	unsigned int j=0;
	while (x)
	{
		x/=10;
		j++;
	};
	std::wstring w;
	w.resize(j);
	
	for (x=_i,j-=1;x;)
	{
		w[j]=(x%10)+48;
		x/=10;
		j--;
	};
	return w;
}

std::wstring LtoW(unsigned long _l)
{
	if (_l==0){std::wstring w=L"0"; return w;};
	unsigned long x=_l;
	unsigned int j=0;
	while (x)
	{
		x/=10;
		j++;
	};
	std::wstring w;
	w.resize(j);
	
	for (x=_l,j-=1;x;)
	{
		w[j]=(x%10)+48;
		x/=10;
		j--;
	};
	return w;
}
long WtoL(std::wstring _str)
{
	long j=1;
	long output=0;
	for (int i=_str.length()-1;i>=0;i--)
	{
		output+=(_str[i]-48)*j;
		j*=10;
	};
	return output;
}
/*unsigned int insert(wchar_t* _Dest, unsigned int _Pos, wchar_t* _Source)
{
}*/