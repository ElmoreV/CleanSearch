#include"Edit.h"

void Win::Edit::SelectAll()
{
	SendMessage(EM_SETSEL,0,-1);
}

void Win::Edit::Clear()
{
	SelectAll();
	SendMessage(WM_CLEAR);
}

