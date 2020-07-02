#include "FilesComp.h"
#include "Dir.h"
#include "ComparisonWorker.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QTableView>
#include <set>
#include <map>
#include <fstream>

FilesComp::FilesComp(QWidget *parent)
	: QWidget(parent)
{
	setWindowTitle("Files Comparison");
	qRegisterMetaType<ComparisonResult>();
	createInterface();
	connectInit();
}

void FilesComp::createInterface()
{
	//QGroupBox* algBox = new QGroupBox(tr("Algorithm"));

	/*md5RBtn = new QRadioButton(tr("&MD5"));
	md5RBtn->setChecked(true);
	sha1RBtn = new QRadioButton(tr("&SHA1"));
	sha256RBtn = new QRadioButton(tr("S&HA256"));*/

	/*QVBoxLayout* algLayout = new QVBoxLayout;
	algLayout->addWidget(md5RBtn);
	algLayout->addWidget(sha1RBtn);
	algLayout->addWidget(sha256RBtn);*/

	//algBox->setLayout(algLayout);

	uniqueCBox = new QCheckBox(tr("Show identical files in same dir"));
	subDirsCBox = new QCheckBox(tr("Include subdirectories"));

	/*QVBoxLayout* CheckBoxLayout = new QVBoxLayout;
	CheckBoxLayout->addWidget(uniqueCBox);
	CheckBoxLayout->addWidget(subDirsCBox);*/

	QHBoxLayout* topLayout = new QHBoxLayout;
	//topLayout->addWidget(algBox, 1);
	//topLayout->addStretch();
	topLayout->addWidget(uniqueCBox);
	topLayout->addWidget(subDirsCBox);

	findBtn = new QPushButton(tr("Find binary identical files"));
	findBtn->setDisabled(true);

	QLabel* infoLabel = new QLabel(tr("Please choose two dirs for comparison:"));

	leftFileModel = new QFileSystemModel;
	leftFileModel->setRootPath("/");
	leftFileModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
	leftFileModel->sort(0);

	leftTree = new QTreeView();
	leftTree->setModel(leftFileModel);
	leftTree->hideColumn(1);
	leftTree->hideColumn(2);
	leftTree->hideColumn(3);

	rightFileModel = new QFileSystemModel;
	rightFileModel->setRootPath("/");
	rightFileModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
	rightFileModel->sort(0);

	rightTree = new QTreeView();
	rightTree->setModel(rightFileModel);
	rightTree->hideColumn(1);
	rightTree->hideColumn(2);
	rightTree->hideColumn(3);

	QHBoxLayout* treeLayout = new QHBoxLayout;
	treeLayout->addWidget(leftTree);
	treeLayout->addWidget(rightTree);
	
	statusLabel = new QLabel(tr("Status: Ready"));

	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addLayout(topLayout);
	mainLayout->addWidget(infoLabel);
	mainLayout->addLayout(treeLayout, 3);
	mainLayout->addWidget(findBtn);
	mainLayout->addWidget(statusLabel);
	
	setLayout(mainLayout);
}

void FilesComp::connectInit()
{
	/*connect(md5RBtn, SIGNAL(toggled(bool)), this, SLOT(algChanged(bool)));
	connect(sha1RBtn, SIGNAL(toggled(bool)), this, SLOT(algChanged(bool)));
	connect(sha256RBtn, SIGNAL(toggled(bool)), this, SLOT(algChanged(bool)));*/

	connect(uniqueCBox, SIGNAL(stateChanged(int)), this, SLOT(uniqueChanged(int)));
	connect(subDirsCBox, SIGNAL(stateChanged(int)), this, SLOT(subDirChanged(int)));

	connect(findBtn, SIGNAL(clicked()), this, SLOT(find()));

	connect(leftTree, SIGNAL(clicked(const QModelIndex&)), this, SLOT(treeClicked(const QModelIndex&)));
	connect(rightTree, SIGNAL(clicked(const QModelIndex&)), this, SLOT(treeClicked(const QModelIndex&)));
}

void FilesComp::uniqueChanged(int state)
{
	if (state == Qt::CheckState::Checked) uniqueFiles = false;
	else uniqueFiles = true;
}

void FilesComp::subDirChanged(int state)
{
	if (state == Qt::CheckState::Checked) subDirs = true;
	else subDirs = false;
}

//void FilesComp::algChanged(bool state)
//{
//	if (md5RBtn->isChecked()) alg = QCryptographicHash::Algorithm::Md5;
//	else if (sha1RBtn->isChecked()) alg = QCryptographicHash::Algorithm::Sha1;
//	else alg = QCryptographicHash::Algorithm::Sha256;
//}

void FilesComp::treeClicked(const QModelIndex& index)
{
	if (leftTree->selectionModel()->currentIndex().isValid()
		&& rightTree->selectionModel()->currentIndex().isValid())
		findBtn->setDisabled(false);
}

