#include "mcserverhelper.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	QFileInfo file(qApp->applicationDirPath() + "/update.bat");
	if (file.exists() == true)
	{
		QProcess::startDetached(qApp->applicationDirPath() + "/update.bat");
		qApp->exit(0);
	}
	else
	{
		MCServerHelper w;
		w.show();
		return a.exec();
	}
}
