#ifndef LIST_H
#define LIST_H
#include <windows.h>

class Node
{
public:
	Node(wchar_t* input=L""){wcscpy_s(SearchInput,MAX_PATH,input);};
	virtual operator wchar_t*()const {return SearchInput;};
protected:
	wchar_t SearchInput[MAX_PATH];
};


class List
{
public:
	List():_arrSize(5),_position(0)
	{
		_arrSize=5;
		_pNode=new Node[_arrSize];
	}
	~List()
	{
		delete [] _pNode;
	}
	bool AddNode(Node& newNode)
	{
			if (_position>=_arrSize)
			{
				_arrSize+=5;
				Node* _tmpNode=new Node[_arrSize];
				for (int i=0;i<_arrSize;i++)
				{
					_tmpNode[i]=_pNode[i];
				}
				delete []_pNode;
				_pNode=_tmpNode;
			}
			_pNode[_position++]=newNode;
			return true;
	}
	bool ClearAll()
	{
		delete [] _pNode;
		_position=0;
		_arrSize=5;
		_pNode=new Node[_arrSize];
		return (_pNode!=NULL);
	}
	int GetPosition(){return _position;}
	Node GetNode(int index){return _pNode[index];}


private:
	Node* _pNode;
	int _position;
	int _arrSize;
};









#endif