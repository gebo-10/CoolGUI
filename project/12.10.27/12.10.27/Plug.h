#ifndef COOLGUIPLUG
#define COOLGUIPLUG

#include "GUIOBJ.h"
#include "nodeType.h"

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include <iostream>
using namespace rapidxml;

class RDOM;
class CoolGUI;

class Plug:public GUIOBJ{
public:
	CoolGUI * instance;
	//注册插件 要重写
	static void RegisterPlug(map<string,Plug *(* )(void)> &plugMap){
		plugMap.insert(map<string, Plug *(* )(void)> :: value_type("Plug", plug));
	};
	//注册插件时候的静态函数 由构建dom时候使用 要重写
	static Plug * plug(){
		Plug * plug =new Plug;
		return plug;
	};
	virtual Rnode* build(rapidxml::xml_node<> *docNode,Rnode *father,RDOM *Rdom,CoolGUI * instance){		//instance 在初始化时候不能用  内部内容为错误指针
		this->instance=instance;
		return NULL;
	};

};
#endif