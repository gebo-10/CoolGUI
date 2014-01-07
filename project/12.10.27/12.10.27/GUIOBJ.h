#ifndef COOLGUIOBJ
#define COOLGUIOBJ
struct Rnode;
class GUIOBJ{
public:
	Rnode * pnode;
	void (* action)(MSG *msg,Rnode *pnode);
	virtual int deal(MSG *msg){
		if(action!=NULL)action(msg,this->pnode);		//默认运行div中的action
		return 0;
	};
};
#endif