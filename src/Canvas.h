#if !defined (CANVAS_H)
#define CANVAS_H
#include<windows.h>
#include <WinGDI.h>
/*::ReleaseDC and ::GetDc uses user32.dll
::CreateDC uses Gdi.dll*/
namespace Win
{
	class Canvas
	{
	public:
		inline operator HDC(){return _hdc;};
	protected:
		Canvas(HDC hdc=0):_hdc(hdc){};
		HDC _hdc;
	};

	class UpdateCanvas: public Canvas
	{
	public:
		UpdateCanvas (HWND win=0):_h(win),Canvas(GetDC(_h))
		{}
		void Init(HWND win=0)
		{
			Cleanup();
			if (win!=0)
			{
				_hdc=GetDC(win);
				_h=win;
			}
		}
		void Cleanup()
		{
			if (_hdc!=0&&_h!=0)
				if (::ReleaseDC (_h, _hdc)){_hdc=0;_h=0;}
		}
		~UpdateCanvas ()
		{
			if (_hdc!=0&&_h!=0)
				::ReleaseDC (_h, _hdc);
		}
	protected:
		HWND _h;
	};

	class DisplayCanvas: public Canvas
	{
	public:
		DisplayCanvas ()
			: Canvas (::CreateDC (L"DISPLAY", 0, 0, 0))
		{}
	};
}

#endif