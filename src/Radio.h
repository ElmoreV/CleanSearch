#if !defined (RADIO_H)
#define RADIO_H
#include"Control.h"
namespace Win
{

	class RadioButton:public Win::SimpleCtrl
	{
	public:
		RadioButton (HWND winParent, int id)
			: SimpleCtrl (winParent, id)
		{}
		RadioButton (HWND win = 0) : SimpleCtrl (win) {}
		inline bool IsChecked()
		{
			if (SendMessage(BM_GETCHECK)==BST_CHECKED)
			{return true;}
			return false;
		}
		inline void SetCheck(bool v)
		{
			WPARAM w;
			w=(v?BST_CHECKED:BST_UNCHECKED);
			SendMessage(BM_SETCHECK,w);
			return;
		}
		inline void Toggle()
		{
			if (IsChecked()){SetCheck(false);}else {SetCheck(true);};
		}
	};

	class RadioButtonCreator:public Win::ControlCreator
	{
	public:
		RadioButtonCreator(Win::Dow winParent, int childId):Win::ControlCreator(L"button",winParent,childId)
		{
			_exStyle^=WS_EX_CLIENTEDGE;
			_style=WS_VISIBLE|WS_CHILD|BS_AUTORADIOBUTTON;
		};
	};

};
#endif