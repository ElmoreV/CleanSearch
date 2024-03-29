#include"MyController.h"
#include"Search.h"
#include"MyStrings.h"
/*::PostQuitMessage, ::KillTimer and ::GetWindowText uses user32.dll*/
void TopController::OutputStatistics()
{
	_view->LScroll();
	// make a string for the time
	std::wstring w=L"Elapsed Time: ";
	w+=ItoW(_srch.GetItem(CLOCK_TIME));
	w+=L" milliseconds!";
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
	_view=std::unique_ptr<View>(new View(_h));
	_view->SetSearchMode(false);
	return 0;
};
bool TopController::OnSize(int width, int height, int flag)
{
	_view->Size(width,height);
	return 0;
}
bool TopController::OnControl (Win::Dow control, int controlId, int notifyCode)
{
	if (controlId>=5&&controlId<=8)
	{
		//Handle the checkboxes, they have id 5 to 8
		Win::CheckBox c(control);
		c.Toggle();
		return true;
	}
	else if (controlId==IDOK && _view.get())//If the _view was initialized and the ID was enter or the main button of "Search" and "Stop"
	{
		
		//If it was the stop button that was clicked
		if (_view->IsSearching())
		{
			//Stop all the timers, and show the "Continue" button
			_view->SetSearchMode(false);
			::KillTimer(_h,1);
			_srch.StopTimeTracker();
			_view->SetButtonText(L"SEARCH");
			_view->ShowNextButton(SW_SHOW);
			return true;
		}else
		{
			_view->SetSearchMode(true);
			//If it wasn't, don't show the "Continue" button
			_view->ShowNextButton(SW_HIDE);
			//Continue everything, clear the list, initialize everything 
			_srch.ContinueTimeTracker();
			_view->ClearList();
			_srch.SetView(_view.get());
			_srch.SetWindow(_h);
			_srch.IsLetterCombination(_view->GetCheck2());
			_srch.SetCaseSensitivity(_view->GetCheck1());
			_srch.IncludeWindowsDir(_view->GetCheck3());
			_srch.SetMode(_view->GetRadio());
			if (_view->GetRadio())
			{_view->SetButtonText(L"STOP!");};
			_srch.SetFileSizeInput(_view->GetFileSizeInput().c_str());
			_srch.SetBegin(_view->GetInput2().c_str());
			_srch.SetSearchInput(_view->GetInput().c_str());
			//Start searching and output the statistics
			_srch.Find(true);

			OutputStatistics();
			return true;
		}
	}
	else if (controlId==12)
	{
		//The "Next" or "continue" button was clicked, so hide that button, set the "Search" to the "Stop" button and
		//continue searching
		_view->ShowNextButton(SW_HIDE);
		if (_view->GetRadio()){_view->SetButtonText(L"STOP!");};
		_srch.ContinueTimeTracker();
		_view->ClearList();
		_srch.Find(false);
		OutputStatistics();
		_view->SetSearchMode(true);
		return true;
	}
	return false;
}
bool TopController::OnTimer (unsigned int TimerId, TIMERPROC TimerProc)	
{ 
	if (TimerId==1)
	{
		::KillTimer(_h,1);
		_srch.Find(false);	
		OutputStatistics();
	}
	return 0;
}

