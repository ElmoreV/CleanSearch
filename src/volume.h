#ifndef VOLUM_H
#define VOLUM_H
#include<iostream>
#include<Windows.h>
#include<vector>
struct VolumeVec
{
public:
	wchar_t _GUID[MAX_PATH];
	wchar_t _pathName[MAX_PATH];
	unsigned long long _totalBytes;
	unsigned long long _freeSpace;
};
class Volume
{
public:
	Volume();
	std::wstring GetFullestVolume();
private:
	std::vector <VolumeVec> _volumes;
};
#endif