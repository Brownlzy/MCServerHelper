#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mcserverhelper.h"
#include "iniedit.h"
#include <QProcess>
#include <QMessageBox>
#include <QTime>
#include <QTimer>


class MCServerHelper : public QMainWindow
{
	Q_OBJECT

public:
	MCServerHelper(QWidget* parent = Q_NULLPTR);
	~MCServerHelper();
	void UpdateSettings();
	void startServer();
	void startFrp();
	QString ChangeTextColorS(QString oText);
	QString ChangeTextColorF(QString oText);

private:
	Ui::MCServerHelperClass ui;
	iniEdit* ie;
	QProcess* m_server;
	QProcess* m_frp;

public slots:
	void onServerOutput();
	void onFrpOutput();
	void ServerStart();
	void ServerInput();
	void ServerStop();
	void FrpStart();
	void FrpStop();
	void MSCHConfirm();
	void MSCHCancel();
	void ServerINIConfirm();
	void ServerINICancel();
	void FrpConfirm();
	void FrpCancel();
	void closeEvent(QCloseEvent* event);
	void timeTik();
};
