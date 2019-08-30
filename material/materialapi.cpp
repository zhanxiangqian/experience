#include "materialapi.h"
#include "materialconfigparser.h"

MaterialViewerHandle::MaterialViewerHandle(QWidget* parent)
{
	m_dimensionConvert = new MaterialDimensionConvert;
	m_dimensionConvert->setDimensionDocPath(MaterialConfigParser::configs[MaterialConfigParser::dimensionPath]);
	m_dimensionConvert->readDimension();
	m_view = new MaterialViewer(m_dimensionConvert);
	m_view->setParent(parent);
}

void MaterialViewerHandle::openMaterial()
{
	m_view->selectMatDatabase();
}

void MaterialViewerHandle::exportMaterial()
{
	m_view->exportMaterial();

}

void MaterialViewerHandle::createMaterial()
{
	m_view->createMat();
}

void MaterialViewerHandle::editMaterial()
{
	m_view->editMat();
}

void MaterialViewerHandle::saveMaterlial()
{
	m_view->saveMat();
}

void MaterialViewerHandle::addToCurrent()
{
	m_view->addMatFromDatabaseToCurrent();
}

void MaterialViewerHandle::addToLibrary()
{
	m_view->addCurrentMatToDatabase();
}

void MaterialViewerHandle::deleteMaterial()
{
	m_view->deleteMatFromCurrent();
}

QWidget* MaterialViewerHandle::getWidget()
{
	return m_view;
}

MaterialViewerHandle::~MaterialViewerHandle()
{
	delete(m_view);
	delete(m_dimensionConvert);
}
