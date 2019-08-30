#include "materialsavedialog.h"
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSpacerItem>

MaterialSaveDialog::MaterialSaveDialog(QWidget* parent )
:QDialog(parent)
{

}


MaterialSaveDialog::~MaterialSaveDialog()
{

}

void MaterialSaveDialog::initUI()
{
	QVBoxLayout *mainLayout = new QVBoxLayout;
	this->setLayout(mainLayout);

	QHBoxLayout *pathLayout = new QHBoxLayout;
	mainLayout->addLayout(pathLayout);
	QLabel *pathLabel = new QLabel("path");
	combobox = new QComboBox;
	combobox->addItems(comboboxList);
	pathLayout->addWidget(pathLabel);
	pathLayout->addWidget(combobox);

	QHBoxLayout *nameLayout = new QHBoxLayout;
	mainLayout->addLayout(nameLayout);
	QLabel *nameLabel = new QLabel("name");
	lineEdit = new QLineEdit;
	nameLayout->addWidget(nameLabel);
	nameLayout->addWidget(lineEdit);

	QHBoxLayout *buttonLayout = new QHBoxLayout;
	mainLayout->addLayout(buttonLayout);
	QSpacerItem *spacerItem = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	buttonLayout->addSpacerItem(spacerItem);
	okButton = new QPushButton("OK");
	okButton->setEnabled(false);
	buttonLayout->addWidget(okButton);
	cancelButton = new QPushButton("Cancel");
	buttonLayout->addWidget(cancelButton);

	connect(okButton, SIGNAL(clicked()), this, SLOT(sl_OkButClicked()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(sl_CancelButClicked()));
	connect(lineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(sl_LineEidtFinished()));
}

void MaterialSaveDialog::sl_OkButClicked()
{
	path = combobox->currentText();
	this->accept();
}

void MaterialSaveDialog::sl_CancelButClicked()
{
	this->close();
}

void MaterialSaveDialog::sl_LineEidtFinished()
{
	name = lineEdit->text();
	bool flag = true;
	for (int i = 0; i < existNames.length(); i++ )
	{
		if (name==existNames.at(i))
		{
			flag = false;
			break;
		}
	}

	if (name.isEmpty() )
	{
		flag = false;
	}

	okButton->setEnabled(flag);
}