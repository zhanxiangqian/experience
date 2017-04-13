
#include <QPushButton>

#include <QComboBox>
#include <QHBoxLayout>
#include <QTreeView>
#include <QStringList>
#include <QFileDialog>

#include <QScatterSeries>
#include <QLineSeries>
#include <QSplitter>
#include "materialviewer.h"




MaterialViewer::MaterialViewer(QWidget *parent)
	:QDialog(parent)
{
	delete this->layout();
	QHBoxLayout *hlayout = new QHBoxLayout(this);
	this->setLayout(hlayout);

	matTree = new QTreeView();
	QVBoxLayout *vrightLayout = new QVBoxLayout();

	plabel_isar = new QLabel();
	plabel_rcs = new QLabel();

	QHBoxLayout *phlayout = new QHBoxLayout();
	phlayout->addWidget(new QLabel("ISAR"));
	phlayout->addWidget(new QLabel("Angle"));
	pline_edit = new QLineEdit();
	phlayout->addWidget(pline_edit);

	QVBoxLayout *pvlayout = new QVBoxLayout();

	QWidget *ptopright = new QWidget();
	ptopright->setLayout(pvlayout);
	pvlayout->addLayout(phlayout);
	pvlayout->addWidget(plabel_isar);

	QSplitter *spmain = new QSplitter(Qt::Horizontal, this);
	spmain->addWidget(matTree);

	QSplitter *spchild = new QSplitter(Qt::Vertical, NULL);
	spchild->addWidget(ptopright);
	spchild->addWidget(plabel_rcs);
	spmain->addWidget(spchild);
	hlayout->addWidget(spmain);
	QString path = "D:\\project\\RCS_Display\\RCS_Display\\RCS_Display\\Resources\\Published_ISAR_Aircraft_Dataset";
	fileMaterialModel = new FileMaterialModel(path, this);
	QStringList fliter;
	fliter << "*.jpg";
	fileMaterialModel->setNameFilters(fliter);
	fileMaterialModel->setNameFilterDisables(false);
	matTree->setModel(fileMaterialModel);
	QModelIndex index = fileMaterialModel->index(path);
	matTree->expand(index);      //当前项展开
	matTree->scrollTo(index);
	this->setWindowTitle("Concerto");
	connect(matTree, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onMatTreeItemClicked(const QModelIndex &)));
}

MaterialViewer::~MaterialViewer()
{

}



void MaterialViewer::onMatTreeItemClicked(const QModelIndex &index)
{
	FileMaterialModel * fmmodel = (FileMaterialModel *) matTree->model();

	QVariant var = fmmodel->data(index, Qt::DisplayRole);//!!!

	QFileInfo fi(fmmodel->filePath(index));
	if (fi.isDir())
	{
	}
	else if (fi.isFile())
	{
		QString fullname = fi.absoluteFilePath();
		QPixmap *p = new QPixmap(fullname);
		plabel_isar->setPixmap(*p);

		QPixmap *pRCS = new QPixmap("D:\\project\\RCS_Display\\RCS_Display\\RCS_Display\\Resources\\Published_ISAR_Aircraft_Dataset\\Aircraft-1\\a1_img000_RCS.png");
		pRCS->scaledToHeight(p->height());
		pRCS->scaledToWidth(p->width());
		plabel_rcs->setPixmap(*pRCS);
		QString name = fi.fileName();
		QString tmp = name.right(7);
		QString tmpangle = tmp.left(3);
		QString angle = tmpangle;
		if (tmpangle.left(1) == "0")
		{
			angle = tmpangle.right(2);
			if (angle.left(1) == "0")
			{
				angle = tmpangle.right(1);
			}
		}
		angle += "°";
		pline_edit->setText(angle);
		plabel_isar->update();
	}
}








