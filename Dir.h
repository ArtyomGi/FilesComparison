#pragma once
#include <QDir>
#include <QCryptographicHash>
#include <deque>

using namespace std;

class Dir : public QDir
{
public:

	Dir(const QDir& qdir) :
		QDir(qdir) {};
	Dir(const QString& path = QString()) :
		QDir(path) {};
	Dir(const QString& path, const QString& nameFilter,
		SortFlags sort = SortFlags(Name | IgnoreCase), Filters filter = AllEntries) :
		QDir(path, nameFilter, sort, filter) {};

	list<list<QFileInfo>> fileIntersection(const Dir&, bool unique = false,
		bool subDirs = false) const;
};
