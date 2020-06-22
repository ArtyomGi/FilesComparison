#include "FilesComp.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	FilesComp w;
	w.show();

	return a.exec();
}
