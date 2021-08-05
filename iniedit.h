#pragma once

#include <QObject>
#include <QDebug>
#include <QApplication>
#include <QFileInfo>
#include <iostream>
#include <fstream>
#include <string>
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
		int version = 0;
		bool isStartWithWindow = false;
		bool isStartServerOnceStarted = false;
		bool isStartFrpWithServer = false;
		string JavaPath = "java";
		int MinMemory = 1024;
		int MaxMemory = 1024;
		string ServerPath = "NULL";
		string FrpPath = "NULL";
		string FrpINIPath = "NULL";
		string ServerINI = "";
		string FrpINI = "";
	}myini;
};
