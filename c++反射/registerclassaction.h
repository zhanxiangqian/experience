#ifndef __REGISTER_CLASS_ACTION_H__
#define __REGISTER_CLASS_ACTION_H__
#include <QString>

typedef void* (*createObject)();

class RegisterClassAction 
{
public:
	RegisterClassAction(QString className, createObject func);//在构造函数中实现注册
	~RegisterClassAction();
};

#define REGISTER_CLASS(className)\
className* create##className(){\
        return new className();\
}\
RegisterClassAction reg##className(#className, (createObject)create##className);

#define CLASSNAMEFUNC(className)\
virtual QString  getClassName (){\
        return #className;\
}
#endif