#include"volume.h"

/* ::FindFirstVolume, ::FindNextVolume, ::FindVolumeClose, ::GetVolumePathNamesForVolumeName and ::GetDiskFreeSpaceEx uses Kernel.dll*/
Volume::Volume()
{
	VolumeVec volume;
	HANDLE hdl=::FindFirstVolume(volume._GUID,MAX_PATH);
	if (hdl==INVALID_HANDLE_VALUE)
	{
		return;
	};
	do
	{
		if (!::GetVolumePathNamesForVolumeName(volume._GUID,volume._pathName,MAX_PATH,NULL))
		{
			::FindVolumeClose(hdl);
			return;
		};
		SetErrorMode(SEM_FAILCRITICALERRORS);
		
		if (!::GetDiskFreeSpaceEx(volume._pathName,NULL,(ULARGE_INTEGER*)&volume._totalBytes,(ULARGE_INTEGER*)&volume._freeSpace))
		{
			volume._totalBytes=0;
			volume._freeSpace=0;
		};
		SetErrorMode(0);
		_volumes.push_back(volume);
	}while(::FindNextVolume(hdl,volume._GUID,MAX_PATH));
	::FindVolumeClose(hdl);
}
std::wstring Volume::GetFullestVolume()
{
	unsigned long long maxBytesUsed=0;
	wchar_t iMax;
	for (unsigned int i=0;i<_volumes.size();i++)
	{
		if ((_volumes[i]._totalBytes-_volumes[i]._freeSpace)>maxBytesUsed)
		{
			maxBytesUsed=(_volumes[i]._totalBytes-_volumes[i]._freeSpace);
			iMax=i;
		}
	}
	return _volumes[iMax]._pathName;
}
