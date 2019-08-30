#ifndef __CLASS_FACTORY_H__
#define __CLASS_FACTORY_H__
#include <QString>
#include <QMap>
typedef void* (*createObject)();
class ClassFactory
{
public:
	~ClassFactory();
	static ClassFactory* getInstance();

	void	registerClass(QString className, createObject func);
	void*	createByClassName(QString className);
private:
	ClassFactory();
	static ClassFactory* m_instance;

private:
	QMap<QString, createObject> m_createFuncMap;
};


#endif // !__CLASS_FACTORY_H__
