#include<windows.h>
#include"Search.h"
#include"volume.h"
#include "MyStrings.h"
#include "Exception.h"

/*MessageBox uses user32.dll*/
#include <fstream>


wchar_t* _macros[][2]=
{
	{L"music",L"<.mp3|.mid|.wma|.ogg|.aac>"},
	{L"sounds",L"<.mp3|.mid|.wma|.ogg|.aac|.wav>"},
	{L"videos",L"<.avi|.mov|.wmv|.mpg|.mpeg|.flv|.ogm|.ogv|.mkv|.vob|.asf>"},
	{L"images",L"<.bmp|.jpg|.jpeg|.jpe|.jfif|.dib|.png|.gif|.tif>"},
	{L"documents",L"<.doc|.txt|.htm|.pdf|.xml>"}
};
unsigned int _macrolenght=5;

class SearchException:public Win::Exception
{
public:
	SearchException(const wchar_t* message, unsigned long error=0):Win::Exception(message,error){_file.open("searchLog.txt");};
	SearchException():Win::Exception(){};
	~SearchException(){_file.close();}
	void ClearLog()
	{
		if (_file.is_open())
		{
			_file.close();
		}
		_file.open("searchLog.txt",std::ios::out|std::ios::trunc);
	}
	void AppendExtendedLog(const wchar_t* path)
	{
		if (_file.is_open())
		{
			std::wstring writebuf=_msg; writebuf+=L"\nError ["; writebuf+=ItoW(_error); writebuf+=L"] : "; writebuf+=(wchar_t*)_sysMsg;
			writebuf+=L"It happened at path: ";writebuf+=path; writebuf+=L"\n";
			
//			std::wstring writebuf=L"Error "; writebuf+=ItoW(_error);writebuf+=L" happened with path \""; writebuf+=path; 
//			writebuf+=L"\"\nDesription: ";writebuf+=(wchar_t*)_sysMsg;writebuf+=L"Extended information: \"";writebuf+=_msg;writebuf+=L"\"\n\0";
			_file.seekp(0,std::ios::end);
			_file.write(writebuf.c_str(),writebuf.length());
		}

	};
	void AppendBasicLog(const wchar_t* path)
	{
		if (_file.is_open())
		{
			std::wstring writebuf=_msg; writebuf+=path; writebuf+=L"\n";
//			std::wstring writebuf=L"An error happened with path \""; writebuf+=path; 
//			writebuf+=L"\"\nDesription: ";writebuf+=_msg;writebuf+=L"\"\n\0";
			_file.seekp(0,std::ios::end);
			_file.write(writebuf.c_str(),writebuf.length());
		}

	};

private:
	std::wfstream _file;
	
};
//Changes all capitals in the string to lowercase
void Uncapitalize(wchar_t* caps)//3555 executions per ms
{
	unsigned int i=0;
	while (*caps!='\0')
	{
		if (*caps>='A' && *caps<='Z')
		{
			*caps+='a'-'A';

		};
		caps++;
	};
	return;
}
//WcsLetComb:
//Return true when combination is in str, else false
//11528 Executions per sec
bool WcsLetComb(const wchar_t* str, const wchar_t* letterCombination)
{
	//Check if there is something to check. If not, then quit
	if (!*letterCombination)
	{return true;}
	const wchar_t *p;
	const wchar_t *startLetComb;
	while (*str)
	{
		if (*str==*letterCombination)
		{
			p=str;//Replace the values, because resetting them with the '-' operator takes longer
			startLetComb=letterCombination;
			startLetComb++;
			while (*++p==*startLetComb)//Check until they aren't equal anymore, or letterCombination iterates to '\0', which means a match
			{
				if(!*++startLetComb)
				{return true;}
			}
		}
		str++;//Advance the string one letter
	}
	return false;
}


