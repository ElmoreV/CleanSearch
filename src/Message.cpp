#include"Message.h"
#include"Exception.h"
/*::GetMessage,::TranslateMessage and ::DispatchMessage use user32.dll*/

WPARAM Win::Message::PumpLoop()//return when WM_QUIT or error. Return wParam of message
{
	while ((_status = ::GetMessage (& _msg, 0, 0, 0))!=0)// return -1 as a error, 0 with WM_QUIT, non-zero with all others
	{
		if (_status==-1)
		{
			
			throw Win::Exception(L"Error while getting message.");
		}
		::TranslateMessage(& _msg);//returned non-zero if translated, else zero
		::DispatchMessage (& _msg);//value returned by the procedure
	}
	return _msg.wParam;
}
int Win::Message::PumpOnce()//return -1 when WM_QUIT, return 0 when no message, return 1 when message
{
	if ((_status = PeekMessage(&_msg, 0, NULL, NULL, PM_REMOVE))!=0)// return 0 when no messages available, return non-zero else
	{
		if (_status==-1)
		{	
			throw Win::Exception(L"Error while getting message.");
		}
		if (_msg.message == WM_QUIT)     //did we receive a quit message?
		{
			return -1;
		}
		::TranslateMessage(& _msg);//returned non-zero if translated, else zero
		::DispatchMessage (& _msg);//value returned by the procedure
		return 1;
	}
	return 0;//return 0 when no quit message or no message at all
}