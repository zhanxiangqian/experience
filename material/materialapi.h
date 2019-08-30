#ifndef __MATERIAL_API_H__
#define __MATERIAL_API_H__
#include <QWidget>
#include <QObject>
#include "materialglobal.h"
#include "materialviewer.h"
#include "materialdimensionconvert.h"

class MaterialLib_EXPORT MaterialViewerHandle : public QObject
{
	Q_OBJECT
public:
	MaterialViewerHandle(QWidget* parent);
	~MaterialViewerHandle();
	QWidget* getWidget();

public slots:
	void openMaterial();
	void exportMaterial();
	void createMaterial();
	void editMaterial();
	void saveMaterlial();
	void addToCurrent();
	void addToLibrary();
	void deleteMaterial();

private:
	MaterialViewer* m_view;
	IMaterialDimensionConvert *m_dimensionConvert;
};


#endif