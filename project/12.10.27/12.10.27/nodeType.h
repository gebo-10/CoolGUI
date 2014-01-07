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
	void (* draw )(Pnode *);		//����ָ��
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
	int scale; //���λture�� ��div�߿�
//	int clip;	//���λture�� ��div�߿� ����
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
	0���Ƿ���ʾ
	1���Ƿ�ˢ��
	2���Ƿ�ض���Ϣ
	3���Ƿ���
	4���Ƿ�����
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
	int id;				//Ĭ��Ϊ0 
	int cclass[MAXCLASS];			//Ĭ��Ϊ0 

	int position;		//position true=absloute ʱ��x ��y ����absx  //	int absx,absy; 
	int x,y;			//Ĭ��Ϊ��һ�ڵ����λ�� Ҫ���� 
	int height,width;	//Ĭ��Ϊ0
	int top,left;
	//int degree;			//Ĭ��Ϊ0 

	Pnode *p;				//Ĭ��Ϊnull

	RGBAcolor bgcolor;
	imageNode *bgImage;	//Ĭ��Ϊnull

	GUIOBJ * obj;
	//void (* action)(MSG *msg,Rnode * thisnode);
	//Action * action;
	
};



#endif