#ifndef SEARCHITEM_H
#define SEARCHITEM_H
#include<windows.h>

class SearchHandleVector
{
public:
	SearchHandleVector():_position(-1)
	{}
	void push(HANDLE h){
		_handle[++_position]=h;
	};
	int pop()
	{
		return ::FindClose(_handle[_position--]);//return nonzero if succeeds, else 0
	}
	int& getPos() {return _position; }
	operator HANDLE() const {return _handle[_position]; }
private:
	HANDLE _handle[128];
	int _position;
	
};


class SearchItem
{
public:
	enum Status
	{
		Ok,
		EndOfDirectory,
		NotOpenable,
		FindCloseError
	};
	bool Reset();
	bool Init(const wchar_t* beginPath);
	bool NextItem();
	bool Append(const wchar_t* appendix);
	bool ReInitPrevious();
	bool Select(const wchar_t* path);
	inline bool IsDirectory(){return (_fileAtt.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)?true:false;};
	inline bool IsReparsePoint(){return (_fileAtt.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)?true:false;};
	inline bool IsDots(){return (*_fileAtt.cFileName=='.');}
	unsigned long long int GetFileSize();
	inline wchar_t* GetPath(){return _path;};
	inline wchar_t* GetName(){return _fileAtt.cFileName;};
	inline Status GetState(){return _state;}
	
private:
	SearchHandleVector _hdl;
	WIN32_FIND_DATA _fileAtt;
	wchar_t _path[MAX_PATH];
	Status _state;
};

#endif