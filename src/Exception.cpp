#include "Exception.h"
/*::GetLastError, ::SetLastError, ::FormatMessage and ::LocalFree uses Kernel.dll*/

Win::Exception::Exception(const wchar_t* message, unsigned long error):_msg(message),_sysMsg(0),_error(error)
{
	if (!_error)
	{
		zError();
		zSysMsg();
	}

	::SetLastError(0);
};

Win::Exception::~Exception()
{
	if (_sysMsg){
		::LocalFree(_sysMsg);
	}
}

DWORD Win::Exception::zError()
{
	_error=::GetLastError();
	return _error;
};

DWORD Win::Exception::zSysMsg()
{
	return 
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|
		FORMAT_MESSAGE_FROM_SYSTEM|
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		_error,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(wchar_t*) &_sysMsg,
		0,
		NULL
		);
};