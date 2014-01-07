//
#ifndef CSSPARSER
#define CSSPARSER

#include <stdio.h>
#include <iostream>
using namespace std;
#define FILE 1
#define CSS 2
struct cssAttr{
	char * name;
	char * value;
	cssAttr *next;
};

struct cssItem{
	char *cssSelect;
	cssAttr *attr;
	cssItem *next;
};
class cssParser{
private:

	char *dataBuf;
public:
	cssItem *cssSet;

	cssParser(int type,char * str){
		if(type==CSS){
			parser(str);
		}else if(type==FILE){
			//���ļ�
			std::ifstream cssFile(str,std::ios::in);
			if(!cssFile)
			{
				std::cout<<"�����Դ��ļ�"<<std::endl;
				exit(1);
			}
			//���ļ�
			cssFile.seekg(0, ios::end);      //�����ļ�ָ�뵽�ļ�����β��
			streampos pos = cssFile.tellg();  //��ȡ�ļ�ָ���λ��
			//cout << "File size: " << pos << endl;
			cssFile.seekg(0, ios::beg);
			int size=pos;
			dataBuf=new char[size+1];
			dataBuf[size-1]='\0';
			dataBuf[size]='\0';
			cssFile.read(dataBuf,size);
			parser(dataBuf);
		}

	};
	~cssParser(){
		delete dataBuf;
		free_cssItem(cssSet);

	};
	void freeAttr(cssAttr * attr){
		if(attr->next==NULL)
			delete attr;
		else freeAttr(attr->next);
	};
	void free_cssItem(cssItem * item){
		if(item->next==NULL){
			freeAttr(item->attr);
			delete item;
		}
		else free_cssItem(item->next);
	};

	int parser(char *data){
		char *pbuf=data,*pbuf2=data;
		cssItem *ptmp=NULL;

		while(*pbuf2!='\0'){
			if(*pbuf2=='\n')
				{*pbuf2 =' ';}
			pbuf2++;
		}

		while(*pbuf!='\0'){
			cssItem *pcssItem=new cssItem;
			memset(pcssItem,0,sizeof(cssItem));
			if(ptmp!=NULL)
				ptmp->next=pcssItem;
			else 
				cssSet= pcssItem;
			ptmp=pcssItem;
			while(*pbuf==' ')pbuf++;
			ptmp->cssSelect=pbuf;

			while((*pbuf!=' ')&&(*pbuf!='{')){pbuf++;}
			while((*pbuf==' ')||(*pbuf=='{')){*pbuf='\0';pbuf++;}//ȥ�ո�

			cssAttr * pattr=NULL;
			while ((*pbuf!='\0')){				//��n������
				cssAttr *pattrtmp=new cssAttr;
				memset(pattrtmp,0,sizeof(cssAttr));
				if(pattr!=NULL)
					pattr->next=pattrtmp;
				else 
					ptmp->attr= pattrtmp;
				pattr=pattrtmp;

				pattr->name=pbuf;								//��������

				while((*pbuf!=' ')&&(*pbuf!=':')){pbuf++;}
				while((*pbuf==' ')||(*pbuf==':')){*pbuf='\0';pbuf++;}

				pattr->value=pbuf;								//������ֵ

				while((*pbuf!=' ')&&(*pbuf!=';')){pbuf++;}
				while((*pbuf==' ')||(*pbuf==';')){*pbuf='\0';pbuf++;}

				if(*pbuf=='}'){*pbuf++='\0';break;}
			}

		}
		return 0;
	};

};

#endif