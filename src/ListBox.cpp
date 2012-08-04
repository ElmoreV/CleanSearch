#include "ListBox.h"
//#include "MyStrings.h"
#include "shellapi.h"
#include "C:\Users\Gebruiker\Documents\Elmore\Programmeren\Cleansearch\Search.h"

/*::ShellExecute uses Shell32.dll*/

bool Win::ListBoxController::OnLButtonDoubleClick(int x, int y, KeyState kState)
{
	if (_h)
	{
		Win::ListBox list(_h);
		std::wstring str=list.GetSelection();

		if (wcscmp(str.c_str(),L"No search results")!=0 && str.length()>0)
		{
			std::wstring selectPhrase=L"/select,";
			selectPhrase+=str.c_str();
			::ShellExecute(list,L"open",L"explorer.exe",selectPhrase.c_str(),0,SW_SHOWNORMAL);
			return 0;
		}
	};
	return 1;
}