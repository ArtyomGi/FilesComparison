#include "File.h"

Hash File::getHash(QCryptographicHash::Algorithm method)
{
	QCryptographicHash hash(method);
	hash.addData(this);
	return hash.result();
}