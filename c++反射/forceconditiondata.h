#ifndef __FORCE_CONDITION_DATA_H__
#define __FORCE_CONDITION_DATA_H__

#include "datainterface.h"

class ForceConditionData : public DataInterface
{
public:
	CLASSNAMEFUNC(ForceConditionData);
	ForceConditionData();
	~ForceConditionData();
	virtual void	 reset();
	virtual QVariant data(int roleData);
	virtual QDataStream& operator<< (QDataStream &in);
	virtual QDataStream& operator>> (QDataStream &out);

private:
	QString m_name;
};


#endif