#include "uftreeobject.h"

UfTreeObject::UfTreeObject()
{
	parentObject = NULL;
}

UfTreeObject::~UfTreeObject()
{
}

void UfTreeObject::setEntry(QString entry)
{
	entryID = entry;
}


QString UfTreeObject::entry()
{
	return entryID;
}

int UfTreeObject::childCount() const
{
	return myChildren.count();
}

int UfTreeObject::childPos(const UfTreeObject* child) const
{
	return myChildren.indexOf((UfTreeObject*)child);
}

UfTreeObject* UfTreeObject::childObject(const int pos) const
{
	return myChildren.at(pos);
}

void UfTreeObject::appendChild(UfTreeObject* child)
{
	myChildren.append(child);
	child->setParent(this);
}
void UfTreeObject::insertChild(UfTreeObject* child, int pos)
{
	myChildren.insert(pos, child);
	child->setParent(this);
}
void UfTreeObject::removeChild(UfTreeObject* child, const bool del)
{
	myChildren.removeOne(child);
	child->setParent(NULL);
	if (del)
	{
		child->deleteLater();
	}
}

int UfTreeObject::level() const
{
	int lev = 0;
	UfTreeObject* p = parent();
	while (p) {
		p = p->parent();
		lev++;
	}
	return lev;
}

int UfTreeObject::position() const
{
	UfTreeObject* parentObj = parent();
	if (parentObj)
	{
		return parentObj->childPos(this);
	}
	else
	{
		return -1;
	}
}

UfTreeObject* UfTreeObject::parent() const
{
	return parentObject;
}

void UfTreeObject::setParent(UfTreeObject* parent)
{
	parentObject = parent;
}

UfTreeObject* UfTreeObject::nextBrother() const
{
	int next = parent()->childPos(this) + 1;
	return childObject(next);
}

UfTreeObject* UfTreeObject::prevBrother() const
{
	int pre = parent()->childPos(this) - 1;
	return childObject(pre);
}


QString UfTreeObject::name() const
{
	return "name";
}

QString UfTreeObject::text(const int ID) const
{
	if (ID == 0)
	{
		return entryID;
	}
	return "text";
}

QPixmap UfTreeObject::icon(const int ID) const
{
	return QPixmap();
}

QString UfTreeObject::toolTip(const int ID) const
{
	return "toolTip";
}

QString UfTreeObject::statusTip(const int ID) const
{
	return "statusTip";
}

QString UfTreeObject::whatsThis(const int ID) const
{
	return "whatsThis";
}

void UfTreeObject::children(DataObjectList& lst, const bool rec) const
{
	for (DataObjectList::const_iterator it = myChildren.begin(); it != myChildren.end(); ++it)
	{
		lst.append(*it);
		if (rec)
			(*it)->children(lst, rec);
	}

}

DataObjectList UfTreeObject::children(const bool rec)
{
	DataObjectList lst;
	children(lst, rec);
	return lst;
}
