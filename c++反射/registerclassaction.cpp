#include "registerclassaction.h"
#include "classfactory.h"

RegisterClassAction::RegisterClassAction(QString className, createObject func)
{
	ClassFactory::getInstance()->registerClass(className, func);
}

RegisterClassAction::~RegisterClassAction()
{

}
