#pragma once
#include <QObject>
#include "Dir.h"
#include "ComparisonResult.h"

class ComparisonWorker : public QObject
{
    Q_OBJECT

public:
    ComparisonWorker(bool _uniqueFiles = false, bool _subDirs = false) :
        uniqueFiles(_uniqueFiles), subDirs(_subDirs) {}

    void setDirs(const Dir& _dir1, const Dir& _dir2);
    void setOptions(bool _uniqueFiles, bool _subDirs);

public slots:
    void compare();

signals:
    void progressStatus(int);
    void finished();
    void compareFinish(ComparisonResult);

private:
    Dir dir1, dir2;
    bool uniqueFiles, subDirs;
};

