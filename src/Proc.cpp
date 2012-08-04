#include "Controller.h"
#include "Proc.h"
#include "Win.h"
#include "Params.h"

/*::DefWindowProc uses User32.lib*/

LRESULT CALLBACK Win::WndProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Win::Dow h(hwnd);
	Win::Controller* pCtrl=h.GetLong<Win::Controller *>();
	if (msg>=WM_USER)
	{
		LRESULT result;
		pCtrl->OnUserMessage(msg,wParam,lParam,result);
		return result;
	}
	switch(msg)
	{
	
	case WM_CREATE:
		pCtrl->OnCreate(reinterpret_cast<CreateData const*> (lParam));
		break;
	case WM_DESTROY:
		pCtrl->OnDestroy();
		break;
	case WM_SIZE:
		pCtrl->OnSize(LOWORD(lParam),HIWORD(lParam),wParam);
		break;
	//case WM_SIZING:
		//{
		//RECT* windowBoundaries=reinterpret_cast<RECT*>(lParam);
		//pCtrl->OnSize(windowBoundaries->right-windowBoundaries->left,windowBoundaries->bottom-windowBoundaries->top,0);
		//break;
		//}
	case WM_PAINT:
		pCtrl->OnPaint();
		break;
	case WM_NCCREATE:
		{
			CreateData const*  create=reinterpret_cast<CreateData const*> (lParam);
			pCtrl=reinterpret_cast<Win::Controller *> (create->GetCreationData());
			pCtrl->SetWindowHandle(hwnd);
			h.SetLong(pCtrl);
		}
		break;									
	case WM_COMMAND:
		if (lParam == 0)
		{
			if (pCtrl->OnCommand (LOWORD (wParam), HIWORD (wParam) == 1))
				return 0;
		}
		else
		{
			HWND hCtrl = reinterpret_cast<HWND>(lParam);
			if (pCtrl->OnControl (hCtrl, LOWORD (wParam), HIWORD (wParam)))
				return 0;
		}
		break;
	case WM_LBUTTONDBLCLK :
		pCtrl->OnLButtonDoubleClick(LOWORD(lParam),HIWORD(lParam),wParam);
		break;
	case WM_MOUSEMOVE:
		pCtrl->OnMouseMove(LOWORD(lParam),HIWORD(lParam),wParam);
		break;
	case WM_TIMER:
		pCtrl->OnTimer(wParam,(TIMERPROC)lParam);
		break;
	case WM_KEYDOWN:
		pCtrl->OnKeyDown (wParam, lParam);
		return 0;
	case WM_KEYUP:
		pCtrl->OnKeyUp(wParam,lParam);
		return 0;
	/*case WM_ERASEBKGND:
	//	return true;
		;
	case WM_CTLCOLORMSGBOX:
	//	pCtrl->OnCtlColor((HDC)wParam,(HWND)lParam);
	//			return NULL_BRUSH;
		;
	case WM_CTLCOLOREDIT:
	//	pCtrl->OnCtlColor((HDC)wParam,(HWND)lParam);
	//			return NULL_BRUSH;
		;
	case WM_CTLCOLORLISTBOX:
	//	pCtrl->OnCtlColor((HDC)wParam,(HWND)lParam);
	//			return NULL_BRUSH;
		;
	case WM_CTLCOLORBTN:
	//	pCtrl->OnCtlColor((HDC)wParam,(HWND)lParam);
	//			return NULL_BRUSH
		;
	case WM_CTLCOLORDLG:
	//	pCtrl->OnCtlColor((HDC)wParam,(HWND)lParam);
	//			return NULL_BRUSH
		;
	case WM_CTLCOLORSCROLLBAR:
	//	pCtrl->OnCtlColor((HDC)wParam,(HWND)lParam);
	//			return NULL_BRUSH
		;
	case WM_CTLCOLORSTATIC:
	//	pCtrl->OnCtlColor((HDC)wParam,(HWND)lParam);
	//	return NULL_BRUSH
		;*/
	
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			Win::Dow otherWnd= reinterpret_cast<HWND>(lParam);
			pCtrl->OnActivate(LOWORD(wParam),HIWORD(wParam)?true:false,otherWnd);
			return 0;								// Return To The Message Loop
		}
		/*
		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}*/
	

	}
	

    

  

	return ::DefWindowProc(hwnd,msg,wParam,lParam);//return result of message processing
};

LRESULT CALLBACK Win::SubProcedure (HWND win, 
                                 UINT message, 
                                 WPARAM wParam, 
                                 LPARAM lParam)
{
	Win::Dow h(win);
	Win::SubController * pCtrl = h.GetLong<Win::SubController *>();
    switch (message)
    {
	case WM_LBUTTONDBLCLK :
		pCtrl->OnLButtonDoubleClick(LOWORD(lParam),HIWORD(lParam),wParam);
		break;
	//case WM_SETFOCUS:
	//	{
			//HWND wndPrev = reinterpret_cast<HWND> (wParam);
	//		if (pCtrl->OnFocus (wndPrev))
	//			return 0;
	//	}
	//	break;
	case WM_KEYDOWN:
		if (pCtrl->OnKeyDown (wParam, lParam))
			return 0;
		break;
	
	}
	return pCtrl->CallPrevProc (message, wParam, lParam);
}
