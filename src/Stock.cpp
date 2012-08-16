#include"Stock.h"
#include"Canvas.h"

void Win::Font::GetTextSize(long & width, long & height)
{
	Win::DisplayCanvas canvas;
	//Font::Holder font(canvas, *this);
	this->Select(canvas);
	TEXTMETRIC metric;
	::GetTextMetrics((HDC) canvas,&metric);//return is requested setting
	width=metric.tmAveCharWidth+metric.tmOverhang;
	height=metric.tmHeight+metric.tmExternalLeading;
	return;
}