#pragma once
#include <QFile>
#include <QDir>
#include <QString>
#include "Hash.h"

class File : public QFile
{
public:
	File(const QString& name, QObject* parent) :
		QFile(name, parent) {};
	File(QObject* parent) :
		QFile(parent) {};
	File(const QString& name) :
		QFile(name) {};

	File() = default;

	Hash getHash(QCryptographicHash::Algorithm);
};
