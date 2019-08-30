#ifndef __DATA_INTERFACE_H__
#define __DATA_INTERFACE_H__
#include <QObject>
#include <QUuid>
#include "registerclassaction.h"
#include "classfactory.h"
#include <QVariant>
#include <QByteArray>
#include <QDataStream>
#include "macro.h"


class DataInterface : public QObject
{
public:
	CLASSNAMEFUNC(DataInterface);

	DataInterface();
	~DataInterface();
	virtual QVariant data(int roleData) { return 0; };
	virtual void	 reset() {};

	virtual QDataStream& operator<< (QDataStream &in);
	virtual QDataStream& operator>> (QDataStream &out);

// 	virtual void	 save(QByteArray *dataArray) {};//存入QByteArray一定会成功
// 	virtual void	 read(const QByteArray *dataArray) {};
protected:
	QUuid		m_uuid;
	QString		m_version;
	QString     m_className;
private:

};



#endif // !__DATA_INTERFACE_H__
