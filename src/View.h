#if !defined (VIEW_H)
#define VIEW_H
#include"MyWinLibB.h"
#include"Stock.h"
#include"volume.h"

class View
{
public:
	View(Win::Dow winParent):_radioH(20),_borderSpacing(20),_butW(70),_butH(30),_controlSpacing(5),_editH(20),_checkH(25),_checkW(300)
	{
		Win::Font::Stock defaultF(DEFAULT_GUI_FONT);
		Win::Font::OemFixed fixed;
		//Main query line
		Win::EditCreator edit(winParent,2);
		_edit.Init(edit.Create(L"Insert query here (e.g. \"music?\" or \".mid|.mp3\") "));
		//Root directory query
		Win::EditCreator edit2(winParent,3);
		std::wstring editText=L"Directory or drive to search in(default is \"";
		editText+=MostImportantVolume().c_str();
		editText+=L"\")";
		_edit2.Init(edit2.Create(editText.c_str()));
		//Filesize query
		Win::EditCreator fileSizeEdit(winParent,13);
		_fileSizeEdit.Init(fileSizeEdit.Create(L"Approximate file size e.g. \"30 GB\" or \"2 MiB\""));
		//Elapsed time
		Win::EditCreator static1(winParent,4);
		static1.MakeReadOnly();
		_static.Init(static1.Create(L"Elapsed Time:"));
		//"Amount of folders and files found"
		Win::EditCreator static2(winParent,5);
		static2.MakeReadOnly();
		static2.MultiLine();
		_static2.Init(static2.Create(L"No items searched"));
		//Case-sensitivity checkbox
		Win::CheckBoxCreator check1(winParent,6);
		_check1.Init(check1.Create(L"Case Sensitive"));
		_check1.SetFont(defaultF);
		//Part of word checkbox
		Win::CheckBoxCreator check2(winParent,7);
		_check2.Init(check2.Create(L"Part of a whole word"));
		_check2.SetCheck(1);
		_check2.SetFont(defaultF);
		//Include Windows directory - checkbox
		Win::CheckBoxCreator check3(winParent,8);
		_check3.Init(check2.Create(L"Include the Windows directory into the search (slower)"));
		_check3.SetFont(defaultF);
		//Radio buttons of 'Stable' and 'Fast'
		Win::RadioButtonCreator radio1(winParent,9);
		_radio1.Init(radio1.Create(L"Stable"));
		_radio1.SetFont(defaultF);
		_radio1.SetCheck(true);
		Win::RadioButtonCreator radio2(winParent,10);
		_radio2.Init(radio2.Create(L"Fast"));
		_radio2.SetFont(defaultF);
		//The list of items
		Win::ListBoxCreator list(winParent,11);
		list.AddVScrollBar();
		list.AddHScrollBar();
		_list.Init(list.Create());
		_list.SetFont(fixed);
		//Font::Stock listF(_list.GetFont());
		//int charHeigth;
		//listF.GetTextSize(_charWidth,charHeigth);
		
		//Search/Stop button
		Win::ButtonCreator button(winParent,1);
		_button.Init(button.Create(L"SEARCH!"));
		//Continue button
		Win::ButtonCreator nextButton(winParent,12);
		_nextButton.Init(nextButton.Create(L"Continue"));
		_nextButton.Show(SW_HIDE);
		_list.SubClass(&_listCtrl);
		_edit.SubClass(&_editCtrl);
		_edit2.SubClass(&_editCtrl2);
		_fileSizeEdit.SubClass(&_editCtrl3);


	}
	/*void Paint()
	{
	}*/
	long ListAllocate(unsigned int items,long bytes)
	{
		return _list.PreAllocate(items,bytes);
	}
	void ClearList()
	{
		_list.Clear();
	}
	int GetListCount()
	{
		return _list.GetLineCount();
	}
	void Size(int width, int height)
	{
		int editY=_borderSpacing;
		int edit2Y=editY+_editH+_controlSpacing;
		int maxWidth=width-(2*_borderSpacing);
		int checkY=edit2Y+_editH+_controlSpacing;
		int check2Y=checkY+_checkH;
		int check3Y=check2Y+_checkH;
		int radioY=checkY+_controlSpacing;
		int radio2Y=radioY+_radioH;
		int listY=check3Y+_checkH+_controlSpacing;
		int butY=height-_borderSpacing-_butH;
		int butX=(width/2)-(_butW/2);
		int listH=height-listY-(_butH+_borderSpacing)-_controlSpacing;
		int staticX=_borderSpacing+_checkW+10;
		int static2Y=height-_borderSpacing-_editH*2;
		int static2W=butX-_borderSpacing-_controlSpacing;
		int nextButtonX=butX+_butW+_controlSpacing;
		int fileSizeX=staticX+_checkW+_borderSpacing;
		_edit.Move(_borderSpacing,editY,maxWidth,_editH);
		_edit2.Move(_borderSpacing, edit2Y,maxWidth,_editH);
		_fileSizeEdit.Move(fileSizeX,radioY, _checkW,_editH);
		_check1.Move(_borderSpacing, checkY, _checkW, _checkH);
		_check2.Move(_borderSpacing, check2Y, _checkW, _checkH);
		_check3.Move(_borderSpacing, check3Y, _checkW, _checkH);
		_radio1.Move(staticX,radioY,_checkW,_radioH);
		_radio2.Move(staticX,radio2Y,_checkW,_radioH);
		_list.Move(_borderSpacing,listY,maxWidth,listH);
		_button.Move(butX,butY,_butW,_butH);
		_nextButton.Move(nextButtonX,butY,_butW,_butH);
		_static.Move(staticX,check3Y,_checkW,_editH);
		_static2.Move(_borderSpacing,static2Y,static2W,_editH*2);
	}
	void LScroll()
	{
		_list.SetHScrollBar();
		return;
	};
	void SetButtonText(wchar_t* w)
	{
		_button.SetWindowCaption(w);
	}
	void GetButtonText(wchar_t* w)
	{
		::GetWindowText(_button,w,wcslen(w));
	}
	std::wstring GetInput()
	{
		std::wstring w=_edit.GetText();
		return w;
	};
	std::wstring GetInput2()
	{
		std::wstring w=_edit2.GetText();
		return w;
	};
	std::wstring GetFileSizeInput()
	{
		std::wstring w=_fileSizeEdit.GetText();
		return w;
	}
	bool GetCheck1()
	{
		return _check1.IsChecked();
	};
	bool GetCheck2()
	{
		return _check2.IsChecked();
	};
	bool GetCheck3()
	{
		return _check3.IsChecked();
	};
	bool GetRadio()
	{
		return _radio1.IsChecked();
	}
	int AddListBoxString(const wchar_t* wchar)
	{
		std::wstring w=wchar;
		return _list.AddString(w);
	};
	void SetStatic (const std::wstring w)
	{
		_static.SetText(w.c_str());
		return;
	}
	void SetStatic2 (const wchar_t* w)
	{
		_static2.SetText(w);
		return;
	}
	void ShowNextButton(int cmd)
	{
		_nextButton.Show(cmd);
	}

private:

	int _borderSpacing;
	int _controlSpacing;
	int _butW;
	int _butH;
	int _editH;
	int _checkH;
	int _checkW;
	int _radioH;
	Win::Edit _edit;
	Win::Edit _edit2;
	Win::Edit _fileSizeEdit;
	Win::StaticText _static;
	Win::StaticText _static2;
	Win::ListBox _list;
	Win::CheckBox _check1;
	Win::CheckBox _check2;
	Win::CheckBox _check3;
	Win::Button _button;
	Win::Button _nextButton;
	Win::RadioButton _radio1;
	Win::RadioButton _radio2;
	Win::EditController _editCtrl;
	Win::EditController _editCtrl2;
	Win::EditController _editCtrl3;
	Win::ListBoxController _listCtrl;

	};
#endif