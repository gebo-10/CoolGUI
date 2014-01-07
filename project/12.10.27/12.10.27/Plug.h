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
	//ע���� Ҫ��д
	static void RegisterPlug(map<string,Plug *(* )(void)> &plugMap){
		plugMap.insert(map<string, Plug *(* )(void)> :: value_type("Plug", plug));
	};
	//ע����ʱ��ľ�̬���� �ɹ���domʱ��ʹ�� Ҫ��д
	static Plug * plug(){
		Plug * plug =new Plug;
		return plug;
	};
	virtual Rnode* build(rapidxml::xml_node<> *docNode,Rnode *father,RDOM *Rdom,CoolGUI * instance){		//instance �ڳ�ʼ��ʱ������  �ڲ�����Ϊ����ָ��
		this->instance=instance;
		return NULL;
	};

};
#endif