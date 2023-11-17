#if !defined(WIN_H)
#define WIN_H
#include<windows.h>
/*::SendMessage, ::GetWindowLongPtr, ::SetWindowLongPtr, ::GetParent, ::MoveWindow and ::SetWindowText belongs to user32.dll*/

namespace Win
{
	
	class SubController;
	class Dow
	{
	public:
		Dow(HWND win=0):_h(win){};
		void Init(HWND win){_h=win;}
		bool Destroy(){return DestroyWindow(_h)?true:false;}
		operator HWND () {return _h;};

		template <class T>
		inline T GetLong (int which = GWLP_USERDATA)
		{
			return reinterpret_cast<T> (::GetWindowLongPtr (_h, which));
		};
		template <class T>
		inline void SetLong (T value, int which = GWLP_USERDATA)
		{
			::SetWindowLongPtr (_h, which, reinterpret_cast<LONG_PTR> (value));
		};

		HINSTANCE GetInstance()
		{
			return GetLong <HINSTANCE> (GWLP_HINSTANCE);
		};
		LRESULT SendMessage (UINT msg, WPARAM wparam = 0, LPARAM lparam = 0) const
		{
			return ::SendMessage (_h, msg, wparam, lparam);
		}
		BOOL SetWindowCaption(wchar_t* w)
		{
			return ::SetWindowText(_h,w);
		}

		void Move (int x, int y, int width, int height)
		{
			::MoveWindow (_h, x, y, width, height, TRUE);//Succeed=nonzero, fail=zero
		}
		bool Show(int cmdShow);
		void SubClass (Win::SubController * subCtrl);
		void UnSubClass();
		Win::Dow GetParent () { return ::GetParent (_h); }
		void SetFont(HFONT font)
		{
			BOOL fRedraw = TRUE;
			SendMessage (WM_SETFONT, reinterpret_cast<WPARAM> (font), MAKELPARAM (fRedraw, 0));
			return;
		}
		HFONT GetFont()
		{
			LRESULT lresult=SendMessage (WM_GETFONT);
			return reinterpret_cast<HFONT> (lresult);
		}
		//void SetTranparency(int opacity);
		bool GoFullscreen(int type=0);
		bool ExitFullScreen(int windowX, int windowY, int windowWidth,int windowHeight,int type=0);
	private:
		HWND _h;
	};


};


#endif