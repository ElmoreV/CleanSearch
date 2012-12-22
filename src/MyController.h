#if !defined(MY_CONTROLLER_H)
#define MY_CONTROLLER_H
#include"Controller.h"
#include<memory>
#include<iostream>
#include"View.h"
#include"Search.h"



class TopController:public Win::Controller
{
public:
	void OutputStatistics();
	bool OnDestroy();
	bool OnCreate(const Win::CreateData * create);
	bool OnSize(int width, int height, int flag);
	bool OnControl (Win::Dow control, int controlId, int notifyCode);
	bool OnTimer (unsigned int TimerId, TIMERPROC TimerProc);
	std::auto_ptr<View> _view;
	Search _srch;
};


#endif