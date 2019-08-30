#include "datainterface.h"

DataInterface::DataInterface()
{
	m_uuid = QUuid::createUuid();
	m_version = "0.0";
}

DataInterface::~DataInterface()
{
}

QDataStream& DataInterface::operator<< (QDataStream &in)
{
	return in;
}

QDataStream& DataInterface::operator>> (QDataStream &out)
{
	return out;
}
