#include "mcserverhelper.h"
#include <QTranslator>
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	QTranslator translator;
	translator.load(":/language/mcserverhelper_zh.qm");
	qApp->installTranslator(&translator);
	MCServerHelper w;
	w.show();
	return a.exec();
}
