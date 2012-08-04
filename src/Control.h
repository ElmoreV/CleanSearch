#if !defined(CONTROL_CREATOR_H)
#define CONTROL_CREATOR_H
/*::GetDlgItem uses User32.dll*/
#include"WinCreate.h"
namespace Win
{
	class SimpleCtrl:public Win::Dow
	{
	public:
		SimpleCtrl (HWND winParent, int id)
			: Win::Dow (::GetDlgItem (winParent, id)), _id (id)
		{}
		SimpleCtrl (HWND win = 0)
			: Win::Dow (win), _id (0)
		{}
		void Init (HWND winParent, int id)
		{
			Init (::GetDlgItem (winParent, id));
			_id = id;
		}
		void Init (HWND win)
		{
			Dow::Init (win);
			_id = 0;
		}	
		/*useless now
		static bool IsClicked (int code)
		{
			return code == BN_CLICKED;
		}*/
		int GetId () const { return _id; }
	private:

		int _id;
	};
	class ControlCreator:public ChildCreator
	{
	public:
		ControlCreator(LPCTSTR className, Win::Dow winParent, int childId):Win::ChildCreator(className,winParent, childId)
		{
			_exStyle|=WS_EX_CLIENTEDGE;
			_style= WS_CHILD|WS_VISIBLE;
		}
	};
}

#endif