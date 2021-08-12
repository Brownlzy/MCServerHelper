#pragma once

#include <QObject>
#include <QDebug>
#include <QApplication>
#include <QFileInfo>
#include <iostream>
#include <fstream>
#include <string>
#include "appinfo.h"
using namespace std;

class iniEdit : public QObject
{
	Q_OBJECT

public:
	iniEdit(QObject* parent);
	~iniEdit();
	int ReadMyINI();
	string ReadMyINI(string sName);
	QString ReadMyINI(QString sName);
	int ReadServerINI();
	int ReadFrpINI();
	int WriteMyINI();
	int WriteServerINI();
	int WriteFrpINI();

	struct MyINI
	{
		bool isLasted = false;
		int version = VERSIONID;
		bool isStartWithWindow = false;
		bool isStartServerOnceStarted = false;
		bool isStartFrpWithServer = false;
		bool isOpenedWelcomeNode = true;
		string Language = "en_us";
		string JavaPath = "java";
		string ServerName = "MCSH_Server";
		string Welcome1 = "Default";
		string Welcome2 = "Default";
		int MinMemory = 1024;
		int MaxMemory = 1024;
		string ServerPath = "NULL";
		string FrpPath = "NULL";
		string FrpINIPath = "NULL";
		string ServerINI = "";
		string FrpINI = "";
	}myini;
};
