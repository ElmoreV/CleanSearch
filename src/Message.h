#if !defined(MESSAGE_H)
#define MESSAGE_H

#include<windows.h>

namespace Win
{
	class Message
	{
	public:
		WPARAM PumpLoop();
		int PumpOnce();
		inline WPARAM WParam(){return _msg.wParam;}
	private:
		MSG _msg;
		int _status;
	};
}


#endif