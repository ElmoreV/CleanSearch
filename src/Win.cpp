#include"Win.h"
#include"Proc.h"
#include"Controller.h"
/*::ShowWindow and ::UpdateWindow use user32.dll*/
bool Win::Dow::Show(int cmdShow)
{
	::ShowWindow(_h,cmdShow);//return nonzero if prev visible, else if hidden before 0
	int success=::UpdateWindow(_h);//return nonzero if succeeds, else 0
	if (success!=0)
	{return true;}
	return false;
}

void Win::Dow::SubClass (SubController * subCtrl)
{
	// get previous window procedure and controller (if any)
	ProcPtr prevProc = GetLong<ProcPtr> (GWL_WNDPROC);
	Controller * prevCtrl = GetLong<Controller *> ();
	// remember them in the new controller
	subCtrl->Init (_h,prevProc, prevCtrl);
	// attach new controller to window
	SetLong<Controller *> (subCtrl);
	// attach new window procedure to window
	SetLong<ProcPtr> (SubProcedure, GWL_WNDPROC);
}

void Win::Dow::UnSubClass ()
{
	// Get the current subclass controller
	SubController * pCtrl = GetLong<SubController *> ();
	// restore previous window procedure and controller (if any)
	SetLong<ProcPtr> (pCtrl->GetPrevProc (), GWL_WNDPROC);
	SetLong<Controller *> (pCtrl->GetPrevController ());
}

bool Win::Dow::GoFullscreen(int type)
{
	bool success=true;
	HDC windowHDC = GetDC(_h);
	DWORD fullscreenWidth  = GetDeviceCaps(windowHDC, HORZRES);
	DWORD fullscreenHeight = GetDeviceCaps(windowHDC, VERTRES);
	//Crop the border, cut the menu and make it a visible pop-up window
	SetWindowLongPtr(_h, GWL_EXSTYLE, WS_EX_APPWINDOW);
	DWORD dwStyle = ::GetWindowLong(_h, GWL_STYLE);
	DWORD dwRemove= WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX|WS_THICKFRAME|WS_OVERLAPPED;
	DWORD dwNewStyle =WS_POPUP| dwStyle & ~dwRemove;
	::SetWindowLongPtr(_h, GWL_STYLE, dwNewStyle);
	//Use the retrieved window height and width to resize the window fullscreen.
	HWND z=type>=1?HWND_TOPMOST:HWND_TOP; 
	if (SetWindowPos(_h, z, 0, 0, fullscreenWidth, fullscreenHeight ,SWP_FRAMECHANGED)==0)success=false;;
	//Maximize for extra security
	ShowWindow(_h, SW_MAXIMIZE);
	if (type==2)
	{
		//Get more info
		DWORD colourBits       = GetDeviceCaps(windowHDC, BITSPIXEL);
		DWORD refreshRate      = GetDeviceCaps(windowHDC, VREFRESH);
		DEVMODE fullscreenSettings;
		EnumDisplaySettings(NULL, 0, &fullscreenSettings);

		//Set the fullscreen display modifications
		fullscreenSettings.dmPelsWidth        = fullscreenWidth;
		fullscreenSettings.dmPelsHeight       = fullscreenHeight;
		fullscreenSettings.dmBitsPerPel       = colourBits;
		fullscreenSettings.dmDisplayFrequency = refreshRate;
		fullscreenSettings.dmFields           = DM_PELSWIDTH |DM_PELSHEIGHT |DM_BITSPERPEL |DM_DISPLAYFREQUENCY;

		//Change it.
		if (ChangeDisplaySettings(&fullscreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
			success=false;
	}
	return success;
};

bool Win::Dow::ExitFullScreen(int windowX, int windowY, int windowWidth,int windowHeight,int type)
{
	bool success=true;
	SetWindowLongPtr(_h, GWL_EXSTYLE, WS_EX_LEFT);
	SetWindowLongPtr(_h, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
	if (type==1)
	{
		if (ChangeDisplaySettings(NULL, CDS_RESET) != DISP_CHANGE_SUCCESSFUL)
			{success=false;}
	}
	if (SetWindowPos(_h, HWND_NOTOPMOST, windowX, windowY, windowWidth , windowHeight, SWP_SHOWWINDOW)==0)
		{success=false;}
	ShowWindow(_h, SW_RESTORE);
	return success;
};