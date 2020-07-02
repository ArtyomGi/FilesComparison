#include "Dir.h"
#include "File.h"
#include <fstream>
#include <QDirIterator>

template<typename InputIterator1, typename InputIterator2>
bool range_equal(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2)
{
	while (first1 != last1 && first2 != last2)
	{
		if (*first1 != *first2) return false;
		++first1;
		++first2;
	}
	return (first1 == last1) && (first2 == last2);
}

list<list<QFileInfo>> Dir::fileIntersection(const Dir& anotherDir, bool unique, bool subDirs) const
{
    if (!exists() || !anotherDir.exists()) return list<list<QFileInfo>>();

	QDirIterator::IteratorFlags flags = QDirIterator::NoIteratorFlags;

	if (subDirs) flags = QDirIterator::Subdirectories;
	
	QDirIterator it(this->absolutePath(), QDir::Files, flags);

	qint64 size;
    map<qint64, list<QFileInfo>> FileSizes;


	while(it.hasNext())
	{
		it.next();
		size = it.fileInfo().size();

		FileSizes.emplace(make_pair(size, list<QFileInfo>()));
		FileSizes[size].push_back(it.fileInfo());
	}

	QDirIterator it2(anotherDir.absolutePath(), QDir::Files, flags);

	while (it2.hasNext())
	{
		it2.next();
		size = it2.fileInfo().size();

		FileSizes.emplace(make_pair(size, list<QFileInfo>()));
		FileSizes[size].push_back(it2.fileInfo());
	}

	list<QFileInfo> equalFiles;
	list<list<QFileInfo>> res;

	for (auto& FiltFile : FileSizes)
	{
		if (FiltFile.second.size() > 1) // if two or more files have same size
		{
			for (auto file = FiltFile.second.begin(); file != FiltFile.second.end(); file++)
			{
				ifstream fileStream(file->absoluteFilePath().toStdString(), ios_base::binary);
				if (!fileStream.is_open()) continue;

				istreambuf_iterator<char> ii1(fileStream);
				istreambuf_iterator<char> end;

				for (auto fileComp = file; fileComp != FiltFile.second.end();)
				{
					if (*file == *fileComp)
					{
						fileComp++;
						continue;
					}
					
					if (unique && (file->absoluteDir() == fileComp->absoluteDir()))
					{
						fileComp++;
						continue;
					}

					ifstream fileCompStream(fileComp->absoluteFilePath().toStdString(), ios_base::binary);
					if (!fileCompStream.is_open())
					{
						fileComp++;
						continue;
					}
					istreambuf_iterator<char> ii2(fileCompStream);
					fileStream.seekg(0);

					if (range_equal(ii1, end, ii2, end))
					{
						equalFiles.push_back(*fileComp);
						fileComp = FiltFile.second.erase(fileComp);
					}
					else
					{
						fileComp++;
					}

					fileCompStream.close();
				}
				
				fileStream.close();

				if (equalFiles.size())
				{
					equalFiles.push_back(*file);
					res.push_back(equalFiles);
					equalFiles.clear();
				}
			}
		}
	}

	return res;
}
