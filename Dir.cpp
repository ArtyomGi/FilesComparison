#include "Dir.h"
#include "File.h"
#include "Hash.h"

vector<vector<QFileInfo>> Dir::fileIntersection(const Dir& anotherDir, bool unique, QCryptographicHash::Algorithm alg) const
{
	if (!exists() || !anotherDir.exists()) return vector<vector<QFileInfo>>();

	QFileInfoList files = entryInfoList(QDir::Files);

	qint64 size;
	map<qint64, vector<QFileInfo>> FileSizes;

	for (const auto& f : files) // map of sizes of files not to calculate hash for every file in dir
	{
		size = f.size();

		FileSizes.emplace(make_pair(size, vector<QFileInfo>()));
		FileSizes[size].push_back(f);
	}

	files = anotherDir.entryInfoList(QDir::Files);

	for (const auto& f : files)
	{
		size = f.size();

		FileSizes.emplace(make_pair(size, vector<QFileInfo>()));
		FileSizes[size].push_back(f);
	}

	map<Hash, vector<QFileInfo>> Hashes;
	File temp;

	for (const auto& FiltFile : FileSizes)
	{
		if (FiltFile.second.size() > 1) // if two or more files have same size
		{
			for (const auto& file : FiltFile.second)
			{
				temp.setFileName(file.absoluteFilePath());
				temp.open(QIODevice::ReadOnly);

				Hash h = temp.getHash(alg); // calculate hash for that files

				Hashes.emplace(make_pair(h, vector<QFileInfo>()));
				Hashes[h].push_back(file);

				temp.close();
			}
		}
	}

	vector<QFileInfo> intersFiles;
	vector<vector<QFileInfo>> result;
	bool check = false;
	QString strCheck;

	for (const auto& hash : Hashes)
	{
		if (hash.second.size() > 1) // if two or more files have same hash
		{
			strCheck = hash.second[0].absolutePath();
			for (const auto& file : hash.second)
			{
				intersFiles.push_back(file); // generating a result
				if (unique && !check && strCheck != file.absolutePath()) check = true;
			}

			if ((unique && check) || !unique || (absolutePath() == anotherDir.absolutePath()))
				result.push_back(intersFiles);

			intersFiles.clear();
			check = false;
		}
	}

	return result;
}
