#include "forceconditiondata.h"
#include <QDebug>
ForceConditionData::ForceConditionData() : DataInterface()
{
	m_name = "force1";
}

ForceConditionData::~ForceConditionData()
{

}

QVariant ForceConditionData::data(int roleData)
{
	return 0;
}

void ForceConditionData::reset()
{

}

QDataStream& ForceConditionData::operator<< (QDataStream &in)
{
	reset();
	in >> m_version;
	QString className;
	in >> className;
	if (className != getClassName())
	{
		qDebug() << getClassName() << ":read error";
	}
	in >> m_name;
	return in;
}

QDataStream& ForceConditionData::operator>> (QDataStream &out)
{
	out << m_version;
	QString className = getClassName();
	out << className;
	out << m_name;
	return out;
}

REGISTER_CLASS(ForceConditionData);
