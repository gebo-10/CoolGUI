#ifndef BUTTONPLUG
#define BUTTONPLUG
#include "Plug.h"


#define BUTTON	1000
class button:public Plug{
public:
	int sign;
	static void RegisterPlug(map<string,Plug *(* )(void)> *plugMap){
		plugMap->insert(map<string, Plug *(* )(void)> :: value_type("Button", button::plug));
	};
	//注册插件时候的静态函数 由构建dom时候使用 要重写
	static Plug * plug(){
		Plug * plug =new button;
		return plug;
	};
	virtual Rnode* build(xml_node<> *docNode,Rnode *father,RDOM *Rdom,CoolGUI * instance){
		Plug::build(docNode,father,Rdom,instance);
		MessageBox(NULL,L"BUtton hello",L"",MB_OK);
		if(docNode==NULL)return NULL;
		Rnode * tmp=new Rnode;
		tmp->tag=BUTTON;
		memset(tmp,0,sizeof(Rnode));
		rapidxml::xml_attribute<char> * attr = docNode->first_attribute();
		int attrType=0;
		while(attr){
			attrType=Rdom->guiDict.findi(attr->name());
			switch(attrType){
			case ID:tmp->id=Rdom->guiDict.Dinsert(attr->value());break;
			case CLASS:									//在字典中有class 和id的映射
				for(int i=0;i<MAXCLASS;i++)
					if(tmp->cclass[i]!=0)
						tmp->cclass[i]=Rdom->guiDict.Dinsert(attr->value());
				break;
			case ENTER:
				if(strcmp(attr->value(),"true")==0)
					Rdom->setFlag(tmp,FENTER,1);
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
				iter=Rdom->funMap->find(attr->value());
				tmp->obj=this;
				tmp->obj->pnode=tmp;
				tmp->obj->action=iter->second;
				break;
			}
			attr=attr->next_attribute();
		}
		Rdom->buildDoc(docNode->first_node(),tmp);
		return tmp;
	};
	virtual int deal(MSG *msg){
		//MessageBox(NULL,L"Button deal()",L"",MB_OK);
		switch (msg->message)
		{
			
		case   WM_LBUTTONDOWN:
			//if(action!=NULL)action(msg,this->pnode);		//默认运行div中的action
			mouseOut(msg);
			break;
		case   WM_MOUSEMOVE:
				mouseIn(msg);
			
			break;
		}
		return 0;
	};
	void mouseIn(MSG *msg){

	//	MessageBox(NULL,L"mouse in",L"",MB_OK);
		
		 static Image *bg=new Image(L"bg1.png");
		

		 if(sign!=1){ 
			 this->instance->render->drawImage(this->pnode,bg);
			this->instance->window->updatewindow(0);
			sign=1;
		 }

	};
	void mouseOut(MSG *msg){
		sign=0;
		this->instance->window->updatewindow();
	};
};
#endif