#pragma once
#include <QtWidgets>
#include <QCryptographicHash>

class FilesComp : public QWidget
{
	Q_OBJECT

public:
	FilesComp(QWidget *parent = Q_NULLPTR);
public slots:
	void closeEvent(QCloseEvent* event) override;
private slots:
	void uniqueChanged(int);
	void subDirChanged(int);
	void treeClicked(const QModelIndex&);
	//void algChanged(bool);
	void find();
private:
	void createInterface();
	void connectInit();

	bool uniqueFiles = true;
	bool subDirs = false;
	//QCryptographicHash::Algorithm alg = QCryptographicHash::Algorithm::Md5;
	QTreeView* leftTree, * rightTree;
	QFileSystemModel* leftFileModel, * rightFileModel;
	QPushButton* findBtn;
	//QRadioButton* md5RBtn, * sha1RBtn, * sha256RBtn;
	QCheckBox* uniqueCBox, * subDirsCBox;
};
