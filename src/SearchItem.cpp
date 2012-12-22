#include "SearchItem.h"

bool SearchItem::Reset()
{
	int i=_hdl.getPos();
	while (i-->=0)
	{
		_hdl.pop();
	}
	_state=Ok;
	return true;
}
bool SearchItem::NextItem()
{
	if (::FindNextFile(_hdl,&_fileAtt))
	{
		_state=Ok;
		return true;
	};
	//When FindNextFile returns 0, it is just the end of directory, mostly.
	//This also means that no new file name is retrieved.
	//So change the state to reflect that
	_state=EndOfDirectory;
	return false;
}
//Select an item defined a fullpath 'path'
bool SearchItem::Select(const wchar_t* path)
{
	_hdl.push(::FindFirstFile(path,&_fileAtt));
	if (_hdl==INVALID_HANDLE_VALUE)
	{
		_state=NotOpenable;
		return false;
	};
	return true;
}
//Get the first item of path
bool SearchItem::Init(const wchar_t* beginPath)
{
	wcscpy_s(_path,MAX_PATH,beginPath);
	wcscat_s(_path,MAX_PATH,L"*");
	_hdl.push(::FindFirstFile(_path,&_fileAtt));
	if (_hdl==INVALID_HANDLE_VALUE)
	{
		_state=NotOpenable;
		_path[wcslen(_path)-1]='\0';
		return false;
	};
	_path[wcslen(_path)-1]='\0';
	return true;
}
//Go up one level
bool SearchItem::ReInitPrevious()
{
	if (_state!=NotOpenable)
	{
		if (_hdl.pop())
		{
			_state=Ok;
			return true;
		} else 
		{
			_state=FindCloseError;
			return false;
		}
	}else
	{
		_hdl.getPos()-=1;
		return true;

	}
}
//Append a directory name to current pathname
//Go from "X:\" to "X:\appendix\"
bool SearchItem::Append(const wchar_t* appendix)
{
	wcscat_s(_path,MAX_PATH,appendix);
	wcscat_s(_path,MAX_PATH,L"\\*");
	_hdl.push(::FindFirstFile(_path,&_fileAtt));
	if (_hdl==INVALID_HANDLE_VALUE)
	{
		_state=NotOpenable;
		_path[wcslen(_path)-1]='\0';

		return false;
	};
	_state=Ok;
	_path[wcslen(_path)-1]='\0';
	return true;

}
//Filesize is QWORD=8 bytes
unsigned long long int SearchItem::GetFileSize()
{
	unsigned long long int a=(unsigned long long int)((_fileAtt.nFileSizeHigh<<16)+_fileAtt.nFileSizeLow);
	return a;
}
bool SearchItem::IsDots()
{
	if(*_fileAtt.cFileName!='.')
		return false;
	if (
		*(_fileAtt.cFileName+1)=='\0'||
		(*(_fileAtt.cFileName+1)=='.'&&*(_fileAtt.cFileName+2)=='\0')
		)
	{return true;}
	//It's just a file with a dot at the beginning
	return false;
}