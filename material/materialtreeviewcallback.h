#ifndef TREEVIEWCALLBACK_H_
#define TREEVIEWCALLBACK_H_

class TreeViewCallBack
{
public: 
	virtual ~TreeViewCallBack(){}
public:
	virtual void needUpdate() = 0;
};

#endif//TREEVIEWCALLBACK_H_