#if !defined(EDIT_H)
#define EDIT_H
#include"Control.h"
#include<iostream>


namespace Win
{
	class StaticText:public SimpleCtrl
	{
	public:
		StaticText (HWND winParent, int id)
			:SimpleCtrl (winParent,id)
		{}
		StaticText (HWND win=0): SimpleCtrl(win){}

		int SetText(const wchar_t*  str)
		{
			return SendMessage(WM_SETTEXT,0,reinterpret_cast<LPARAM> (str));
		}

	};

	class Edit:public SimpleCtrl
	{
	public:
		Edit (HWND winParent, int id)
			: SimpleCtrl (winParent, id)
		{}

		Edit (HWND win = 0) : SimpleCtrl (win) {}

		/*void Append (char const * buf);

		void Select (int offStart, int offEnd)
		{
			SendMessage (EM_SETSEL, (WPARAM) offStart, (LPARAM) offEnd);
		}

		void SetReadonly (bool flag)
		{
			SendMessage (EM_SETREADONLY, (WPARAM) (flag ? TRUE : FALSE), 0);
		}

		// code is the HIWORD (wParam)
		static bool IsChanged (int code)
		{ 
			return code == EN_CHANGE;
		}
*/
		int GetLen () const
		{
			return SendMessage (WM_GETTEXTLENGTH);
		}
/*
		int GetLineCount ()
		{
			return SendMessage (EM_GETLINECOUNT);
		}*/

		inline void GetText (wchar_t * buf, int len) const
		{
			SendMessage(WM_GETTEXT, (WPARAM) len, (LPARAM) buf);
		}

		std::wstring GetText () const
		{
			std::wstring buf;
			int len= GetLen();
			if (len)
			{
				buf.resize(len+1);
				GetText(&buf[0],len+1);
			}
			return buf;
		};
/*
		void Select ()
		{
			SendMessage (EM_SETSEL, 0, -1);
		}

		void SelectLine (int lineIdx);
		void ReplaceSelection (char const * info)
		{
			SendMessage (EM_REPLACESEL, 0, reinterpret_cast<LPARAM>(info));
		}*/
		void SelectAll();
		void Clear ();
	};
	class EditCreator:public ControlCreator
	{
	public:
		EditCreator(Win::Dow winParent, int childId):ControlCreator(L"EDIT",winParent,childId)
		{}
		void MakeReadOnly(){ _style |= ES_READONLY; }
		void MultiLine(){ _style |= ES_MULTILINE | ES_WANTRETURN | ES_AUTOVSCROLL | ES_AUTOHSCROLL;}
	};
	class EditController:public Win::SubController
	{
		bool OnKeyDown (int vKey, int flags) throw ()
		{
			if (vKey == VK_RETURN)
			{
				_h.GetParent().SendMessage (WM_COMMAND, 
					MAKEWPARAM (IDOK, 0),
					reinterpret_cast<LPARAM> (static_cast<HWND> (_h)));
				return true;
			}
			return false;
		}
	};
}

#endif