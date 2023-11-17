//#include"volume.h"
#include<windows.h> 
//#include<iostream>
#include"MyStrings.h"
#include"MyWinLibA.h"
#include"MyController.h"
#include"Resource.h"

wchar_t version[255]=L"v1.1";

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* cmdLine, int cmdShow)
{
	try
	{
		// Create a Window, add the EventHandler (Controller), and start waiting/handling messages.
		Win::Class winclass(hInstance,L"mainclass",Win::WndProc);
		winclass.SetResIcons(IDI_ICON1);
		winclass.Register();
		Win::Creator winMaker(winclass);
		TopController Ctrl;
		wchar_t title[255]=L"Fresh as ever, enjoy Cleansearch ";
		wcscat_s(title,255,version);
		Win::Dow win(winMaker.Create(Ctrl,title));
		win.Show(cmdShow);
		Win::Message msg;
		return msg.PumpLoop();
	}
	catch(Win::Exception e)
	{
		MessageBox(NULL,e.GetSysErrorMessage(),(wchar_t*)e.GetErrorMessage(),MB_OK | MB_ICONERROR);
	};
	return -1;
}

