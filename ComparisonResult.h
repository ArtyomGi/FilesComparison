#pragma once
#include <list>
#include <QFileInfo>
#include <QString>

using namespace std;

struct ComparisonResult
{
	list<list<QFileInfo>> result;
	bool uniqueFiles, subDirs;
	QString dir1, dir2;

	ComparisonResult():
		uniqueFiles(false), subDirs(false) {};

	ComparisonResult(list<list<QFileInfo>> _result,
		QString _dir1, QString _dir2, bool _uniqueFiles, bool _subDirs) :
		result(_result), dir1(_dir1), dir2(_dir2),
		uniqueFiles(_uniqueFiles), subDirs(_subDirs) {}

};

Q_DECLARE_METATYPE(ComparisonResult);