#include "ComparisonWorker.h"

void ComparisonWorker::setDirs(const Dir& _dir1, const Dir& _dir2)
{
    dir1 = _dir1;
    dir2 = _dir2;
}

void ComparisonWorker::setOptions(bool _uniqueFiles, bool _subDirs)
{
    uniqueFiles = _uniqueFiles;
    subDirs = _subDirs;
}

void ComparisonWorker::compare()
{
    emit progressStatus(1);
    ComparisonResult res = dir1.fileIntersection(dir2, uniqueFiles, subDirs);
    emit progressStatus(0);
    emit compareFinish(res);
    emit finished();
}
