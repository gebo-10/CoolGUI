// dict.cpp : 定义控制台应用程序的入口点。
//

//dict.c
#ifndef DICT
#define DICT

#include<stdio.h>
#include<malloc.h>
#include <vector>
using namespace std;

#define UNDEF 9999
#define OK	0
#define ERROR -1

typedef struct{
	char word[16];
	int value;
}dictItem;
typedef struct dictNode{
	char letter;		//字母
	int value;
	struct dictNode *son;
	struct dictNode *next;
}dictNode;

class Dict{
	dictNode *tire;
	int lock;
	int count;
	int allocNum;
	vector <dictItem> dictMap;
private:
	/////////////////////////////////////////////////////////////////////////////////////
	int addword(char * pword,int n,dictNode * pnode){
		dictNode *pnewson=NULL,*pnewnext=NULL;
		if(*(pword+1)==0){				//嵌套退出
			if(*pword==pnode->letter){
				pnode->value=n;
				return OK;}
		}
		if(*pword==pnode->letter){
			if(pnode->son==NULL){
				pword++;
				while(*(pword+1)!=0){
					pnewson=(dictNode*)malloc(sizeof(dictNode));
					pnewson->value=UNDEF;
					pnewson->son=pnewson->next=NULL;
					pnewson->letter=*pword++;
					pnode->son=pnewson;
					pnode=pnewson;
				}
				pnewson=(dictNode*)malloc(sizeof(dictNode));
				pnewson->value=n;
				pnewson->son=pnewson->next=NULL;
				pnewson->letter=*pword;
				pnode->son=pnewson;
				return OK;
			}
			addword(++pword,n,pnode->son);
		}else{
			if(pnode->next==NULL){
				pnewnext=(dictNode*)malloc(sizeof(dictNode));
				pnewnext->value=UNDEF;
				pnewnext->son=pnewnext->next=NULL;
				pnewnext->letter=*pword++;

				pnode->next=pnewnext;
				pnode=pnewnext;
				if(*pword==0){pnewnext->value=n;return OK;}
				while(*(pword+1)!=0){
					pnewson=(dictNode*)malloc(sizeof(dictNode));
					pnewson->value=UNDEF;
					pnewson->son=pnewson->next=NULL;
					pnewson->letter=*pword++;

					pnode->son=pnewson;
					pnode=pnewson;
				}
				pnewson=(dictNode*)malloc(sizeof(dictNode));
				pnewson->value=n;
				pnewson->son=pnewson->next=NULL;
				pnewson->letter=*pword;
				pnode->son=pnewson;
				return OK;
			}
			addword(pword,n,pnode->next);
		}
		return OK;
	};

	/////////////////////////////////////////////////////////////////////////////////////




	int dicti(char *pword,dictNode *pnode ){
		if(*(pword+1)==0){				//嵌套退出
			if(*pword==pnode->letter)
				return pnode->value;
		}
		if(*pword==pnode->letter){
			if(pnode->son==NULL)return UNDEF;
			return dicti(++pword,pnode->son);
		}else{
			if(pnode->next==NULL)return UNDEF;
			return dicti(pword,pnode->next);
		}
	
	};


public:
	Dict(dictItem *pitem,int itemNum){

		count=0;
		allocNum=500;

		this->tire=new dictNode;
		this->tire->letter='a';
		this->tire->value=UNDEF;
		this->tire->son=this->tire->next=NULL;

		for(int i=0;i<itemNum;i++){
			addword(pitem[i].word,pitem[i].value,tire);
			dictMap.push_back(pitem[i]);
		}
	};
	~Dict(){};
	/////////////////////////////////////////////////////////////////////////////////////
	int insert(char * pword,int n){
		return addword( pword,n,tire);

	}
	int Dinsert(char * pword){
		addword( pword,allocNum++,tire);
		return allocNum-1;

	}
	int findi(char *word){
		return dicti(word,tire);
	};


	char * findc(int n){
		return dictMap[n-1].word;
	};

};

/////////////////////////////////////////////////////////////////////////////////////
/*



int itemnum=sizeof(items)/sizeof(dictItem);
*/

#endif
