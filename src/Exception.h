#ifndef EXCEPTION_H
#define EXCEPTION_H

#include<windows.h>

namespace Win
{
	class Exception
	{
	public:
		Exception(const wchar_t* message, unsigned long error=0);
		Exception():_error(0){zError();zSysMsg();};
		~Exception();
		unsigned long GetError(){return _error;}
		const wchar_t* GetErrorMessage(){return _msg;}
		const wchar_t* GetSysErrorMessage(){return (wchar_t*)_sysMsg;}
	protected:
		DWORD zError();
		DWORD zSysMsg();
		const wchar_t* _msg;
		void* _sysMsg;
		unsigned long _error;
	};
}

#endif