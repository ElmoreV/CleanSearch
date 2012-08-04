#if !defined(LISTBOX_H)
#define LISTBOX_H
#include"Control.h"
#include"Stock.h"
#include<iostream>


namespace Win
{
	class ListBox:public Win::SimpleCtrl
	{
	public:
		ListBox(HWND winParent, int id):SimpleCtrl(winParent,id),_limit(10000){}
		ListBox(HWND win=0):Win::SimpleCtrl(win),_limit(10000){};
		int AddString (std::wstring const & str)
		{
			if (GetLineCount()<_limit)
			{
				long result=SendMessage(LB_ADDSTRING, 0, reinterpret_cast<LPARAM> (str.c_str ()));
				if (result>=0)
				{
					if (str.length()>_maxWidth)
					{
						_maxWidth=str.length();
					}
				}
				return result;
				
			}
			return -3;
		}
	/*	void SetLimit(unsigned int limit)
		{
			_limit=limit;
		}*/
		long PreAllocate(unsigned int items, long bytes)
		{
			return SendMessage(LB_INITSTORAGE,items,bytes);
		}
		std::wstring GetSelection ()
		{
			long lresult=SendMessage (LB_GETCURSEL);
			if (lresult!=LB_ERR)
			{
				std::wstring str=GetText(lresult);
				return str;
			}
			else
				return L"\0";
		}

		inline void Clear()
		{
			_maxWidth=0;
			SendMessage (LB_RESETCONTENT);
		}
		inline unsigned int GetLineCount()
		{
			return SendMessage(LB_GETCOUNT);
		}

		inline int GetTextLen (int idx)
		{
			return SendMessage (LB_GETTEXTLEN, idx);
		}
		std::wstring GetText (int idx)
		{
			
			std::wstring str;
			str.resize (GetTextLen (idx));
			SendMessage (LB_GETTEXT, idx, reinterpret_cast<LPARAM> (&str[0]));
			return str;
		}

		void SetHScrollBar(int width=0)
		{
			if (!width)
			{
				Font::Stock font(GetFont());
				long charWidth,charHeight;
				font.GetTextSize(charWidth,charHeight);
				SendMessage(LB_SETHORIZONTALEXTENT,(WPARAM)(_maxWidth+3)*charWidth);
				return;
			}
			SendMessage(LB_SETHORIZONTALEXTENT,(WPARAM)width);
			return;
		}
	private:
		unsigned int _limit;
		unsigned int _maxWidth;
	};

	class ListBoxCreator:public ControlCreator
	{
	public:
		ListBoxCreator(HWND winParent, int id):ControlCreator(L"LISTBOX",winParent,id)
		{
		_style |= LBS_NOINTEGRALHEIGHT;
		}
	};

	class ListBoxController:public Win::SubController
	{
	public:
		bool OnLButtonDoubleClick(int x, int y, KeyState kState);
	};
}
#endif