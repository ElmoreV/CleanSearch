#include"volume.h"
#include<windows.h>
#include<iostream>
#include<vector>

/* ::FindFirstVolume, ::FindNextVolume, ::FindVolumeClose, ::GetVolumePathNamesForVolumeName and ::GetDiskFreeSpaceEx uses Kernel.dll*/

std::wstring MostImportantVolume()
{
	wchar_t GUID[MAX_PATH];
	wchar_t pathName[MAX_PATH];
	wchar_t volume[MAX_PATH];
	HANDLE hdl;
	ULARGE_INTEGER freeSpace;
	ULARGE_INTEGER totalBytes;
	ULONGLONG maxBytesUsed=0;
	hdl=::FindFirstVolume(GUID,MAX_PATH);
	if (hdl==INVALID_HANDLE_VALUE)
	{
		return 0;
	};
	do
	{
	if (!::GetVolumePathNamesForVolumeName(GUID,pathName,MAX_PATH,NULL))
	{
		::FindVolumeClose(hdl);
		return 0;
	};
	SetErrorMode(SEM_FAILCRITICALERRORS);
	if (::GetDiskFreeSpaceEx(pathName,NULL,&totalBytes,&freeSpace))
	{
		if ((totalBytes.QuadPart-freeSpace.QuadPart)>maxBytesUsed)
		{
			maxBytesUsed=(totalBytes.QuadPart-freeSpace.QuadPart);
			wcscpy_s(volume,MAX_PATH,pathName);
		}
	}
	SetErrorMode(0);
	}while(::FindNextVolume(hdl,GUID,MAX_PATH));
	::FindVolumeClose(hdl);
	return volume;
}
/*
class Volume
{
public:
	Volume()
	{
		_hdl=::FindFirstVolume(_vec[0],MAX_PATH);
		for (int i=1;;i++)
		{
			if (!::FindNextVolume(_hdl,_vec[i],MAX_PATH))
			{
				break;
			};
		};
	}
	~Volume(){}
	wchar_t* GetPathName(){};
private:
	HANDLE _hdl;
	std::vector <wchar_t*> _vec;


};*/