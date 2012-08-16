#if !defined (STOCKS_H)
#define STOCKS_H
#include"Canvas.h"

/*::SelectObject and ::GetStockObject uses coredll.lib
::SetTextColor uses gdi.dll*/

namespace Win
{
	class StockObject
	{
	public:
		StockObject(int type=0):_obj(::GetStockObject(type)),_objOld(0),_hdc(0)
		{}
		~StockObject()
		{DeleteObject(_obj);}
		operator HGDIOBJ () const
		{ return _obj; }
		//Select the new object and store the old one
		bool Select(HDC hdc)
		{
			HGDIOBJ tempOld=::SelectObject(hdc,_obj);
			if (tempOld!=INVALID_HANDLE_VALUE)
			{return false;}
			else
			{
				_hdc=hdc;
				_objOld=tempOld;
				return true;
			}
		};
		//Select the old object (if and only if the current one belongs to this object)
		bool Unselect()
		{
			if (!_hdc)
			{return false;}
			HGDIOBJ tempOld=::SelectObject(_hdc,_objOld);
			if (tempOld==INVALID_HANDLE_VALUE)
			{return false;}
			if (tempOld!=_obj)//The wrong one is unselected
			{
				SelectObject(_hdc,tempOld);//So select it back
				return false;
			}

		};
	protected:
		HGDIOBJ _obj;
		HDC _hdc;
		HGDIOBJ _objOld;
	};
	class Font:public StockObject
	{
	public:
		//If you get a stocktype, just initialize with it
		Font(int type=0):StockObject(type)
		{}
		//If you get a font, well, than you have to use the font
		Font(HFONT font)
		{_obj=font;}
		operator HFONT() const{return reinterpret_cast<HFONT>(_obj);}
		void SetTextColor(int r, int g, int b)
		{_oldColor=::SetTextColor(_hdc,RGB(r,g,b));};
		void UnsetTextColor()
		{
			COLORREF tempColor=::SetTextColor(_hdc,_oldColor);
			if (tempColor!=_color)
			{::SetTextColor(_hdc,tempColor);}
		}
		void GetTextSize(long & width, long & height);
	protected:
		COLORREF _color;
		COLORREF _oldColor;
	};
	class OemFixedFont:public Font
	{
	public:
		OemFixedFont():Font(OEM_FIXED_FONT)
		{}
	};
	class SystemFixedFont:public Font
	{
	public:
		SystemFixedFont():Font(SYSTEM_FIXED_FONT)
		{}
	};
	/*

	namespace Font
	{
		class Stock:public Win::StockObject
		{
		public:
			Stock(int type):Win::StockObject(type){};
			Stock(HFONT font):Win::StockObject(0){_obj=font;};
			operator HFONT() const{return reinterpret_cast<HFONT>(_obj);}
			void GetTextSize(long & width, long & height);
		};
		class OemFixed:public Stock
		{
		public:
			OemFixed():Font::Stock(OEM_FIXED_FONT){};
		};
		class SysFixed:public Stock
		{
		public:
			SysFixed():Font::Stock(SYSTEM_FIXED_FONT){};
		};
		class Holder
		{
		public:
			Holder(HDC hdc,HFONT font):_hdc(hdc),_fontOld(reinterpret_cast<HFONT> (::SelectObject (_hdc, font))){}
		private:
			HDC _hdc;
			HFONT _fontOld;
		public:
			class Color
			{
			public:
				Color(HDC hdc,int r, int g, int b):_hdc(hdc),_oldColor(::SetTextColor(_hdc,RGB(r,g,b))){};
				~Color(){::SetTextColor(_hdc,_oldColor);}
			private:
				HDC _hdc;
				COLORREF _oldColor;

			};
		};
	}
	class StockObject
	{
	public:
		StockObject (int type)
			: _obj (::GetStockObject (type)){}
		~StockObject()
		{::DeleteObject(_obj);}
		operator HGDIOBJ () const { return _obj; }
	protected:
		HGDIOBJ _obj;
	};

	class StockObjectHolder
	{
	public:
		StockObjectHolder (HDC hdc, int type)
			: _hdc (hdc)
		{
			_hObjOld = ::SelectObject (_hdc, StockObject (type));
		}

		~StockObjectHolder ()
		{
			::SelectObject (_hdc, _hObjOld);
		}
	private:
		HGDIOBJ  _hObjOld;
		HDC      _hdc;
	};
	*/
};

#endif