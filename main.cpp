﻿#include "mcserverhelper.h"
#include <QTranslator>
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	QTranslator translator;
	QFileInfo file(qApp->applicationDirPath() + "/update.bat");
	if (file.exists() == true)
	{
		QProcess::startDetached(qApp->applicationDirPath() + "/update.bat");
		qApp->exit(0);
	}
	else
	{
		translator.load(":/language/mcserverhelper_zh.qm");
		qApp->installTranslator(&translator);
		MCServerHelper w;
		w.show();
		return a.exec();
	}
}
