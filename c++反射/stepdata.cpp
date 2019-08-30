#include "stepdata.h"
#include <QDebug>
#include <QDataStream>
StepData::StepData() : DataInterface()
{
	m_name = "step1";
}

StepData::~StepData()
{

}

QVariant StepData::data(int roleData)
{
	return 0;
}

void StepData::reset()
{
	for each (DataInterface* var in m_boundaryConditions)
	{
		delete(var);
	}
	m_boundaryConditions.clear();
}

QDataStream& StepData::operator<< (QDataStream &in)
{
	reset();
	in >> m_version;
	QString className;
	in >> className;
	if (className != getClassName())
	{
		qDebug() << getClassName() << ":" << "read error:read other object data";
	}
	in >> m_name;
	int boundaryConditions = -1;
	in >> boundaryConditions;
	for (int i = 0; i < boundaryConditions; i++)
	{
		QString className;
		in >> className;
		DataInterface* bc = (DataInterface*)ClassFactory::getInstance()->createByClassName(className);
		*bc << in;
		m_boundaryConditions.append(bc);
	}
	return in;
}

QDataStream& StepData::operator>> (QDataStream &out)
{
	out << m_version;
	out << getClassName();
	out << m_name;
	out << m_boundaryConditions.count();
	for each (DataInterface* var in m_boundaryConditions)
	{
		out << var->getClassName();
		*var >> out;
	}
	return out;
}

REGISTER_CLASS(StepData);
