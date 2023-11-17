#include "ListBox.h"
#include "MyStrings.h"
#include "shellapi.h"

/*::ShellExecute uses Shell32.dll*/

// TODO: ShellExecute cannot handle 8.3-style filepaths when the filepath is above 259/260 (with NULL) characters

bool Win::ListBoxController::OnLButtonDoubleClick(int x, int y, KeyState kState)
{
	if (_h)
	{
		Win::ListBox list(_h);
		std::wstring str=list.GetSelection();

		if (wcscmp(str.c_str(),L"No search results")!=0 && str.length()>0)
		{
			std::wstring selectPhrase=L"/select,\""; 
			selectPhrase+=str.c_str();
			selectPhrase+=L"\""; // Have file path in quotes to handle spaces in file path
			HINSTANCE res = ::ShellExecute(list,L"open",L"explorer.exe",selectPhrase.c_str(),0,SW_SHOWNORMAL);
			//res returns >32 if successful
			if (res>(HINSTANCE)32)
			{return 0;}
		}
	};
	return 1;
}