#pragma once
#pragma execution_character_set("utf-8")

#include <QMainWindow>
#include "ui_mcserverhelper.h"
#include "iniedit.h"
#include "appinfo.h"
#include "update.h"
#include "playerinfo.h"
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
	void PlayerInfo2Table();
	void PlayerInfo4Table();
	void PlayerLogined(QString uName, QString uUUID, QString uIP);
	void PlayerLosted(QString uName, QString reason);
	void WelcomeNote(int index, QString uIP, int isFirst = 0);

private:
	Ui::MCServerHelperClass ui;
	iniEdit* ie = nullptr;
	QProcess* m_server = nullptr;
	QProcess* m_frp = nullptr;
	QPropertyAnimation* pUpdateFrm = nullptr;
	Update* pUpdate = nullptr;
	PlayerInfo* pi = nullptr;
	QString LoginTmp = "";

public slots:
	void onServerOutput();
	void onFrpOutput();
	void ServerStart();
	void ServerInput();
	void ServerStop();
	void FrpStart();
	void FrpStop();
	void MCSHConfirm();
	void MCSHCancel();
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
	void MenuCommand();
};