Search::Search(){SetDefaults();	SearchException se;se.ClearLog();};
void Search::SetDefaults()
{
	SI.Reset();
	_startTimeTracked=clock();
	_cycleTimer=_startTimeTracked;
	_totalTimeTracked=0;
	_srchState=Ok;
	_directoryCount=0;
	_fileCount=0;
	_directoriesFound=0;
	_filesFound=0;
	//Initialize what directory is the Windows directory
	//GetWindowsDirectory return 0 on error
	if (::GetWindowsDirectoryW(_windowsPathName,MAX_PATH)==0)
	{
		wcscpy_s(_windowsPathName,L"C:\\");
		wcscpy_s(_windowsFileName,L"Windows");
	}else
	{
		wchar_t tempString[MAX_PATH];
		ZeroMemory(tempString,sizeof(tempString));
		wcscpy_s(tempString,_windowsPathName);
		for (unsigned int i=wcslen(tempString);i>0;i--)
		{
			if (tempString[i]=='\\')
			{
				wcscpy_s(_windowsFileName,&tempString[i+1]);
				tempString[i+1]='\0';
				wcscpy_s(_windowsPathName,tempString);
				break;
			}
		}
	}
	wcscpy_s(_windowsUncapPathName,_windowsPathName);
	Uncapitalize(_windowsUncapPathName);



}
bool Search::OutputString(const wchar_t* string)
{
	if (_view->AddListBoxString(string)<0)
	{
		int lbAdd=_view->AddListBoxString(string);
		if(lbAdd==-3)
		{
			SearchException se(L"The listbox is full at path: ");
			std::wstring w=SI.GetPath();
			w.append(SI.GetName());
			se.AppendBasicLog(w.c_str());
			_srchState=ListBoxFull;
			return false;
		}else if (lbAdd<0)
		{
			SearchException se(L"An error occurred with the listbox!");
			std::wstring w=SI.GetPath();
			w.append(SI.GetName());
			se.AppendExtendedLog(w.c_str());
			_srchState=ListBoxFull;
			return false;
		}
	};
	return true;
}
void Search::SearchLoop()
{
	while(true)
	{
		if (!DownLevel())
		{
			break;
		}
		while (SI.GetState()==SI.Ok && _srchState==Ok)
		{
			if (SI.IsDots())
			{
				SI.NextItem();
				if (SI.IsDots())
				{
					SI.NextItem();
				};
			}else if (SI.IsReparsePoint())
			{
				if (!_fsValid && CheckWord())
				{
					std::wstring w(SI.GetPath());
					w+=SI.GetName();
					OutputString(w.c_str());
					if(_srchState==Ok)
					{
						_directoriesFound++;
					};
				}
				if (_srchState==Ok)
				{
					_directoryCount++;
					SI.NextItem();
				}
			}else if (SI.IsDirectory())
			{
				if (!_fsValid && CheckWord())
				{
					std::wstring w(SI.GetPath());
					w+=SI.GetName();
					OutputString(w.c_str());
					if(_srchState==Ok)
					{
						_directoriesFound++;
					};
				};
				if(_srchState==Ok)
				{
					_directoryCount++;
					OpenDir();
				}
			}else
			{
				if (CheckWord() && CheckFileSize())
				{
					std::wstring w(SI.GetPath());
					w+=SI.GetName();
					OutputString(w.c_str());
					if (_srchState==Ok)
					{
					_filesFound++;
					}
				};
				if(_srchState==Ok)
				{
					SI.NextItem();
					_fileCount++;
				}
			};
		};
	};
}
bool Search::Find(bool firstLoop)
{
	if (firstLoop)
	{
		SetDefaults();
		if (!_caseSensitive)
		{
			for (unsigned int i=0;i<_criteria.size();i++)
			{
				Uncapitalize(_criteria[i]._query);
			}
		}
		if (!SI.Init(_beginPath)){
			SearchException se(L"Could not find path: ");
			se.AppendBasicLog(SI.GetPath());
		};
	}else
	{_cycleTimer=clock();}
	SearchLoop();
	return 0;
}

