#ifndef UFTREEOBJECT_H_
#define UFTREEOBJECT_H_

#include <QString>
#include <QPixmap>

class UfTreeObject;
typedef QList<UfTreeObject*> DataObjectList;

class UfTreeObject : public QObject
{
	Q_OBJECT
public:
	UfTreeObject();
	~UfTreeObject();

	//! Column id
	enum
	{
		NameId,          //!< name column
		VisibilityId     //!< visibility state column
	};

	QString             entry();
	void                setEntry(QString);
	UfTreeObject*       parent() const;
	void                setParent(UfTreeObject*);
	int                 childCount() const;
	int                 childPos(const UfTreeObject*) const;
	UfTreeObject*       childObject(const int) const;
	void                appendChild(UfTreeObject*);
	void                insertChild(UfTreeObject*, int);
	void                removeChild(UfTreeObject*, const bool = false);

	int                 level() const;
	int                 position() const;
	UfTreeObject*       nextBrother() const;
	UfTreeObject*       prevBrother() const;

	QString             name() const;
	QString             text(const int = NameId) const;
	QPixmap             icon(const int = NameId) const;
	QString             toolTip(const int = NameId) const;
	QString             statusTip(const int = NameId) const;
	QString             whatsThis(const int = NameId) const;

	virtual void                children(DataObjectList&, const bool = false) const;
	virtual DataObjectList      children(const bool = false);


private:
	QString entryID;
	DataObjectList      myChildren;
	UfTreeObject*       parentObject;
};



#endif