void FilesComp::find()
{
	QString leftPath = leftFileModel->filePath(leftTree->selectionModel()->currentIndex());
	QString rightPath = rightFileModel->filePath(rightTree->selectionModel()->currentIndex());

	ComparisonWorker* compWorker = new ComparisonWorker(uniqueFiles, subDirs);
	compWorker->setDirs(leftPath, rightPath);
	
	QThread* thread = new QThread;
	compWorker->moveToThread(thread);

	connect(thread, SIGNAL(started()), compWorker, SLOT(compare()));
	connect(compWorker, SIGNAL(finished()), thread, SLOT(quit()));
	connect(compWorker, SIGNAL(finished()), compWorker, SLOT(deleteLater()));
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

	connect(compWorker, SIGNAL(compareFinish(ComparisonResult)), this, SLOT(onCompareFinished(ComparisonResult)));
	connect(compWorker, SIGNAL(progressStatus(int)), this, SLOT(statusChanged(int)));
	
	thread->start();
}

void FilesComp::onCompareFinished(ComparisonResult result)
{
	if (result.subDirs)
		createTreeViewer(result);
	else createTableViewer(result);
}

void FilesComp::createTreeViewer(const ComparisonResult& result)
{
	QString leftPath = result.dir1;
	QString rightPath = result.dir2;

	QTreeWidget* resultWidget = new QTreeWidget();
	resultWidget->setWindowTitle(tr("Result of comparing"));
	resultWidget->setColumnCount(1);
	resultWidget->setHeaderLabels(QStringList() << "Files");

	int currRow = 0;
	set<QString> preventDuplicates;

	auto files = result.result;

	for (const auto& group : files) // fill the tree
	{
		QTreeWidgetItem* topLevel = new QTreeWidgetItem(resultWidget);
		topLevel->setText(0, QString::number(currRow + 1));

		for (const auto& item : group)
		{
			if (rightPath == leftPath) // check if result vector have duplicates
			{
				if (preventDuplicates.find(item.absoluteFilePath()) != preventDuplicates.end())
					continue; // if we have already created row with that file then skip it
				else preventDuplicates.insert(item.absoluteFilePath());
			}

			QTreeWidgetItem* subElem = new QTreeWidgetItem(topLevel);
			subElem->setText(0, item.absoluteFilePath());
		}

		preventDuplicates.clear();
		currRow++;
	}

	resultWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	resultWidget->show();
}

void FilesComp::createTableViewer(const ComparisonResult& result)
{
	QString leftPath = result.dir1;
	QString rightPath = result.dir2;

	QTableWidget* resultWidget = new QTableWidget;

	resultWidget->setWindowTitle(tr("Result of comparing"));
	resultWidget->setColumnCount(2);
	resultWidget->setHorizontalHeaderLabels(QStringList()
		<< leftPath
		<< rightPath);

	int leftRow = 0, rightRow = 0;
	set<QString> preventDuplicates;

	auto files = result.result;

	for (const auto& group : files) // fill the table
	{
		for (const auto& item : group)
		{
			if (rightPath == leftPath) // check if result vector have duplicates
			{
				if (preventDuplicates.find(item.absoluteFilePath()) != preventDuplicates.end())
					continue; // if we have already created row with that file then skip it
				else preventDuplicates.insert(item.absoluteFilePath());
			}
			if (item.absolutePath() == leftPath)
			{
				if (leftRow >= rightRow) resultWidget->insertRow(resultWidget->rowCount());
				resultWidget->setItem(leftRow, 0, new QTableWidgetItem(item.absoluteFilePath()));
				leftRow++;
			}
			if (item.absolutePath() == rightPath)
			{
				if (rightRow >= leftRow) resultWidget->insertRow(resultWidget->rowCount());
				resultWidget->setItem(rightRow, 1, new QTableWidgetItem(item.absoluteFilePath()));
				rightRow++;
			}
		}
		preventDuplicates.clear();
		leftRow = rightRow = std::max(leftRow, rightRow) + 1;
		resultWidget->insertRow(resultWidget->rowCount());
	}

	resultWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	resultWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	resultWidget->resizeColumnsToContents();

	resultWidget->show();
}

void FilesComp::statusChanged(int newStatus)
{
	if (newStatus)
	{
		workingNumber++;
		statusLabel->setText(QString("Status: Working (%1)...").arg(workingNumber));
	}
	else
	{
		workingNumber--;

		if(workingNumber) statusLabel->setText(QString("Status: Working (%1)...").arg(workingNumber));
		else  statusLabel->setText(tr("Status: Ready"));
	}
}

void FilesComp::closeEvent(QCloseEvent* event)
{
	QApplication::instance()->quit();
}
