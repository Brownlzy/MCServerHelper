#pragma once

#include <QMainWindow>
#include "ui_mcserverhelper.h"
#include "iniedit.h"
#include "appinfo.h"
#include "update.h"
#include <QProcess>
#include <QMessageBox>
#include <QTime>
#include <QTimer>
#include <QPropertyAnimation>
#include <QTranslator>

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
	void Donate();

private:
	Ui::MCServerHelperClass ui;
	iniEdit* ie = nullptr;
	QProcess* m_server = nullptr;
	QProcess* m_frp = nullptr;
	QPropertyAnimation* pUpdateFrm = nullptr;
	Update* pUpdate = nullptr;

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
	void tabChanged(int tabid);
	void ShowUpdateInfo();
	void btnDoUpdate();
	void ChangeLanguage();
};
