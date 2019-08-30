#ifndef __STEP_DATA_H__
#define __STEP_DATA_H__

#include "datainterface.h"
#include <QList>
class StepData : public DataInterface
{
public:
	CLASSNAMEFUNC(StepData);
	StepData();
	~StepData();
	virtual QVariant data(int roleData);
	virtual void	 reset();
	virtual QDataStream& operator<< (QDataStream &in);
	virtual QDataStream& operator>> (QDataStream &out);

public:
	QList<DataInterface*> m_boundaryConditions;//force,pressure,displacement and so on.
private:
	QString m_name;
};
#endif