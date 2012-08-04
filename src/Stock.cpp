#include"Stock.h"
#include"Canvas.h"

void Win::Font::Stock::GetTextSize(long & width, long & height)
{
	Win::DisplayCanvas canvas;
	Font::Holder font(canvas, *this);
	TEXTMETRIC metric;
	::GetTextMetrics((HDC) canvas,&metric);//return is requested setting
	width=metric.tmAveCharWidth+metric.tmOverhang;
	height=metric.tmHeight+metric.tmExternalLeading;
	return;
}