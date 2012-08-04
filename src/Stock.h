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
		StockObject (int type)
			: _obj (::GetStockObject (type))
		{}
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
};

#endif