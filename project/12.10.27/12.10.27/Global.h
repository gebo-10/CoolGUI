#ifndef GLOBAL
#define GLOBAL

#include<Windows.h>
#include <comdef.h>//初始化一下com口
#ifndef ULONG_PTR
#define ULONG_PTR unsigned long*
#include "GdiPlus.h"
using namespace Gdiplus;
#pragma comment (lib,"GdiPlus.lib")
#endif



#include <iostream>
#include <vector>
#include <map>
#include <string>

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include "cssParser.h"
#include "nodeType.h"
#include "dict.h"

#include "GUIOBJ.h"
#include "Plug.h"


using namespace rapidxml;
using std::vector;
using std::map;

#define FENTER	1
#define YES		8888
#define NO		7777




////////////////////////////////////////////////////////////////////////////////////////////////////
#define XML		20
#define DIV		21
#define P		22

#define IMG		23
#define IMAGE	23

#define LAYER	25

#define ID		40
#define CLASS	41

#define HEIGHT	42
#define WIDTH	43
#define X		44
#define Y		45
#define WIDTH	46
#define OPACITY	47
#define REPEAT	48
#define SCALE	49	
#define CLIP	50
#define FONTCOLOR	51
#define FONTSIZE	52
#define FONT		53

#define ENTER		54

#define TOP			55
#define LEFT		56

#define SRC			57

#define ACTION		58
///////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
//coolGUI的消息id字段
#define COOLGUI_MSG			WM_USER + 100
#define USER_MSG			WM_USER + 200

#define FOCUS_OUT		(COOLGUI_MSG+1)
#define FOCUS_IN		(COOLGUI_MSG+2)

#define MOUSE_OUT
#define MOUSE_IN

#endif