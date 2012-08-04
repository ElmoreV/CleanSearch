#if !defined(WINCREATE_H)
#define WINCREATE_H
#include <windows.h>
#include"Controller.h"
namespace Win
{

	class Class
	{
	public:
		Class(HINSTANCE& hInst,LPCTSTR className, WNDPROC WndProc);
		void Register();
		void SetResIcons(int ResId);
		//Get
		inline LPCTSTR getName(){return _class.lpszClassName;};
		inline HINSTANCE getInstance(){return _class.hInstance;};
		//Set
	private:
		void SetDefaults();
		WNDCLASSEX _class;
		WNDPROC (* _pWndProc);
		LPCWSTR _name;
		HINSTANCE _hInst;

	};

	class Creator
	{
	public:
		Creator(Win::Class & WinClass);
		Creator(HINSTANCE hInstance, LPCTSTR className);
		void AddVScrollBar();
		void AddHScrollBar();
		void ChangeDimensions(int x, int y, int width, int height)
		{
			_x=x;
			_y=y;
			_width=width;
			_height=height;
		};
		HWND Create(Controller & Ctrl,LPCTSTR caption=0);


	protected:
		void SetDefaults();
		DWORD _exStyle;
		LPCTSTR _className;
		DWORD _style;
		int _x;
		int _y;
		int _width;
		int _height;
		HWND _wndParent;
		HMENU _hMenu;
		HINSTANCE _hInst;
		LPVOID _lpParam;
	};

	class ChildCreator:public Creator
	{
	public:
		ChildCreator(LPCTSTR className, Win::Dow win, int childId);
		HWND Create (LPCTSTR text=L"");
	};

}

#endif
