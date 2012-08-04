#if !defined(PROC_H)
#define PROC_H

#include<windows.h> 

namespace Win
{
	class CreateData:public CREATESTRUCT
	{
	public:
		void * GetCreationData () const { return lpCreateParams; }
		int GetHeight () const { return cy; }
		int GetWidth () const { return cx; }
		int GetX () const { return x; }
		int GetY () const { return y; }
		const LPCTSTR GetWndName () const { return lpszName; }

	};
	typedef LRESULT (CALLBACK * ProcPtr)
		(HWND win, UINT message, WPARAM wParam, LPARAM lParam);

	LRESULT CALLBACK WndProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK SubProcedure (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

}
#endif