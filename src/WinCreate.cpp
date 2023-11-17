#include "WinCreate.h"
#include"Exception.h"
/*::LoadImage, ::LoadIcon, ::LoadCursor, ::RegisterClassEx and ::GetSystemMetrics uses user32.dll*/
/*::CreateWindowEx uses user32.dll*/


//////////////
//////////////
//Win::Class//
//////////////
//////////////
Win::Class::Class(HINSTANCE& hInst,LPCTSTR className, WNDPROC WndProc):_hInst(hInst),_name(className),_pWndProc(&WndProc)
{
	SetDefaults();
}

void Win::Class::SetResIcons (int resId)
{


    _class.hIcon = static_cast<HICON> (
        ::LoadImage (
            _class.hInstance, 
            MAKEINTRESOURCE (resId), 
            IMAGE_ICON, 
            ::GetSystemMetrics (SM_CXICON),
            ::GetSystemMetrics (SM_CYICON),
            LR_DEFAULTCOLOR));
    // Small icon can be loaded from the same resource
    _class.hIconSm = reinterpret_cast<HICON> (
        ::LoadImage (
            _class.hInstance, 
            MAKEINTRESOURCE (resId), 
            IMAGE_ICON, 
            ::GetSystemMetrics (SM_CXSMICON),
            ::GetSystemMetrics (SM_CYSMICON),
            0));
}
void Win::Class::SetDefaults()
{
	_class.cbSize				=sizeof(WNDCLASSEX);
	_class.style				=CS_DBLCLKS;//CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	_class.lpfnWndProc			=*_pWndProc;
	_class.cbClsExtra			=0;
	_class.cbWndExtra			=0;
	_class.hInstance			=_hInst;
	_class.hIcon				=::LoadIcon(0, IDI_WINLOGO);
	_class.hCursor				=::LoadCursor (0, IDC_ARROW);//
	_class.hbrBackground		=reinterpret_cast<HBRUSH> (COLOR_WINDOW + 1);//
	_class.lpszMenuName			=0;
	_class.lpszClassName		=_name;
	_class.hIconSm				=::LoadIcon(0, IDI_WINLOGO);
};

void Win::Class::Register()
{
	if (::RegisterClassEx (&_class) == 0)
	{
		throw Win::Exception(L"RegisterClass failed");
	}
	return;
}
////////////////
////////////////
//Win::Creator//
////////////////
///////////////
Win::Creator::Creator(Win::Class & winClass):_className(winClass.getName()),_hInst(winClass.getInstance())
{
	SetDefaults();
}
Win::Creator::Creator(HINSTANCE hInst, LPCTSTR className):_className(className),_hInst(hInst)
{
	SetDefaults();
};

void Win::Creator::SetDefaults()
{
	_exStyle=0;
	_style=WS_CLIPCHILDREN |WS_OVERLAPPEDWINDOW;
	_x=CW_USEDEFAULT;
	_y=CW_USEDEFAULT;
	_width=CW_USEDEFAULT;
	_height=CW_USEDEFAULT;
	_wndParent=0;
	_hMenu=0;
	_lpParam=0;
	return;
}

HWND Win::Creator::Create(Controller & Ctrl,LPCTSTR caption)
{
	HWND _h=::CreateWindowEx(_exStyle,
		_className,
		caption,
		_style,
		_x,
		_y,
		_width,
		_height,
		_wndParent,
		_hMenu,
		_hInst,
		&Ctrl);
	if (_h==0)
	{
		throw Win::Exception(L"Window Creation Failed");
	}
	return _h;
}
void Win::Creator::AddVScrollBar()
{
	_style|=WS_VSCROLL| ES_AUTOVSCROLL;
}
void Win::Creator::AddHScrollBar()
{
	_style|=WS_HSCROLL|ES_AUTOHSCROLL;
}


Win::ChildCreator::ChildCreator(LPCTSTR className, Win::Dow winParent, int childId): Win::Creator(winParent.GetInstance(),className)
{
	_style|= WS_CHILD|WS_CLIPSIBLINGS;
	_wndParent = winParent;
	_hMenu = reinterpret_cast<HMENU> (childId);
};

HWND Win::ChildCreator::Create(LPCTSTR text)
{
	HWND _h=::CreateWindowEx(_exStyle,_className,
        text,
        _style,
        0, 0,
        500, 20,
        _wndParent,
        _hMenu,
        _hInst,
        NULL);
	if (_h==0)
	{
		throw Win::Exception(L"Child Window Creation Failed");
	}
	return _h;
}