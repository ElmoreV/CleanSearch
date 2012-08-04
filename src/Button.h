#if !defined (BUTTON_H)
#define BUTTON_H
#include "Control.h"
namespace Win
{
	class Button:public SimpleCtrl
	{
	public:
		Button (HWND winParent, int id)
			: SimpleCtrl (winParent, id)
		{}

		Button (HWND win = 0) : SimpleCtrl (win) {}

	};
	class ButtonCreator:public Win::ControlCreator
	{
	public:
		ButtonCreator(HWND winParent, int id):ControlCreator(L"BUTTON",winParent,id)
		{
		};
	};

}

#endif