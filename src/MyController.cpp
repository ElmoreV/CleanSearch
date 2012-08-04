#include"MyController.h"
#include"Search.h"
#include"MyStrings.h"
/*::PostQuitMessage, ::KillTimer and ::GetWindowText uses user32.dll*/
void TopController::OutputStatistics()
{
		_view->LScroll();
		// make a string for the time
		std::wstring w=L"Elapsed Time: ";
		w.append(ItoW(_srch.GetItem(CLOCK_TIME)));
		w.append(L" milliseconds!");
		_view->SetStatic(w);

		//and for the numbers of files/dirs
		w=LtoW(_srch.GetItem(DIR_FOUND));
		w+=L" directories of the ";
		w+=LtoW(_srch.GetItem(DIR_COUNT));
		w+=L" directories searched found.\r\n\r";
		w+=LtoW(_srch.GetItem(FILE_FOUND));
		w+=L" files of the ";
		w+=LtoW(_srch.GetItem(FILE_COUNT));
		w+=L" files searched found.";
		_view->SetStatic2(w.c_str());
}
bool TopController::OnDestroy()
{
	::PostQuitMessage(0); // no return value
	return 0;
}
bool TopController::OnCreate(const Win::CreateData * create)
{
	_view=std::auto_ptr<View>(new View(_h));
	return 0;
};

/*bool TopController::OnPaint()
{
//	PAINTSTRUCT ps;
	Win::UpdateCanvas canvas(_h);
	SetBkColor(canvas,RGB(0,0,0));
	SetTextColor(canvas,RGB(255,255,255));
	//::SetBkMode(canvas,TRANSPARENT);
	return 0;
}*/

bool TopController::OnSize(int width, int height, int flag)
{
	_view->Size(width,height);
	return 0;
}
/*bool TopController::OnCommand(int cmdId, bool isAccel)
{
	switch (cmdId)
	{
		case IDM_PROGRAM_EXIT:
			{
			::PostQuitMessage(0);
			break;
			}
		case IDM_RESULTS_CLEAR:
			{
				_view->ClearList();
			}
	}
	return 0;
};*/
bool TopController::OnControl (Win::Dow control, int controlId, int notifyCode)
{
	if (controlId>=5&&controlId<=8)
	{
		Win::CheckBox c(control);
		c.Toggle();
		return true;
	}
	else if (controlId==IDOK && _view.get())
	{
		wchar_t buttontext[8];
		_view->GetButtonText(buttontext);
		if (wcscmp(buttontext,L"STOP!")==0)
		{
			::KillTimer(_h,1);
			_srch.StopTimeTracker();//new
			_view->SetButtonText(L"SEARCH");
			_view->ShowNextButton(SW_SHOW);
			return true;
		}else 
		{
			_view->ShowNextButton(SW_HIDE);
		}
		_srch.ContinueTimeTracker();
		_view->ClearList();
		_srch.SetView(_view.get());
		_srch.SetWindow(_h);
		_srch.IsLetterCombination(_view->GetCheck2());
		_srch.SetCaseSensitivity(_view->GetCheck1());
		_srch.IncludeWindowsDir(_view->GetCheck3());
		_srch.SetMode(_view->GetRadio());
		if (_view->GetRadio()){_view->SetButtonText(L"STOP!");};
		_srch.SetFileSizeInput(_view->GetFileSizeInput().c_str());
		_srch.SetBegin(_view->GetInput2().c_str());
		_srch.SetSearchInput(_view->GetInput().c_str());
		_srch.Find();
		OutputStatistics();
		return true;
	}
	else if (controlId==12)
	{
		_view->ShowNextButton(SW_HIDE);
		if (_view->GetRadio()){_view->SetButtonText(L"STOP!");};
		_srch.ContinueTimeTracker();
		_view->ClearList();
		_srch.Continue();

		OutputStatistics();
		return true;
	}
	return false;
}
bool TopController::OnTimer (unsigned int TimerId, TIMERPROC TimerProc)	
{ 
	if (TimerId==1)
	{
		::KillTimer(_h,1);
	   
		_srch.Continue();
		
		OutputStatistics();
	}
		return 0;
}

//bool TopController::OnLButtonDoubleClick(int x, int y, KeyState kState)
//{
//	return 0;
//};

/*bool TopController::OnCtlColor(HDC hdc,HWND control)
{
	//::SetBkMode(hdc,TRANSPARENT);
	return true;
};*/

