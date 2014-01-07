#ifndef COOLGUI
#define COOLGUI

#include "Global.h"

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"main\"" ) 

////////////////////////////////////////////////////////////////////////////////////////////////////
dictItem items[]={
	{"div",DIV},{"xml",XML},{"p",P},
	{"id",ID},{"class",CLASS},{"img",IMG},
	{"layer",LAYER},

	{"x",X},{"y",Y},{"width",WIDTH},{"height",HEIGHT},
	{"top",TOP},{"left",LEFT},

	{"image",IMAGE},{"opacity",OPACITY},{"repeat",REPEAT},{"scale",SCALE},{"clip",CLIP},{"src",SRC},

	{"size",FONTSIZE},{"color",FONTCOLOR},{"font",FONT},

	{"enter",ENTER},{"action",ACTION}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////

class RDOM;
class MsgMgr;
class Render;
class Window;
class CoolGUI;

class RDOM{
public:
	Rnode * Rtree;
	Dict guiDict;

	CoolGUI * instance;

	map<string,Plug *(* )(void)> * plugMap;
	map<string,void (*)(MSG *msg,Rnode *pnode)> *funMap;
	////////////////////////////////////////////////////////////////////////////////////////////////////
	RDOM(map<string,Plug *(* )(void)> * plugmap,map<string,void (*)(MSG *msg,Rnode *pnode)> *funMap,CoolGUI * instance)
	:guiDict(items,sizeof(items)/sizeof(dictItem)){

		this->instance=instance;

		this->plugMap=plugmap;
		this->funMap=funMap;
		file<> fdoc("index.xml");
		std::cout<<fdoc.data()<<std::endl; 
		xml_document<>  doc;    
		doc.parse<0>(fdoc.data()); 

		std::cout<<doc.name()<<std::endl;

		cssParser Parser(FILE,"test.css");
		cssToDom(doc,Parser);

		Rtree=new Rnode;

		memset(Rtree,0,sizeof(Rnode));
		Rtree=buildDoc(doc.first_node(),Rtree);

		adjust(Rtree->son);
	};
	/////////////////////////////////////////////////////////////////////////////////////
	Rnode* getTopLayer(Rnode* pnode){
		if(pnode->layer==NULL) return pnode;
		while(pnode->layer){
			if(pnode->layer->zindex >= pnode->zindex)
				pnode=pnode->layer;
		}
		return pnode;
	}
	/////////////////////////////////////////////////////////////////////////////////////
	int findNodeByAttr(vector<xml_node<> *> &Vnodes,xml_node<>* node,char *attr,char * values){
		rapidxml::xml_attribute<char> * xmlpattr=node->first_attribute(attr);
		if(xmlpattr!=NULL)
			if(strcmp(xmlpattr->value(),values)==0)
				Vnodes.push_back(node);
		//else printf("%d",pattr);
		if(node->next_sibling()!=NULL)
			findNodeByAttr(Vnodes,node->next_sibling(),attr,values);
		if(node->first_node()!=NULL)
			findNodeByAttr(Vnodes,node->first_node(),attr,values);

		return 1;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////
	int cssToDom(xml_document<>  &doc,cssParser &Parser){
		xml_node<>* root = doc.first_node();
		xml_node<>* tmp=NULL;

		vector<xml_node<> *> Vnodes;
		cssItem *onecss=Parser.cssSet;
		cssAttr *oneattr;
		while(onecss){
			if(*(onecss->cssSelect)=='#')
				findNodeByAttr(Vnodes,root,"id",onecss->cssSelect+1);
			else if(*(onecss->cssSelect)=='.')
				findNodeByAttr(Vnodes,root,"class",onecss->cssSelect+1);
			while(!Vnodes.empty()){
				tmp=Vnodes.back();
				Vnodes.pop_back();
				oneattr=onecss->attr;
				while(oneattr){
					tmp->append_attribute(doc.allocate_attribute(oneattr->name,oneattr->value));
					oneattr=oneattr->next;
				}
			}

			onecss=onecss->next;
			Vnodes.clear();
		}
		return 1;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Rnode * lastSon(Rnode *pnode){
		Rnode * tmp=pnode->son;
		if(tmp==NULL) return NULL;
		while(1){
			if(tmp->next==NULL) return tmp;
			else tmp=tmp->next;
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////
	int ifEnter(Rnode * pnode){
		if((pnode->flag&FENTER)>0)
			return YES;
		else return NO;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////
	int setFlag(Rnode * pnode,int flag,int value){
		if(value==1){
			pnode->flag=pnode->flag | flag;
		}else if(value==0){
			pnode->flag=pnode->flag & (~flag);
		}
		return OK;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////
	imageNode *buildImgNode(xml_node<> *docNode){
		if(docNode==NULL)return NULL;
		imageNode * tmp=new imageNode;
		memset(tmp,0,sizeof(imageNode));
		wchar_t pwstr[100]={L'\0'};
		//	tmp->x=200;
		//	tmp->y=200;
		rapidxml::xml_attribute<char> * attr = docNode->first_attribute();
		int attrType=0;
		while(attr){
			attrType=guiDict.findi(attr->name());
			switch(attrType){
			case SRC:

				//DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, attr->value(), -1, NULL, 0); //char to wchar_t
				MultiByteToWideChar (CP_ACP, 0,attr->value(), -1, pwstr, MultiByteToWideChar (CP_ACP, 0, attr->value(), -1, NULL, 0));

				tmp->image=new Image(pwstr);break;
			case OPACITY:tmp->opacity=atoi(attr->value());break;
			case REPEAT:
				if(strcmp(attr->value(),"xy")==0)
					tmp->repeat=repeat_xy;
				else if(*attr->value()=='x')
					tmp->repeat=repeat_x;
				else if(*attr->value()=='y')
					tmp->repeat=repeat_y;
				else tmp->repeat=0;
				break;
			case SCALE:
				if(strcmp(attr->value(),"true")==0)
					tmp->scale=TRUE;
				break;
			}
			attr=attr->next_attribute();
		}
		return tmp;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Pnode *buildPNode(xml_node<> *docNode){
		if(docNode==NULL)return NULL;
		Pnode * tmp=new Pnode;
		memset(tmp,0,sizeof(Pnode));
		//	tmp->x=200;
		//	tmp->y=200;

		DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, docNode->value(), -1, NULL, 0); //char to wchar_t
		tmp->text=new wchar[dwNum+4];
		MultiByteToWideChar (CP_ACP, 0,docNode->value(), -1, tmp->text, dwNum);

		rapidxml::xml_attribute<char> * attr = docNode->first_attribute();
		int attrType=0;
		while(attr){
			attrType=guiDict.findi(attr->name());
			switch(attrType){
			case FONTSIZE:tmp->fontSize=atoi(attr->value());break;
			case FONT:
				MultiByteToWideChar(CP_ACP,0, attr->value(),-1,tmp->font,20); 

				break;
			case FONTCOLOR:
				char color[10],*pcolor=color;
				strcpy(color,attr->value());

				while(*pcolor++!='#');
				color[8]=*pcolor++;
				color[9]=*pcolor++;
				char R=strtol(&color[8],NULL,16);

				color[8]=*pcolor++;
				color[9]=*pcolor++;
				char G=strtol(&color[8],NULL,16);

				char B=strtol(pcolor,NULL,16);

				tmp->fontColor=new Color(R,G,B);
				break;
			}
			attr=attr->next_attribute();
		}
		return tmp;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Rnode *buildDivNode(xml_node<> *docNode){
		if(docNode==NULL)return NULL;
		Rnode * tmp=new Rnode;
		memset(tmp,0,sizeof(Rnode));
		rapidxml::xml_attribute<char> * attr = docNode->first_attribute();
		int attrType=0;
		while(attr){
			attrType=guiDict.findi(attr->name());
			switch(attrType){
			case ID:tmp->id=guiDict.Dinsert(attr->value());break;
			case CLASS:									//在字典中有class 和id的映射
				for(int i=0;i<MAXCLASS;i++)
					if(tmp->cclass[i]!=0)
						tmp->cclass[i]=guiDict.Dinsert(attr->value());
				break;
			case ENTER:
				if(strcmp(attr->value(),"true")==0)
					setFlag(tmp,FENTER,1);
				break;
			case HEIGHT:tmp->height=atoi(attr->value());break;
			case WIDTH:tmp->width=atoi(attr->value());break;
			case X:
				tmp->x=atoi(attr->value()); 
				//cout<<'\n'<<attr->value()<<endl; 
				break;
			case Y:tmp->y=atoi(attr->value());break;

			case TOP:tmp->top=atoi(attr->value());break;
			case LEFT:tmp->left=atoi(attr->value());break;
			case ACTION:
				map < string, void (* )(MSG *,Rnode *) >::iterator iter;
				iter=funMap->find(attr->value());
				tmp->obj=new GUIOBJ;
				tmp->obj->pnode=tmp;
				tmp->obj->action=iter->second;
				break;
			}
			attr=attr->next_attribute();
		}
		return tmp;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////
	Rnode *buildExtNode(xml_node<> *docNode,Rnode *father){
		Plug *(* tmp)();
		map < string, Plug *(* )(void) >::iterator iter;
		iter=plugMap->find(docNode->name());
		tmp=iter->second;
		Plug * plug=tmp();
		return plug->build(docNode,father,this,this->instance);
		
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	Rnode * buildDoc(xml_node<> *docNode,Rnode *father){
		if (docNode==NULL)return NULL;
		int tagType=guiDict.findi(docNode->name());
		Rnode *tmp=NULL,*tmp2=NULL;

		switch (tagType){
		case XML:
			tmp=buildDivNode(docNode);
			if((tmp2=buildDoc(docNode->first_node(),father))!=NULL){
				tmp2->father=tmp;				//问题
			}
			tmp->son=tmp2;
			tmp->tag=XML;
			//tmp2=buildDoc(docNode->next_sibling());
			return tmp;
		case DIV:
			tmp=buildDivNode(docNode);
			tmp2=buildDoc(docNode->first_node(),tmp);
			if(tmp!=NULL){
				if(father->son==NULL)
					father->son=tmp;
				tmp->son=tmp2;
				tmp->father=father;
				tmp->tag=DIV;
			}
// 			if(tmp2!=NULL){
// 				tmp2->father=tmp;
// 			}


			tmp2=buildDoc(docNode->next_sibling(),father);
			if(tmp!=NULL){
				tmp->next=tmp2;
			}
			if(tmp2!=NULL){
				tmp2->pre=tmp;
				//tmp2->father=father;//兄弟共父亲
			}

			return tmp;

		case IMG:
			father->bgImage=buildImgNode(docNode);
			buildDoc(docNode->next_sibling(),father);
			break;
		case P:
			father->p=buildPNode(docNode);
			buildDoc(docNode->next_sibling(),father);
			break;
		case LAYER:						//layer 链
			tmp=buildDivNode(docNode);

			tmp->father=father;

			tmp2=father;
			while(tmp2->layer)
				tmp2=tmp2->layer;
			tmp2->layer=tmp;

			tmp2=buildDoc(docNode->first_node(),tmp);
			if(tmp!=NULL){
				tmp->son=tmp2;
				tmp->father=father;
				tmp->tag=LAYER;
			}
			if(tmp2!=NULL){
				tmp2->father=tmp;
			}

			tmp2=buildDoc(docNode->next_sibling(),father);
			break;

		default:
			tmp=buildExtNode(docNode,father);
			tmp2=buildDoc(docNode->next_sibling(),father);
			if(tmp!=NULL){
				tmp->next=tmp2;
				tmp->father=father;//兄弟共父亲
			}
			if(tmp2!=NULL){
				tmp2->pre=tmp;
				//tmp2->father=father;//兄弟共父亲
			}
			return tmp;

			break;
		}
		return NULL;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////
	int adjust(Rnode * pnode){		//不能把xml的xy height 和 width 计算进来 意义不一样 
		if(pnode==NULL)return 0;
		int x=0,y=0;
		int maxHeight=pnode->top+pnode->height;		//第一个子的高 初始化
		if(pnode->father!=NULL){
			if(pnode->father->tag!=XML){			//忽略xml的参数
				x=pnode->father->x;
				y=pnode->father->y;
			}
		}

		while(pnode){				//循环兄弟节点
			pnode->x=x+pnode->left;
			pnode->y=y+pnode->top;
			if(ifEnter(pnode)==YES){		//换行
				x=pnode->father->x;
				y=y+maxHeight;
			}else{
				x=x+pnode->width+pnode->left;
				if(pnode->height>maxHeight)
					maxHeight=pnode->height;
			}
			adjust( pnode->son);	//调整子
			adjust( pnode->layer);	//调整层
			pnode=pnode->next;
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////
	Rnode * catchNode(int x,int y,Rnode *root)
	{
		static Rnode* pnode;
		if(root==NULL) return NULL;
		pnode=root;

		while(1)
		{
			if((pnode->x<x)&&(pnode->y<y)&&((pnode->x+pnode->width)>x)&&((pnode->y+pnode->height)>y))
			{	
				if (pnode->layer!=NULL){			//如果有层的话 就让最高层得到焦点
					pnode= getTopLayer(pnode);
				}

				if(pnode->son!=NULL)
					pnode=pnode->son;
				else
					break;
			}
			else
			{
				if(pnode->next!=NULL){
					pnode=pnode->next;
				}else{
					pnode=pnode->father;
					break;
				}

			}
		}
		return pnode;
	}
};
class Render{
public:	
	Graphics RenderGraph;
	Render(HDC hdc):RenderGraph(hdc){
		;
	};

	void drawText(Rnode * pnode){
		REAL w = 280.0f, h = 100.0f, d = 10.0f, ds = 40;
		StringFormat stringFormat;  
		stringFormat.SetAlignment(StringAlignmentCenter);
		SizeF size((REAL)pnode->width, (REAL)pnode->height);  
		RectF rect(PointF((REAL)pnode->x, (REAL)pnode->y), size);
		//Gdiplus::Font thisfont(pnode->p->font, pnode->p->fontSize);
		//if(pnode->p->fontSize==0) 
		Gdiplus::Font thisfont(L"宋体", 20);
		//	SolidBrush textBrush(*pnode->p->fontColor);
		SolidBrush textBrush(Color(0,10,100));
		if(pnode->p->text!=NULL)
			RenderGraph.DrawString(pnode->p->text,  wcslen(pnode->p->text),  &thisfont,  rect,  NULL,  &textBrush);
	}
	//////////////////////////////////////////////////////////////////////////////////////
	void drawImage(Rnode * pnode){
		Image *image=pnode->bgImage->image;
		imageNode *bgimg=pnode->bgImage;
		int height=image->GetHeight(),width=image->GetWidth();

		if(bgimg->scale==true)
		{
			RenderGraph.DrawImage(image, pnode->x,pnode->y,pnode->width,pnode->height);
		}
		else if(bgimg->repeat!=0){	
			;
		}
		else
		{
			RenderGraph.DrawImage(image, pnode->x,pnode->y, 0, 0,pnode->width,pnode->height,UnitPixel);
			//RenderGraph.DrawImage(image, pnode->x,pnode->y,width,height);
		}
	}

	void drawImage(Rnode * pnode,Image *image){
		imageNode *bgimg=pnode->bgImage;
		int height=image->GetHeight(),width=image->GetWidth();

		if(bgimg->scale==true)
		{
			RenderGraph.DrawImage(image, pnode->x,pnode->y,pnode->width,pnode->height);
		}
		else if(bgimg->repeat!=0){	
			;
		}
		else
		{
			RenderGraph.DrawImage(image, pnode->x,pnode->y, 0, 0,pnode->width,pnode->height,UnitPixel);
			//RenderGraph.DrawImage(image, pnode->x,pnode->y,width,height);
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////
	void drawNode(Rnode * pnode){
		 Pen *myPen= new Pen(0xffff0000, 3); 

		RenderGraph.DrawRectangle(myPen,pnode->x,pnode->y,pnode->width,pnode->height);
		if(pnode->bgImage!=NULL){drawImage(pnode);}
		if(pnode->p!=NULL){drawText(pnode);}

	}
	/////////////////////////////////////////////////////////////////////////////////////
	void drawTree(Rnode * pnode){
		if(pnode==NULL)return;

		drawNode(pnode);
		drawTree(pnode->son);
		drawTree(pnode->next);
	}

	/////////////////////////////////////////////////////////////////////////////////////
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*
	WM_NULL---0x0000    空消息。
	0x0001----0x0087    主要是窗口消息。
	0x00A0----0x00A9    非客户区消息
	0x0100----0x0108    键盘消息
	0x0111----0x0126    菜单消息
	0x0132----0x0138    颜色控制消息
	0x0200----0x020A    鼠标消息
	0x0211----0x0213    菜单循环消息
	0x0220----0x0230    多文档消息
	0x03E0----0x03E8    DDE消息
	0x0400              WM_USER
	0x8000              WM_APP
	0x0400----0x7FFF    应用程序自定义私有消息
*/
//目前只用 键盘消息 和 鼠标消息
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MsgMgr{
public:
	RDOM *dom;
	Rnode * focus;
	INT mousex;
	INT mousey;
	Rnode * clicknode;

	void deal(MSG msg){
		if((msg.message >= 0x0200)&&(msg.message <= 0x020A)){
			mousex = LOWORD (msg.lParam) ;
			mousey = HIWORD (msg.lParam) ;
// 			wchar buf[100];
// 			wsprintf(buf,L"  x= %d   y=%d  ",mousex,mousey);
// 			MessageBox(NULL,buf,L"",MB_OK);

			if(msg.message==WM_LBUTTONDOWN)
				SendMessage(msg.hwnd, WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);

			clicknode =dom->catchNode(mousex , mousey,dom->Rtree);
			if(clicknode!=focus){
				MSG focusOut;
				focusOut.hwnd=msg.hwnd;
				focusOut.message=FOCUS_OUT;
				spreadMsg(focusOut,focus);

				focusOut.message=FOCUS_IN;
				spreadMsg(focusOut,clicknode);

				focus=clicknode;
			}
			spreadMsg(msg,clicknode);
		}
		else if((msg.message >= 0x0100)&&(msg.message <= 0x0108)){
			spreadMsg(msg,focus);
		}
		/*switch (msg.message)
		{

		case   WM_LBUTTONDOWN:
			//if()
			//  MessageBoxW(NULL,L" -_-# 对不起 由于 天气 抑或 人品原因，今天你不能登QQ -_-|||",L" ",MB_OK);
			//	mousex = LOWORD (lParam) ;
			//	mousey = HIWORD (lParam)+28 ;

			

			//wsprintf(buf,L"  x= %d   y=%d  ",mousex,mousey);
			//	MessageBox(NULL,buf,L"",MB_OK);

			//	clicknode = catchNode(mousex , mousey,RDOM);
			// 		if(clicknode->p->text!=NULL)
			// 			MessageBox(NULL,clicknode->p->text,L"f",1); 
			// 			if(clicknode!=NULL)
			// 				if(clicknode->actionList!=NULL)
			// 					clicknode->actionList->action(NULL,0,mousex,mousey);

			break;
		case WM_KEYDOWN:

			break;
		}*/
		
	};
	void spreadMsg(MSG msg,Rnode * pnode){
		if(pnode==NULL) return;
		if((unsigned int)pnode==0xcdcdcdcd)return;
		//if(pnode->action==NULL) return;
		//	pnode->action(msg,pnode);
		if(pnode->obj==NULL) return;
		pnode->obj->deal(&msg);
	}
};

///////////////////////////////////////////////////////////////////////////////////////////
class Window{
public:
	RDOM *dom;
	Render *render;
	MsgMgr *msgMgr;
	//////////////////////
	HWND hWnd;


	POINT ptWinPos;
	SIZE sizeWindow;

	HDC hdc;
	HDC hdcGdi ;
	HBITMAP hBitMap ;
	HGDIOBJ hOldBmp ;


	Window(){
		CreateCoolWindow(L"mainWIndow");
	};

	static LRESULT  CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		int wmId, wmEvent;
		PAINTSTRUCT ps;
		HDC hdc;
		INT mousex;
		INT mousey;
		Rnode * clicknode;
		wchar buf[20];

// 		MSG msg;
// 		msg.hwnd=hWnd;
// 		msg.lParam=lParam;
// 		msg.message=message;
// 		msg.wParam=wParam;

	//	msgMgr->deal(msg);		//无法调用 非静态成员
		
		switch (message)
		{

		case   WM_LBUTTONDOWN:
			//if()
			//  MessageBoxW(NULL,L" -_-# 对不起 由于 天气 抑或 人品原因，今天你不能登QQ -_-|||",L" ",MB_OK);
			//	mousex = LOWORD (lParam) ;
			//	mousey = HIWORD (lParam)+28 ;

		//	SendMessage(hWnd, WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);

			//wsprintf(buf,L"  x= %d   y=%d  ",mousex,mousey);
			//	MessageBox(NULL,buf,L"",MB_OK);

			//	clicknode = catchNode(mousex , mousey,RDOM);
			// 		if(clicknode->p->text!=NULL)
			// 			MessageBox(NULL,clicknode->p->text,L"f",1); 
			// 			if(clicknode!=NULL)
			// 				if(clicknode->actionList!=NULL)
			// 					clicknode->actionList->action(NULL,0,mousex,mousey);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			exit(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			
		}
		return 0;
	};
	 
	void CreateCoolWindow(TCHAR *szWindowClass){
		WNDCLASS wndclass;
		wndclass.style = CS_HREDRAW|CS_VREDRAW;
		wndclass.lpfnWndProc = this->wndProc;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
		wndclass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		wndclass.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
		wndclass.hInstance = NULL;
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName = szWindowClass;

		if (!::RegisterClass(&wndclass))
		{
			::MessageBox(NULL, TEXT("error!"),TEXT("error prog"), MB_ICONERROR);
			return;
		}
// 		hwnd = CreateWindow(szAppName,TEXT("nothing"),WS_OVERLAPPEDWINDOW|WS_VISIBLE,
// 			CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,NULL,NULL,NULL,NULL);

		hWnd = CreateWindowEx(WS_EX_LAYERED,szWindowClass, NULL, 
			//   WS_DLGFRAME|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_SIZEBOX|WS_MAXIMIZEBOX|
			WS_POPUP           |WS_OVERLAPPED  |  WS_MINIMIZEBOX    |WS_MAXIMIZEBOX	| WS_SYSMENU|WS_VISIBLE
			,
			100, 100, 500, 500, NULL, NULL, NULL, NULL);

		if (hWnd != NULL)
		{
			/*::ShowWindow(hWnd, SW_SHOW);*/

			RECT rect;
			GetWindowRect(hWnd, &rect);
			ptWinPos.x=rect.left;
			ptWinPos.y=rect.top;

			sizeWindow.cx=rect.right - rect.left;
			sizeWindow.cy=rect.bottom - rect.top;
			hdc = GetDC (hWnd);
			hdcGdi = CreateCompatibleDC (hdc);
			hBitMap = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
			hOldBmp = SelectObject(hdcGdi, hBitMap);
		}
		else
		{
			printf("the error code is %d \n", GetLastError());
			printf("Failed to create dialog\n");
			//bDone = true;
			return;
		}
		
		
	};
	void showWindow(){
		updatewindow();
	};
	void winLoop(void){
		MSG msg;
		HACCEL hAccelTable;

		HINSTANCE hInstance=GetModuleHandle(NULL);
		hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY121027));

		// 主消息循环:
		while (GetMessage(&msg, NULL, 0, 0))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);

				msgMgr->deal(msg);

				DispatchMessage(&msg);			
			}
		}

	};

	void updatewindow(void)
	{
		///////////////////////////////////////////////////////////////////////////////////////////////////

		static	Image *imgpng= new Image(L"dragon.png");
		//Graphics *myGraphics; 
		//HPEN hPen; // 笔句柄 
		static Pen *myPen= new Pen(0xffff0000, 3);  


// 		RECT rect;
// 		GetWindowRect(hWnd, &rect);
// 		POINT ptWinPos = {rect.left, rect.top};
// 		SIZE sizeWindow = {rect.right - rect.left, rect.bottom - rect.top};
// 
// 		HDC hdc1 = GetDC (hWnd);
// 		HDC hdcGdi = CreateCompatibleDC (hdc1);
// 		HBITMAP hBitMap = CreateCompatibleBitmap(hdc1, rect.right - rect.left, rect.bottom - rect.top);
// 		HGDIOBJ hOldBmp = SelectObject(hdcGdi, hBitMap);

	//	Graphics *myGraphics=new Graphics(hdcGdi);

		//Graphics myGraphics(hdcGdi);

		//myGraphics.DrawImage(imgpng, 0,0,400,300);
		render->RenderGraph.DrawImage(imgpng,0,0,349,265);

		///////////////////////////////////////////////////////////////////////////////////////////////////



		render->drawTree(dom->Rtree);




		///////////////////////////////////////////////////////////////////////////////////////////////////



		POINT ptSrc = {0, 0};
		// Initialize
		BLENDFUNCTION Blend = {AC_SRC_OVER, 0, 255,AC_SRC_ALPHA };
		UpdateLayeredWindow(hWnd, NULL, &ptWinPos, &sizeWindow, hdcGdi, &ptSrc, 0, &Blend, ULW_ALPHA);

// 		SelectObject(hdcGdi, hOldBmp);
// 		DeleteObject(hBitMap);
// 		DeleteDC(hdcGdi);
// 		hdcGdi = NULL;
// 		ReleaseDC(hWnd, hdc1);
	}
	///////////////////////////////////////////////////////////////////////
	void updatewindow(int sign){
		POINT ptSrc = {0, 0};
		// Initialize
		BLENDFUNCTION Blend = {AC_SRC_OVER, 0, 255,AC_SRC_ALPHA };
		UpdateLayeredWindow(hWnd, NULL, &ptWinPos, &sizeWindow, hdcGdi, &ptSrc, 0, &Blend, ULW_ALPHA);

	//	UpdateLayeredWindow(hWnd, NULL, NULL, &sizeWindow, hdcGdi, &ptSrc, 0, &Blend, ULW_ALPHA);

	}
};
// struct plugList{
// 	char name[30];
// 	void (* action)(MSG *msg,Rnode *pnode);
// };
// 
// struct plugStruct{
// 	int n;
// 	plugList * pluglist;
// };



class CoolGUI{
public:
	RDOM *dom;
	Render *render;
	MsgMgr *msgMgr;
	Window *window;
	GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_pGdiToken;
	
	map<string,Plug *(* )(void)> plugMap;		//插件集合 与xml中的自定义标签对应
	map<string,void (*)(MSG *msg,Rnode *pnode)> funMap;		//用户自定义函数 与xml中的函数名对应

	CoolGUI(){
		Gdiplus::GdiplusStartup(& m_pGdiToken,&m_gdiplusStartupInput,NULL); 
		//funMap["hello"]();
	};

	int init(void){							//CoolGUI 的构造函数 与初始化 分开

		dom=new RDOM(&plugMap,&funMap,this);
		window=new Window;
		render=new Render(window->hdcGdi);
		msgMgr=new MsgMgr;

		msgMgr->dom=dom;
		window->msgMgr=msgMgr;
		window->render=render;
		window->dom=dom;	
		return 0;
	};

 	void registerPlug(char * str,Plug *(* pfun)(void)){
 		plugMap.insert(map < string, Plug *(*)(void) >::value_type(str, pfun));
 	};
// 	void registerPlug(plugStruct * plugs){
// 		for(int i=0;i<plugs->n;i++)
// 			registerPlug(plugs->pluglist->name,plugs->pluglist->action);
// 	};

	void registerFun(char * str,void (* pfun)(MSG *msg,Rnode *pnode)){
		funMap.insert(map < string,void (*)(MSG *msg,Rnode *pnode)>::value_type(str, pfun));
	};
};

#endif