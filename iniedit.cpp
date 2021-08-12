#include "iniedit.h"

iniEdit::iniEdit(QObject* parent)
	: QObject(parent)
{
}

iniEdit::~iniEdit()
{
}

int iniEdit::ReadMyINI()
{
	string str_tmp;
	QFileInfo file(qApp->applicationDirPath() + "/MCSH.ini");
	if (file.exists() == false)
		WriteMyINI();
	ifstream fin(qApp->applicationDirPath().toStdString() + "/MCSH.ini");
	if (!fin)
	{
		return 1;
	}
	else
	{
		for (; !fin.eof(); fin >> str_tmp)
		{
			if ("Version" == str_tmp)
				fin >> str_tmp >> myini.version;
			else if ("isStartWithWindows" == str_tmp)
				fin >> str_tmp >> myini.isStartWithWindow;
			else if ("isStartServerOnceStarted" == str_tmp)
				fin >> str_tmp >> myini.isStartServerOnceStarted;
			else if ("isStartFrpWithServer" == str_tmp)
				fin >> str_tmp >> myini.isStartFrpWithServer;
			else if ("isOpenedWelcomeNode" == str_tmp)
				fin >> str_tmp >> myini.isOpenedWelcomeNode;
			else if ("Language" == str_tmp)
				fin >> str_tmp >> myini.Language;
			else if ("ServerName" == str_tmp)
				fin >> str_tmp >> myini.ServerName;
			else if ("Welcome1" == str_tmp)
				fin >> str_tmp >> myini.Welcome1;
			else if ("Welcome2" == str_tmp)
				fin >> str_tmp >> myini.Welcome2;
			else if ("JavaPath" == str_tmp)
				fin >> str_tmp >> myini.JavaPath;
			else if ("MinMemory" == str_tmp)
				fin >> str_tmp >> myini.MinMemory;
			else if ("MaxMemory" == str_tmp)
				fin >> str_tmp >> myini.MaxMemory;
			else if ("ServerPath" == str_tmp)
				fin >> str_tmp >> myini.ServerPath;
			else if ("FrpPath" == str_tmp)
				fin >> str_tmp >> myini.FrpPath;
			else if ("FrpINIPath" == str_tmp)
				fin >> str_tmp >> myini.FrpINIPath;
		}
		myini.isLasted = true;
	}
	fin.close();
	return 0;
}

string iniEdit::ReadMyINI(string sName)
{
	return ReadMyINI(QString::fromStdString(sName)).toStdString();
}

QString iniEdit::ReadMyINI(QString sName)
{
	if ("Version" == sName)
		return QString::number(myini.version);
	else if ("isStartWithWindows" == sName)
		return myini.isStartWithWindow;
	else if ("isStartServerOnceStarted" == sName)
		return myini.isStartServerOnceStarted;
	else if ("isStartFrpWithServer" == sName)
		return myini.isStartFrpWithServer;
	else if ("isOpenedWelcomeNode" == sName)
		return myini.isOpenedWelcomeNode;
	else if ("Language" == sName)
		return QString::fromStdString(myini.Language);
	else if ("ServerName" == sName)
		return QString::fromStdString(myini.ServerName);
	else if ("Welcome1" == sName)
		return QString::fromStdString(myini.Welcome1);
	else if ("Welcome2" == sName)
		return QString::fromStdString(myini.Welcome2);
	else if ("JavaPath" == sName)
		return QString::fromStdString(myini.JavaPath);
	else if ("MinMemory" == sName)
		return QString::number(myini.MinMemory);
	else if ("MaxMemory" == sName)
		return QString::number(myini.MaxMemory);
	else if ("ServerPath" == sName)
		return QString::fromStdString(myini.ServerPath);
	else if ("FrpPath" == sName)
		return QString::fromStdString(myini.FrpPath);
	else if ("FrpINIPath" == sName)
		return QString::fromStdString(myini.FrpINIPath);
	return "NULL";
}

int iniEdit::ReadServerINI()
{
	string buffer;
	ifstream fin(qApp->applicationDirPath().toStdString() + "/Server/server.properties");
	if (!fin)
	{
		return 1;
	}
	myini.ServerINI = "";
	while (!fin.eof())
	{
		getline(fin, buffer);
		buffer.append("\n");
		myini.ServerINI.append(buffer);
	}
	fin.close();
	return 0;
}

int iniEdit::ReadFrpINI()
{
	string buffer;
	ifstream fin(qApp->applicationDirPath().toStdString() + "/FRP/" + myini.FrpINIPath);
	if (!fin)
	{
		return 1;
	}
	myini.FrpINI = "";
	while (!fin.eof())
	{
		getline(fin, buffer);
		buffer.append("\n");
		myini.FrpINI.append(buffer);
	}
	fin.close();
	return 0;
}

int iniEdit::WriteMyINI()
{
	ofstream fout(qApp->applicationDirPath().toStdString() + "/MCSH.ini");
	if (!fout)
	{
		return 1;
	}
	fout << "Version = " << myini.version << endl;
	fout << "isStartWithWindows = " << myini.isStartWithWindow << endl;
	fout << "isStartServerOnceStarted = " << myini.isStartServerOnceStarted << endl;
	fout << "isStartFrpWithServer = " << myini.isStartFrpWithServer << endl;
	fout << "isOpenedWelcomeNode = " << myini.isOpenedWelcomeNode << endl;
	fout << "Language = " << myini.Language << endl;
	fout << "ServerName = " << myini.ServerName << endl;
	fout << "Welcome1 = " << myini.Welcome1 << endl;
	fout << "Welcome2 = " << myini.Welcome2 << endl;
	fout << "JavaPath = " << myini.JavaPath << endl;
	fout << "MinMemory = " << myini.MinMemory << endl;
	fout << "MaxMemory = " << myini.MaxMemory << endl;
	fout << "ServerPath = " << myini.ServerPath << endl;
	fout << "FrpPath = " << myini.FrpPath << endl;
	fout << "FrpINIPath = " << myini.FrpINIPath << endl;
	fout.close();
	return 0;
}

int iniEdit::WriteServerINI()
{
	ofstream fout(qApp->applicationDirPath().toStdString() + "/Server/server.properties");
	if (!fout)
	{
		return 1;
	}
	fout << myini.ServerINI;
	fout.close();
	return 0;
}

int iniEdit::WriteFrpINI()
{
	ofstream fout(qApp->applicationDirPath().toStdString() + "/FRP/" + myini.FrpINIPath);
	if (!fout)
	{
		return 1;
	}
	fout << myini.FrpINI;
	fout.close();
	return 0;
}
