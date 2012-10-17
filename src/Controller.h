#if !defined(CONTROLLER_H)
#define CONTROLLER_H
#include<windows.h>
#include"Proc.h"
#include"Win.h"
#include"Mouse.h"

/* ::CallWindowProc() uses user32.dll*/
namespace Win
{
	
	class Controller
	{
		friend LRESULT CALLBACK Win::WndProc (HWND win, 
			UINT message, WPARAM wParam, LPARAM lParam);
	public:
		virtual ~Controller () {}
		virtual bool OnCreate (Win::CreateData const * create) throw ()
		{ return false; }
		virtual bool OnDestroy () throw ()
		{ return false; }
		virtual bool OnActivate (int type,bool isMinimized, Win::Dow otherWnd)throw ()
		{ return false; }
		virtual bool OnFocus (Win::Dow winPrev) throw ()
		{ return false; }
		virtual bool OnSize (int width, int height, int flag) throw ()
		{ return false; }
		virtual bool OnPaint () throw ()
		{ return false; }
		virtual bool OnCtlColor(HDC hdc,HWND control)
		{return false; }

		virtual bool OnCommand (int cmdId, bool isAccel) throw ()
		{ return false; }
		virtual bool OnControl (Win::Dow control, int controlId, int notifyCode) throw ()
		{ return false; }
		virtual bool OnInitPopup (HMENU menu, int pos) throw ()
		{ return false; }
		virtual bool OnInitSystemPopup (HMENU menu, int pos) throw ()
		{ return false; }
		virtual bool OnMenuSelect (int id, int flags, HMENU menu) throw ()
		{ return false; }
		// Mouse
		virtual bool OnMouseMove (int x, int y, Win::KeyState kState) throw ()
		{ return false; }
		virtual bool OnLButtonDown (int x, int y, Win::KeyState kState) throw ()
		{ return false; }
		virtual bool OnLButtonDoubleClick(int x, int y, Win::KeyState kState) throw()
		{return false; }
		virtual bool OnLButtonUp (int x, int y, Win::KeyState kState) throw ()
		{ return false; }
		virtual bool OnCaptureChanged (Win::Dow newCaptureWin) throw ()
		{ return false; }
		// Keyboard
		virtual bool OnKeyDown (int vKey, int flags) throw ()
		{ return false; }
		virtual bool OnKeyUp (int vKey, int flags) throw ()
		{ return false; }
		virtual bool OnTimer (unsigned int TimerId, TIMERPROC TimerProc)
		{ return false; }
		virtual bool OnUserMessage (UINT msg, 
			WPARAM wParam, 
			LPARAM lParam, 
			LRESULT & result) throw ()
		{ return false; }
		void SetWindowHandle(HWND h){_h=h;}
	protected:
		Win::Dow _h;
	};


	class SubController: public Controller
	{
	public:
		SubController () : _prevProc (0), _prevController (0) {}
		void Init (HWND win, ProcPtr prevProc, Controller * prevCtrl)
		{
			SetWindowHandle (win);
			_prevProc = prevProc;
			_prevController = prevCtrl;
		}
		LRESULT CallPrevProc (UINT message, WPARAM wParam, LPARAM lParam)
		{
			return ::CallWindowProc (_prevProc, _h, message, wParam, lParam);
		}
		ProcPtr GetPrevProc ()
		{
			return _prevProc;
		}
		Controller * GetPrevController ()
		{
			return _prevController;
		}
	protected:
		ProcPtr _prevProc;
		Controller * _prevController;
	};

}
#endif