void Search::SetBegin(const wchar_t* begin)
{
	//copy the path to the begin directory
	wcscpy_s(_beginPath,MAX_PATH,begin);
	//change all '/' into '\'
	unsigned short i=0;
	for (;i<wcslen(_beginPath);i++)
	{
		//check if it's a '/'
		//change it into '\'
		if (_beginPath[i]==L'/')
		{_beginPath[i]=L'\\';}
	}
	//add a '\' to the end if it is not 
	if (_beginPath[wcslen(begin)-1]!=L'\\')
	{
		wcscat_s(_beginPath,MAX_PATH,L"\\");
	}
	//if the input is just '\\', pick the most important volume
	if (!wcscmp(_beginPath,L"\\"))
	{
		wcscpy_s(_beginPath,MAX_PATH,MostImportantVolume().c_str());
	}
	while(true)
	{	
		//check if it exists
		if (SI.Init(_beginPath))
		{
			//if it does, do nothing and return
			return;
		}
		//create a char-pointer, reset i to 0 and 
		wchar_t* path=_beginPath+wcslen(_beginPath);
		i=0;
		while(!(--path<_beginPath))
		{
			if (*path==L'\\')
			{
				if (i)
				{
					*(path+1)=L'\0';
					break;
				} else
				{
					*path=L'\0';
					i=true;
				}
			}
		}
		if (path<=_beginPath)
		{
			break;
		}
	}
	wcscpy_s(_beginPath,MAX_PATH,MostImportantVolume().c_str());
	return;
}
void Search::ConvertMacros(wchar_t* input)
{
	unsigned int temp=0;
	for (unsigned int i=0;i<=wcslen(input);i++)
	{
		if (input[i]==L'|' || input[i]==L'&' || input[i]==L'<' || input[i]==L'>')
		{
			temp=i+1;
		}
		if (input[i]==L'?')
		{
			wchar_t tempString[MAX_PATH]=L"";
			for (unsigned int j=0;temp<i;temp++,j++)
			{
				tempString[j]=input[temp];
			}
			tempString[++temp]=L'\0';
			Uncapitalize(tempString);
			for (unsigned int x=0;x<_macrolenght;x++)
			{
				if (wcscmp(_macros[x][0],tempString)==0)
				{
					wchar_t tempString2[MAX_PATH]=L"";
					int len1=wcslen(_macros[x][0]);
					int len2=wcslen(_macros[x][1]);
					wcscpy_s(tempString2,MAX_PATH,&input[i+1]);
					i-=len1;
					wcscpy_s(&input[i],MAX_PATH-i,_macros[x][1]);
					i+=len2;
					wcscpy_s(&input[i],MAX_PATH-i,tempString2);
					i--;
					break;
				}
			};
		};
	}
	return;
}
/*bool Search::CheckCritInput(const wchar_t* input)
{
	bool check=true;
	for (unsigned int x=0;x<_criteria.size();x++)
	{
		if (_criteria[x]._oprt==OPERAND)
		{
			if (_letterCombination)
			{
				if (WcsLetComb(input,_criteria[x]._query))
				{
					check=false;
					break;
				}else if (WcsLetComb(_criteria[x]._query,input))
				{
					wcscpy_s(_criteria[x]._query,MAX_PATH,input);
					check=false;
				}
			}
			else
			{
				if (!wcscmp(_criteria[x]._query,input))
				{
					check=false;
					break;
				};
			}
		}
	}
	return check;
}
*/
bool Search::OptimizeCriteria(unsigned int begin, int* end)
{
	bool checkAnotherTime=false;
	if (_letterCombination)
	{
		//for just one step to the right
		for (unsigned int i=begin;i<_criteria.size();i++)
		{
			if (_criteria[i]._oprt==OPERAND&&i<_criteria.size()-1)
			{
				switch (_criteria[i+1]._oprt)
				{
				case OPRT_AND:
					if (_criteria[i+2]._oprt==OPERAND)
					{
						if (WcsLetComb(_criteria[i]._query,_criteria[i+2]._query))
						{
							_criteria.erase(_criteria.begin()+i+1,_criteria.begin()+i+3);
							checkAnotherTime=true;
						}else if (WcsLetComb(_criteria[i+2]._query,_criteria[i]._query))
						{
							_criteria.erase(_criteria.begin()+i,_criteria.begin()+i+2);
							checkAnotherTime=true;
						}
					}
					break;
				case OPRT_OR:
					if (_criteria[i+2]._oprt==OPERAND)
					{
						if (WcsLetComb(_criteria[i]._query,_criteria[i+2]._query))
						{
							_criteria.erase(_criteria.begin()+i,_criteria.begin()+i+2);
							checkAnotherTime=true;
						}else if (WcsLetComb(_criteria[i+2]._query,_criteria[i]._query))
						{
							_criteria.erase(_criteria.begin()+i+1,_criteria.begin()+i+3);
							checkAnotherTime=true;
						}
					}
					break;
				case OPRT_RIGHT:
					*end=i;
					return checkAnotherTime;
				}
			}else if (_criteria[i]._oprt==OPRT_LEFT)
			{
				checkAnotherTime|=OptimizeCriteria(i+1,end);
				i=*end;
				break;
			}
		}
	}
	return checkAnotherTime;
};
void Search::SetSearchInput(const wchar_t* input)
{
	_criteria.clear();
	wchar_t _input [MAX_PATH];
	wcscpy_s(_input,MAX_PATH,input);
	if (_letterCombination)
		ConvertMacros(_input);
	unsigned int i=0;
	unsigned int i2=0;
	int id=0;
	wchar_t tempString[MAX_PATH]=L"";
	while(_input[i])
	{
		if (_input[i]=='|')
		{
			if (_criteria.size()<1 ||_criteria[_criteria.size()-1]._oprt!=OPRT_RIGHT/* && tempString)*/)
			{
				_criteria.push_back(SearchNode(tempString,OPERAND));
				i2=0;
				ZeroMemory(tempString,sizeof(tempString));
			}
			_criteria.push_back(SearchNode(L"",OPRT_OR));
		} else if (_input[i]=='&')
		{
			if (_criteria.size()<1 || _criteria[_criteria.size()-1]._oprt!=OPRT_RIGHT/* && CheckCritInput(tempString)*/)
			{
				_criteria.push_back(SearchNode(tempString,OPERAND));
				i2=0;
				ZeroMemory(tempString,sizeof(tempString));
			}
			_criteria.push_back(SearchNode(L"",OPRT_AND));
		} else if (_input[i]=='<')
		{
			_criteria.push_back(SearchNode(ItoW(id).c_str(),OPRT_LEFT));id++;
		} else if (_input[i]=='>')
		{
			if (_criteria.size()<1 ||_criteria[_criteria.size()-1]._oprt!=OPRT_RIGHT/* && CheckCritInput(tempString)*/)
			{
				_criteria.push_back(SearchNode(tempString,OPERAND));
				i2=0;
				ZeroMemory(tempString,sizeof(tempString));
			}
			id--;_criteria.push_back(SearchNode(ItoW(id).c_str(),OPRT_RIGHT));
		}else
		{
			tempString[i2]=_input[i];
			i2++;
		}
		i++;
	}
	if (_criteria.size()<1 ||_criteria[_criteria.size()-1]._oprt!=OPRT_RIGHT/* && CheckCritInput(tempString)*/)
	{
		_criteria.push_back(SearchNode(tempString,OPERAND));
	}
	int end=0;
	while(OptimizeCriteria(0,&end));
}
void Search::SetFileSizeInput(const wchar_t* input)
{
	wchar_t estFileSize[MAX_PATH]=L"";
	wchar_t valuable[MAX_PATH]=L"";
	wcscpy_s(estFileSize,MAX_PATH,input);
	unsigned long long int value=0;
	unsigned long long int  multiplier=1;
	int number=-1;
	enum SizeIdentifier {PB,PiB,TB,TiB,GB,GiB,MB,MiB,kB,kiB,B,None};
	SizeIdentifier id=None;
	_fsValid=true;
	bool Initialized=false;
	for (unsigned int i=0;i<=wcslen(estFileSize);i++)
	{
		if (estFileSize[i]>='0' && estFileSize[i]<='9')
		{
			if (number==-1){number=i;}
			else if (Initialized){_fsValid=false; return;}
		} else if (estFileSize[i]==' ')
		{
			if (number!=-1 && !Initialized)
			{
				wcscpy_s(valuable,MAX_PATH,(estFileSize+number));
				valuable[i-number]='\0';
				value=(long)WtoL(valuable);
				Initialized=true;
			}
		} else if (estFileSize[i]=='\0')
		{
			if (number!=-1)
			{
				if (!Initialized)
				{
					wcscpy_s(valuable,MAX_PATH,(estFileSize+number));
					valuable[i-number]='\0';
					value=(long)WtoL(valuable);
					Initialized=true;
				}
			}else{id=None;_fsValid=false;return;}
		}else 
		{
			if (number!=-1)
			{
				if (!Initialized)
				{
					wcscpy_s(valuable,MAX_PATH,(estFileSize+number));
					valuable[i-number]='\0';
					value=(long)WtoL(valuable);
					Initialized=true;
				}
			}else{id=None;_fsValid=false;return;}
			if (Initialized && id==None)
			{
				switch (estFileSize[i])
				{
				case 'B':
				case 'b':
					{
						id=B;
						break;
					}
				case 'k':
				case 'K':
					{
						i++;
						if (estFileSize[i]=='i'|| estFileSize[i]=='I')
						{i++;
						if (estFileSize[i]=='B'||estFileSize[i]=='b'){id=kiB;i++;}
						else {id=None; _fsValid=false;return;}
						} else if (estFileSize[i]=='B'||estFileSize[i]=='b'){id=kB;i++;}
						else {id=None; _fsValid=false;return;}
						break;
					}
				case 'M':
				case 'm':
					{
						i++;
						if (estFileSize[i]=='i'|| estFileSize[i]=='I')
						{i++;
						if (estFileSize[i]=='B'||estFileSize[i]=='b'){id=MiB;i++;}
						else {id=None; _fsValid=false;return;}
						} else if (estFileSize[i]=='B'||estFileSize[i]=='b'){id=MB;i++;}
						else {id=None; _fsValid=false;return;}
						break;
					}
				case 'G':
				case 'g':
					{
						i++;
						if (estFileSize[i]=='i'|| estFileSize[i]=='I')
						{i++;
						if (estFileSize[i]=='B'||estFileSize[i]=='b'){id=GiB;i++;}
						else {id=None; _fsValid=false;return;}
						} else if (estFileSize[i]=='B'||estFileSize[i]=='b'){id=GB;i++;}
						else {id=None; _fsValid=false;return;}
						break;
					}
				case 'T':
				case 't':
					{
						i++;
						if (estFileSize[i]=='i'|| estFileSize[i]=='I')
						{i++;
						if (estFileSize[i]=='B'||estFileSize[i]=='b'){id=TiB;i++;}
						else {id=None; _fsValid=false;return;}
						} else if (estFileSize[i]=='B'||estFileSize[i]=='b'){id=TB;i++;}
						else {id=None; _fsValid=false;return;}
						break;
					}
				case 'P':
				case 'p':
					{
						i++;
						if (estFileSize[i]=='i'|| estFileSize[i]=='I')
						{i++;
						if (estFileSize[i]=='B'||estFileSize[i]=='b'){id=PiB;i++;}
						else {id=None; _fsValid=false;return;}
						} else if (estFileSize[i]=='B'||estFileSize[i]=='b'){id=PB;i++;}
						else {id=None; _fsValid=false;return;}
						break;
					}
				default:
					{
						id=None; _fsValid=false; return;
						break;
					}
				}
			}else // if !(Initialized&&id=None) 
			{
				_fsValid=false;return;
			}
		}
	}
	switch (id)
	{
	case B: multiplier=1;break;
	case kB: multiplier = 1000;break;
	case kiB: multiplier = 1024;break;
	case MB: multiplier = 1000*1000;break;
	case MiB: multiplier = 1024*1024;break;
	case GB: multiplier = 1000*1000*1000;break;
	case GiB: multiplier = 1024*1024*1024;break;
	case TB: multiplier = (unsigned long long)1000*1000*1000*1000;break;
	case TiB: multiplier = (unsigned long long)1024*1024*1024*1024;break;
	case PB: multiplier = (unsigned long long)1000*1000*1000*1000*1000;break;
	case PiB: multiplier = (unsigned long long)1024*1024*1024*1024*1024;break;
	}
	_estFileSize = value*multiplier;
	
}
bool Search::CheckWord()	
{
	wchar_t itemName[MAX_PATH];
	wchar_t itemName2[MAX_PATH];
	int end=0;
	wcscpy_s(itemName,MAX_PATH,SI.GetName());
	wcscpy_s(itemName2,MAX_PATH,SI.GetName());

	if (!_caseSensitive)
	{
		Uncapitalize(itemName);
		Uncapitalize(itemName2);
	}
	//Remove the extension for itemName2, only needed for non 'part-of-word'-searched
	if (!_letterCombination)
	{
		for (unsigned int i=wcslen(itemName2);i>0;i--)
		{
			if (itemName2[i]=='.')
			{
				itemName2[i]='\0';
				break;
			}
		}
	}
	return ProcessCriteria(0,&end,itemName,itemName2);
};
bool Search::ProcessCriteria(unsigned int begin, int* end, wchar_t* itemName, wchar_t* itemName2)
{
	/*begin is de the amount of recursive calls
	end is the last index in the recursive call
	itemName is the fileName with extension
	itemName2 is the fileName without extension
	*/
	bool left=false;
	bool right=false;
	unsigned int critSize=_criteria.size();//Reduce unnesseccary calls
	for(unsigned int i=begin;i<critSize;i++)
	{
		SearchNode sn=_criteria[i];
		switch (sn._oprt)
		{
		case OPRT_LEFT://the start and every opening bracket '<'
			{
				//Get the result of the recursive call and process the answer
				right=ProcessCriteria(i+1,end,itemName, itemName2);
				/*1. Check if it's not the first index, so the other checks can be performed
				2. Check for OR-operator
				3. Check for AND-operator
				4. Otherwise, just add the value to it
				*/
				if (!i)
				{left=right;}
				else 
				{
					SearchNode sn2=_criteria[i-1];
					if (sn2._oprt==OPRT_OR)
					{left|=right;} 
					else if (sn2._oprt==OPRT_AND)
					{left&=right;}else {left=right;}
				}
				//continue at the right index
				i=*end;
				break;
			}
		case OPRT_RIGHT://the end or a closing bracket '>'
			{
				*end=i;
				return left;
			}
		case OPERAND:
			{
				//Initialize right
				//For the lettercombination it is 'WcsLetComb'
				if (_letterCombination){right=WcsLetComb(itemName,sn._query);}
				//For exact matches, we check with and without the extension
				else if (wcscmp(itemName,sn._query)&&wcscmp(itemName2,sn._query)){right=false;}else{right=true;}
				//Initialize left with right
				if (!i)
				{left=right;}
				else 
				{
					SearchNode sn2=_criteria[i-1];
					if (sn2._oprt==OPRT_OR)
					{left|=right;} 
					else if (sn2._oprt==OPRT_AND)
					{left&=right;}else {left=right;}
				}
				break;
			}
		}
	}
	return left;
}
bool Search::CheckFileSize()	
{
	if (!_fsValid)
	{
		return true;
	}else
	{
		unsigned long long int a=SI.GetFileSize(), b=_estFileSize;
		if (a<=0&&b<=0){return true;}
		unsigned int  promillage= a>b?(unsigned int)((b*1000)/a):(unsigned int)((a*1000)/b);
		if (promillage>=800) 
		{
			return true;
		}else{ 
			return false;
		}
	}
}
bool Search::DownLevel()	
{

	//if the path is equal to the path it started, stop
	if (wcscmp(SI.GetPath(),_beginPath)==0 && SI.GetState()==SI.EndOfDirectory)
	{
		_view->SetButtonText(L"SEARCH");
		SI.ReInitPrevious();
		if ((_filesFound+_directoriesFound)<1)
		{
			OutputString(L"No search results");
		}
		StopTimeTracker();
		_srchState=Ok;
		return false;
	}else if (_srchState==ListBoxFull)
	{	
		_view->SetButtonText(L"SEARCH");
		_view->ShowNextButton(SW_SHOW);
		StopTimeTracker();
		_srchState=Ok;
		return false;

	}else if (SI.GetState()!=SI.Ok)
	{
		// make from 'a\b\c\'-> 'a\b\c' -> 'a\b\'
		wchar_t* path=SI.GetPath();
		bool j=0;
		path+=wcslen(path);
		while(--path>=SI.GetPath())
		{
			if (*path==L'\\')
			{
				if (j)
				{
					*(path+1)=L'\0';
					break;
				} else
				{
					*path=L'\0';
					j=true;
				}
			}
		}

		SI.ReInitPrevious();
		if (SI.GetState()==SI.FindCloseError){
			SearchException se(L"Error while closing a handle!");
			std::wstring w(SI.GetPath());
			w+=SI.GetName();
			se.AppendExtendedLog(w.c_str());
		};
		SI.NextItem();
	}
	if ((clock()-_cycleTimer)>=33 && _stable)
	{
		SetTimer(_h,1,1,0);//Only mention of _h, ever, can be circumvented using View
		return false;
	}
	return true;
};
bool Search::OpenDir()	
{
	if (SI.IsDirectory())
	{		
		if (!(_windows) && //Check if we want to skip the Windows folder
			(wcscmp(SI.GetName(),_windowsFileName/*L"Windows"*/)==0 &&
			((wcscmp(SI.GetPath(),_windowsUncapPathName/*L"c:\\"*/)==0)||(wcscmp(SI.GetPath(),_windowsPathName/*L"C:\\"*/)==0))) 
			)
		{
			SI.NextItem();
			return true;
		}
		else
		{
			//Open directory
			if (SI.Append(SI.GetName()))
			{
				return true;
			}else 
			{
				SearchException Test(L"Could not open directory: ");
				Test.AppendExtendedLog(SI.GetPath());
			}
		}
	}
	return false;
};
unsigned long Search::GetItem(const int index)
{
	switch(index)
	{
case DIR_COUNT:
		{return _directoryCount;}
case DIR_FOUND:
		{return _directoriesFound;}
case FILE_COUNT:
		{return _fileCount;}
case FILE_FOUND:
		{return _filesFound;}
case CLOCK_TIME:
		{return _totalTimeTracked+(clock()-_startTimeTracked);}
	}
	return 0;
};

