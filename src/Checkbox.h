#if !defined (CHECKBOX_H)
#define CHECKBOX_H
#include"Control.h"
namespace Win
{

	class CheckBox:public Win::SimpleCtrl
	{
	public:
		CheckBox (HWND winParent, int id)
			: SimpleCtrl (winParent, id)
		{}
		CheckBox (HWND win = 0) : SimpleCtrl (win) {}
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
			if (IsChecked())
			{SetCheck(false);}
			else 
			{SetCheck(true);};
		}
	};

	class CheckBoxCreator:public Win::ControlCreator
	{
	public:
		CheckBoxCreator(Win::Dow winParent, int childId):Win::ControlCreator(L"button",winParent,childId)
		{
			_exStyle^=WS_EX_CLIENTEDGE;
			_style=WS_VISIBLE|WS_CHILD|BS_CHECKBOX;
		};
	};

};
#endif
