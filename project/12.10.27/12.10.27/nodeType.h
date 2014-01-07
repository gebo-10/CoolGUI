#ifndef COOLGUI_NODE
#define COOLGUI_NODE

#include <Windows.h>
#include "GdiPlus.h"


using namespace Gdiplus;

#define TRUE	1
#define FAlSE	-1

#define MAXCLASS	5

#define RGBAcolor	Color
#define wchar		wchar_t
//void fun
struct Pnode{
	Color *fontColor;
	wchar *text;
	wchar  font[20];
	int fontSize;
	//	char * p;
	//	dirtype dir;	
	void (* draw )(Pnode *);		//函数指针
};


#define Image		Image
#define repeat_x	1
#define repeat_y	2
#define repeat_xy	3
struct imageNode
{
	Image *image;
	int opacity;
	int  repeat; //x=1 y=1=2 or xy=3
	int scale; //如果位ture则 用div高宽
//	int clip;	//如果位ture则 用div高宽 剪裁
};
struct Rnode;
/*

typedef struct actionNode
{
	int type;
	void (* action)(Rnode * thisnode);
	struct actionNode * next;
}actionNode;

*/

/*
union sign{
	0：是否显示
	1：是否刷新
	2：是否截断消息
	3：是否换行
	4：是否在用
};
*/

class GUIOBJ;
struct Rnode{
	int type;
	unsigned int flag;
	struct Rnode * father;
	struct Rnode * son;
	struct Rnode * pre;
	struct Rnode * next;

	struct Rnode * layer;
	int zindex;

	int opacity;

	int tag;
	int id;				//默认为0 
	int cclass[MAXCLASS];			//默认为0 

	int position;		//position true=absloute 时候x 和y 就是absx  //	int absx,absy; 
	int x,y;			//默认为上一节点结束位置 要计算 
	int height,width;	//默认为0
	int top,left;
	//int degree;			//默认为0 

	Pnode *p;				//默认为null

	RGBAcolor bgcolor;
	imageNode *bgImage;	//默认为null

	GUIOBJ * obj;
	//void (* action)(MSG *msg,Rnode * thisnode);
	//Action * action;
	
};



#endif