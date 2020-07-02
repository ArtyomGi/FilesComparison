#pragma once
#include <QtWidgets>
#include <QCryptographicHash>
#include <QLabel>
#include <list>
#include "ComparisonResult.h"

using namespace std;

class FilesComp : public QWidget
{
	Q_OBJECT

public:
	FilesComp(QWidget *parent = Q_NULLPTR);

public slots:
	void closeEvent(QCloseEvent* event) override;
	void onCompareFinished(ComparisonResult);
	void statusChanged(int);

private slots:
	void uniqueChanged(int);
	void subDirChanged(int);
	void treeClicked(const QModelIndex&);
	//void algChanged(bool);
	void find();

private:
	void createInterface();
	void connectInit();
	void createTreeViewer(const ComparisonResult&);
	void createTableViewer(const ComparisonResult&);

	bool uniqueFiles = true;
	bool subDirs = false;
	//QCryptographicHash::Algorithm alg = QCryptographicHash::Algorithm::Md5;
	QTreeView* leftTree, * rightTree;
	QFileSystemModel* leftFileModel, * rightFileModel;
	QPushButton* findBtn;
	//QRadioButton* md5RBtn, * sha1RBtn, * sha256RBtn;
	QCheckBox* uniqueCBox, * subDirsCBox;
	QLabel* statusLabel;
	int workingNumber = 0;
};
