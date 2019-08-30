#include "classfactory.h"

ClassFactory::ClassFactory()
{
}

ClassFactory::~ClassFactory()
{
}

ClassFactory* ClassFactory::m_instance = new ClassFactory();

ClassFactory* ClassFactory::getInstance() 
{
	return m_instance;
}

void ClassFactory::registerClass(QString className, createObject func) 
{
	m_createFuncMap.insert(className, func);
}

void* ClassFactory::createByClassName(QString className) 
{
	QMap<QString, createObject>::const_iterator iter;
	iter = m_createFuncMap.find(className);
	if (iter == m_createFuncMap.end()) {
		return nullptr;
	}
	else {
		return iter.value()();
	}
}
