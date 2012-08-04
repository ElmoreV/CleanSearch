#if !defined (MYSTRINGS_H)
#define MYSTRINGS_H
#include <iostream>

unsigned int CtoW(wchar_t* _Dest,const char* _Source);
unsigned int WtoC(char* _Dest,const wchar_t* _Source);
std::wstring ItoW(unsigned int _i);
std::wstring LtoW(unsigned long _l);
long WtoL(std::wstring _str);

#endif