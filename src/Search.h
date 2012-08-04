#ifndef CLEANSEARCH_H
#define CLEANSEARCH_H

#include<windows.h>
#include<ctime>
#include<vector>
#include"View.h"
//#include "List.h"
/* ::FindFirstFile, ::FindNextFile and ::FindClose uses Kernel32.dll*/

#define DIR_COUNT 1
#define DIR_FOUND 2
#define FILE_COUNT 3
#define FILE_FOUND 4
#define CLOCK_TIME 5

enum Operator
{
		OPRT_AND,
		OPRT_OR,
		OPRT_LEFT,
		OPRT_RIGHT,
		OPERAND
};

class SearchNode
{
public:
	SearchNode(const wchar_t* input, Operator oprt):_oprt(oprt){wcscpy_s(_query,MAX_PATH,input);};
	wchar_t _query[MAX_PATH];
	Operator _oprt;
};

void Uncapitalize(wchar_t* caps);
bool WcsLetComb(const wchar_t* str, const wchar_t* letterCombination);

/*class FolderSizeCounter
{
	int 
}*/

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

class Search;
class SearchItem
{
public:
	friend class Search;
	enum Status
	{
		Ok,
		EndOfDirectory,
		NotOpenable,
		FindCloseError
	};
	bool Init(const wchar_t* beginPath);
	bool Append(const wchar_t* appendix);
	bool ReInitPrevious();
	bool Select(const wchar_t* path);
	bool IsReparsePoint();
	bool IsDirectory();
	bool IsDots();
	bool IsFile();
	unsigned long long int GetFileSize();
	inline wchar_t* GetPath(){return _path;};
	inline wchar_t* GetName(){return _fileAtt.cFileName;};Status _state;
private:
	SearchHandleVector _hdl;
	WIN32_FIND_DATA _fileAtt;
	wchar_t _path[MAX_PATH];
	
};


class Search
{
public:
	enum SearchState
	{
		Ok,
		ListBoxFull
	};
	Search();
	void SetCaseSensitivity(bool v){_caseSensitive=v;return;}//Search
	void IsLetterCombination(bool v){_letterCombination=v;return;}//Search
	void IncludeWindowsDir(bool v){_windows=v;return;}//Search
	void SetMode(bool v){_stable=v;return;}
	void SetBegin(const wchar_t* root);//Search
	void SetView(View* view){_view=view;_view->ListAllocate(10000,1000000);};
	void SetSearchInput(const wchar_t* input=L"");
	void SetFileSizeInput(const wchar_t* input=L"");
	void SetWindow(HWND h){_h=h;};
	bool Find();//Search
	bool Continue();
	bool CheckWord();
	bool CheckFileSize();
	bool OpenDir();
	bool NextItem();
	bool DownLevel();
	bool OutputString(const wchar_t* string);
	unsigned long GetItem(int index);
	void StopTimeTracker(){_totalTimeTracked+=(clock()-_startTimeTracked); _startTimeTracked=clock();}
	void ContinueTimeTracker(){_startTimeTracked=clock();}
private:
	void SetDefaults();
	void ConvertMacros(wchar_t* input);
	bool OptimizeCriteria(unsigned int begin, int* end);
	//bool CheckCritInput(const wchar_t* input);
	bool ProcessCriteria(unsigned int begin, int* end,wchar_t* itemName, wchar_t* itemName2);
	View* _view;
	SearchItem SI;
	clock_t _startTimeTracked;
	clock_t _totalTimeTracked;
	clock_t _cycleTimer;
	unsigned long _fileCount, _directoryCount, _filesFound, _directoriesFound;
	unsigned long long int _estFileSize;
	wchar_t _beginPath[MAX_PATH];
	wchar_t _windowsPathName[MAX_PATH];
	wchar_t _windowsUncapPathName[MAX_PATH];
	wchar_t _windowsFileName[MAX_PATH];
	std::vector<SearchNode> _criteria;
	SearchState _srchState;
	bool _caseSensitive;
	bool _letterCombination;
	bool _windows;
	bool _stable;
	bool _fsValid;
	HWND _h;
};


#endif