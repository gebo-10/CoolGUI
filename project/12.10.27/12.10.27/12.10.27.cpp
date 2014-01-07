// 12.10.27.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "12.10.27.h"

#include "CoolGUI.h"

#include "buttonPlug.h"

void hello(MSG *msg,Rnode *pnode){
	//
	switch (msg->message)
	{
	case   WM_LBUTTONDOWN:
		MessageBox(NULL,L"registerFun",L"",MB_OK);
		break;
	}
}
int main()
{
	void (*tmp)(MSG *msg,Rnode *pnode);

	int a;
	CoolGUI myWin;

	myWin.registerFun("hello",hello);		//用户自定义参数

	//myWin.registerPlug("Button",button::plug);
	button::RegisterPlug(&myWin.plugMap);

	myWin.init();
	
	myWin.window->showWindow();
	myWin.window->winLoop();
	return 0;
}