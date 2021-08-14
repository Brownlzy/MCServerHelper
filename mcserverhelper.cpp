#include "mcserverhelper.h"

MCServerHelper::MCServerHelper(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ie = new iniEdit(this);
	pi = new PlayerInfo(this);
	UpdateSettings();
	pi->ReadPlayerInfo();
	//pi->WritePlayerInfo();
	PlayerInfo2Table();
	QTranslator translator;
	QTranslator translator2;
	if (ie->ReadMyINI(QString("Language")) == "zh_cn")
	{
		translator.load(":/language/mcserverhelper_zh.qm");
		translator2.load(":/language/qt_zh_CN.qm");
		ui.radChinese->setChecked(true);
	}
	else
	{
		translator.load(":/language/mcserverhelper_en.qm");
		translator2.load(":/language/qt_en.qm");
	}
	qApp->installTranslator(&translator);
	ui.retranslateUi(this);
	UpdateSettings();
	ui.menuBar->hide();
	ui.frmUpdateInfo->raise();
	ui.frmUpdateInfo->hide();
	this->setFixedSize(this->width(), this->height());
#ifdef _DEBUG
	this->setWindowTitle(QString("MCServerHelper v") + VERSION + "_DEBUG By:Brownlzy");
#else
	ui.labDebug->hide();
	this->setWindowTitle(QString("MCServerHelper v") + VERSION + " By:Brownlzy");
#endif
	m_server = new QProcess(this);
	m_frp = new QProcess(this);
	pUpdateFrm = new QPropertyAnimation(ui.frmUpdateInfo, "pos", this);
	pUpdate = new Update(this, ui.DownBar, ui.labAbout, ui.txtUpdateInfo);
	m_server->setWorkingDirectory(qApp->applicationDirPath() + "/Server");
	m_server->setProcessChannelMode(QProcess::MergedChannels);
	m_frp->setWorkingDirectory(qApp->applicationDirPath() + "/FRP");
	m_frp->setProcessChannelMode(QProcess::MergedChannels);
	ui.btnServerStop->setEnabled(false);
	ui.btnFrpStop->setEnabled(false);
	ui.btnServerInput->setEnabled(false);
	ui.btnDoUpdate->setEnabled(false);
	ui.labAbout->setText(QString(tr("Build Time: ")) + __TIMESTAMP__);
	ui.frmUpdateInfo->move(0, 130);
	connect(m_server, SIGNAL(readyReadStandardOutput()), this, SLOT(onServerOutput()));
	connect(m_frp, SIGNAL(readyReadStandardOutput()), this, SLOT(onFrpOutput()));
	connect(ui.btnServerStart, SIGNAL(clicked()), this, SLOT(ServerStart()));
	connect(ui.btnFrpStart, SIGNAL(clicked()), this, SLOT(FrpStart()));
	connect(ui.btnServerInput, SIGNAL(clicked()), this, SLOT(ServerInput()));
	connect(ui.btnServerStop, SIGNAL(clicked()), this, SLOT(ServerStop()));
	connect(ui.btnFrpStop, SIGNAL(clicked()), this, SLOT(FrpStop()));
	connect(ui.btnMCSHConfirm, SIGNAL(clicked()), this, SLOT(MCSHConfirm()));
	connect(ui.btnServerSConfirm, SIGNAL(clicked()), this, SLOT(ServerINIConfirm()));
	connect(ui.btnFrpSConfirm, SIGNAL(clicked()), this, SLOT(FrpConfirm()));
	connect(ui.btnMCSHCancel, SIGNAL(clicked()), this, SLOT(MCSHCancel()));
	connect(ui.btnServerSCancel, SIGNAL(clicked()), this, SLOT(ServerINICancel()));
	connect(ui.btnFrpSCancel, SIGNAL(clicked()), this, SLOT(FrpCancel()));
	connect(ui.tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
	connect(ui.btnShowUpdateInfo, SIGNAL(clicked()), this, SLOT(ShowUpdateInfo()));
	connect(ui.btnDoUpdate, SIGNAL(clicked()), this, SLOT(btnDoUpdate()));
	connect(ui.btnChangeLanguage, SIGNAL(clicked()), this, SLOT(ChangeLanguage()));
	connect(ui.btnCommand, SIGNAL(clicked()), this, SLOT(MenuCommand()));
	connect(ui.btnServerRestart, SIGNAL(clicked()), this, SLOT(RestartServer()));
	connect(ui.btnServerRestart_2, SIGNAL(clicked()), this, SLOT(RestartServer()));
	connect(ui.btnFrpRestart, SIGNAL(clicked()), this, SLOT(RestartFrp()));
	connect(ui.btnRestart, SIGNAL(clicked()), this, SLOT(RestartBoth()));
	connect(ui.btnRestart_2, SIGNAL(clicked()), this, SLOT(RestartBoth()));
	connect(ui.btnRestart, SIGNAL(clicked()), this, SLOT(StopBoth()));
	connect(ui.btnRestart_2, SIGNAL(clicked()), this, SLOT(StopBoth()));
	connect(ui.btnPlayerRefresh, SIGNAL(clicked()), this, SLOT(RefreshPlayerList()));
	timeTik();
	//connect(ui.PathJava, SIGNAL(textChanged(QString)), this, SLOT(MCSHTextChanged(QString)));
	//connect(ui.PathServer, SIGNAL(textChanged(QString)), this, SLOT(MCSHTextChanged(QString)));
	//connect(ui.PathFrp, SIGNAL(textChanged(QString)), this, SLOT(MCSHTextChanged(QString)));
	//connect(ui.PathFrpIni, SIGNAL(textChanged(QString)), this, SLOT(MCSHTextChanged(QString)));

}

MCServerHelper::~MCServerHelper()
{
	//m_server->terminate();
	//ServerStop();
	m_frp->terminate();
	m_frp->kill();
}

void MCServerHelper::onServerOutput()
{
	QByteArray qbt = m_server->readAllStandardOutput();
	QString msg = QString::fromLocal8Bit(qbt);
	if (msg.contains("[Server thread/INFO]: Done"))
	{
		ui.btnServerStop->setEnabled(true);
		ui.btnServerInput->setEnabled(true);
	}
	else if (msg.contains("[Server thread/INFO]: ThreadedAnvilChunkStorage: All dimensions are saved"))
	{
		ui.btnServerStart->setEnabled(true);
	}
	//ui.OutServer->append(msg);
	ui.OutServer->append(ChangeTextColorS(msg));
	ui.OutServer->update();
}

void MCServerHelper::UpdateSettings()
{
	ie->ReadMyINI();
	ie->ReadFrpINI();
	ie->ReadServerINI();
	ui.chkStartWithWindows->setChecked(ie->myini.isStartWithWindow);
	ui.chkStartServerOnce->setChecked(ie->myini.isStartServerOnceStarted);
	ui.chkStartFrpOnce->setChecked(ie->myini.isStartFrpWithServer);
	ui.chkWelcomNote->setChecked(ie->myini.isOpenedWelcomeNode);
	ui.ServerName->setText(QString::fromStdString(ie->myini.ServerName).replace("&nbsp;", " "));
	ui.PathJava->setText(QString::fromStdString(ie->myini.JavaPath).replace("&nbsp;", " "));
	ui.WelcomeNote1->setPlainText(QString::fromStdString(ie->myini.Welcome1).replace("&nbsp;", " "));
	ui.WelcomeNote2->setPlainText(QString::fromStdString(ie->myini.Welcome2).replace("&nbsp;", " "));
	ui.MinRam->setText(QString::number(ie->myini.MinMemory));
	ui.MaxRam->setText(QString::number(ie->myini.MaxMemory));
	ui.PathServer->setText(QString::fromStdString(ie->myini.ServerPath).replace("&nbsp;", " "));
	ui.PathServer2->setText(QString::fromStdString(ie->myini.ServerPath).replace("&nbsp;", " "));
	ui.PathFrp->setText(QString::fromStdString(ie->myini.FrpPath).replace("&nbsp;", " "));
	ui.PathFrp2->setText(QString::fromStdString(ie->myini.FrpPath).replace("&nbsp;", " "));
	ui.PathFrpIni->setText(QString::fromStdString(ie->myini.FrpINIPath).replace("&nbsp;", " "));
	ui.PathFrpIni2->setText(QString::fromStdString(ie->myini.FrpINIPath).replace("&nbsp;", " "));
	ui.INIFrp->setPlainText(QString::fromStdString(ie->myini.FrpINI));
	ui.INIServer->setPlainText(QString::fromStdString(ie->myini.ServerINI));
}

void MCServerHelper::startServer()
{
	QFileInfo file(qApp->applicationDirPath() + "/Server/" + QString::fromStdString(ie->myini.ServerPath).replace("&nbsp;", " "));
	if (file.exists() == false)
	{
		ui.OutServer->append("<span style=\"color:red;\">未找到文件:\"" + qApp->applicationDirPath() + "/Server/" + QString::fromStdString(ie->myini.ServerPath).replace("&nbsp;", " ") + "\"</span>");
		ui.OutServer->update();
		ui.btnServerStart->setEnabled(true);
		ui.btnServerStop->setEnabled(false);
		return;
	}
	QStringList Par;
	Par << "-Xmx" + QString::number(ie->myini.MaxMemory) + "M" << "-Xms" + QString::number(ie->myini.MinMemory) + "M" << "-jar" << QString::fromStdString(ie->myini.ServerPath) << "nogui";
	ui.OutServer->append(">\"" + QString::fromStdString(ie->myini.JavaPath).replace("&nbsp;", " ") + "\" -Xmx" + QString::number(ie->myini.MaxMemory) + "M " + "-Xms" + QString::number(ie->myini.MinMemory) + "M -jar " + QString::fromStdString(ie->myini.ServerPath) + " nogui");
	ui.OutServer->update();
	m_server->start(QString::fromStdString(ie->myini.JavaPath).replace("&nbsp;", " "), Par);
	//java -Xmx1024M -Xms1024M -jar mc_server_1.17.1.jar nogui
	//Par << "-Xmx1024M" << "-Xms1024M" << "-jar" << "mc_server_1.17.1.jar" << "nogui";
	//QString qsCmd = "ping localhost";
	//qsCmd += QString::fromStdString(ie->myini.JavaPath + " -Xmx") + QString::number(ie->myini.MaxMemory) + QString::fromStdString("M -Xms") + QString::number(ie->myini.MinMemory) + QString::fromStdString("M -jar " + ie->myini.ServerPath + " nogui");
	//m_server->start("D:/java-runtime-alpha/bin/java.exe", Par);
}

void MCServerHelper::startFrp()
{
	QFileInfo file(qApp->applicationDirPath() + "/FRP/" + QString::fromStdString(ie->myini.FrpPath).replace("&nbsp;", " "));
	if (file.exists() == false)
	{
		ui.OutFrp->append("<span style=\"color:red;\">未找到文件:\"" + qApp->applicationDirPath() + "/FRP/" + QString::fromStdString(ie->myini.FrpPath).replace("&nbsp;", " ") + "\"</span>");
		ui.OutFrp->update();
		ui.btnFrpStart->setEnabled(true);
		ui.btnFrpStop->setEnabled(false);
		return;
	}
	QStringList Par;
	Par << "-c" << QString::fromStdString(ie->myini.FrpINIPath).replace("&nbsp;", " ");
	ui.OutFrp->append(">" + qApp->applicationDirPath() + "/FRP/" + QString::fromStdString(ie->myini.FrpPath).replace("&nbsp;", " ") + " -c " + QString::fromStdString(ie->myini.FrpINIPath));
	ui.OutFrp->update();
	m_frp->start(qApp->applicationDirPath() + "/FRP/" + QString::fromStdString(ie->myini.FrpPath).replace("&nbsp;", " "), Par);
}

QString MCServerHelper::ChangeTextColorS(QString oText)
{
	oText.replace("<", "&lt;");
	oText.replace(">", "&gt;");
	oText.replace("\n", "<br/>");
	//oText.replace(" ", "&nbsp;");
	if (oText.endsWith("<br/>"))
		oText = oText.left(oText.length() - 5);
	QString colorText = "";
	for (int i = 0; i <= oText.contains("<br/>"); i++)
	{
		QString forStr = oText.split("<br/>")[i];
		colorText += "<span style=\"color:";
		if (forStr.contains("ERROR") || forStr.contains("错误"))
			colorText += "red;\">";
		else if (forStr.contains("[") && forStr.contains("]") && forStr.split("[")[2].split("]")[0].contains("INFO"))
		{
			if (forStr.contains("[User Authenticator #") && forStr.contains("UUID of player"))
			{
				LoginTmp = forStr.split("/INFO]: UUID of player ")[1].split(" is ")[0] + ":" + forStr.split("/INFO]: UUID of player ")[1].split(" is ")[1].split("\r")[0];
				colorText += "#087CFA;\">";
			}
			else if (!forStr.contains("&lt;") && forStr.contains("logged in with entity id"))
			{
				colorText += "#087CFA;\">";
				if (forStr.contains(LoginTmp.split(":")[0]))
					PlayerLogined(LoginTmp.split(":")[0], LoginTmp.split(":")[1], forStr.split(LoginTmp.split(":")[0])[1].split(" ")[0]);
			}
			else if (!forStr.contains("&lt;") && forStr.contains("joined the game"))
				colorText += "#087CFA;\">";
			else if (!forStr.contains("&lt;") && forStr.contains("lost connection: "))
			{
				colorText += "#6A3C97;\">";
				PlayerLosted(forStr.split("[Server thread/INFO]: ")[1].split(" ")[0], forStr.split("lost connection: ")[1]);
			}
			else if (!forStr.contains("&lt;") && forStr.contains(" left the game"))
				colorText += "#6A3C97;\">";
			else
				colorText += "green;\">";
		}
		else if (forStr.contains("[") && forStr.contains("]") && forStr.split("[")[2].split("]")[0].contains("WARN"))
			colorText += "#ff7e00;\">";
		else if (forStr.contains("[") && forStr.contains("]") && forStr.split("[")[2].split("]")[0].contains("FATAL"))
			colorText += "red;\">";
		else
			colorText += "red;\">";
		colorText += oText.split("<br/>")[i] + "</span>";
		if (i != oText.contains("<br/>")) colorText += "<br/>";
	}
	return colorText;
}

QString MCServerHelper::ChangeTextColorF(QString oText)
{
	oText.replace("<", "&lt;");
	oText.replace(">", "&gt;");
	oText.replace("\n", "<br/>");
	//oText.replace(" ", "&nbsp;");
	if (oText.endsWith("<br/>"))
		oText = oText.left(oText.length() - 5);
	QString colorText = "";
	for (int i = 0; i <= oText.contains("<br/>"); i++)
	{
		colorText += "<span style=\"color:";
		if (oText.contains("[") && oText.contains("]") && (oText.split("<br/>")[i].split("]")[0].split("[")[1] == "E" || oText.split("<br/>")[i].split("]")[0].split("[")[1] == "F"))
			colorText += "red;\">";
		else if (oText.contains("[") && oText.contains("]") && oText.split("<br/>")[i].split("]")[0].split("[")[1] == "I")
			colorText += "green;\">";
		else if (oText.contains("[") && oText.contains("]") && oText.split("<br/>")[i].split("]")[0].split("[")[1] == "W")
			colorText += "#ff7e00;\">";
		else
			colorText += "red;\">";
		colorText += oText.split("<br/>")[i] + "</span>";
		if (i != oText.contains("<br/>")) colorText += "<br/>";
	}
	return colorText;
}

void MCServerHelper::Donate()
{
	QLabel* plab = new QLabel("<html><head/><body><p><img src="":/pic/png/pay"" widyh=""191"" height=""191""/></p></body></html>");
	plab->setFixedSize(191, 191);
	plab->setWindowIcon(QIcon(":/pic/icon/DinoOL"));
	plab->setToolTip(tr("Note: sponsorship is free and voluntary"));
	plab->setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint);
	plab->show();
}

void MCServerHelper::PlayerInfo2Table()
{
	ui.PlayerList->setRowCount(pi->numPlayer);//把行数设置为用户数
	for (int i = 0; i < pi->numPlayer; i++)
	{
		ui.PlayerList->setItem(i, 0, new QTableWidgetItem(pi->pPlayer[i].ID));
		ui.PlayerList->setItem(i, 1, new QTableWidgetItem(pi->pPlayer[i].UUID));
		ui.PlayerList->setItem(i, 2, new QTableWidgetItem(pi->pPlayer[i].LastLogin));
		ui.PlayerList->setItem(i, 3, new QTableWidgetItem(pi->pPlayer[i].LastLost));
		ui.PlayerList->setItem(i, 4, new QTableWidgetItem(pi->pPlayer[i].OPLevel ? "OPLevel:" + QString::number(pi->pPlayer[i].OPLevel) : "x"));
		ui.PlayerList->setItem(i, 5, new QTableWidgetItem(pi->pPlayer[i].isWhitelist ? "Whited" : "x"));
		ui.PlayerList->setItem(i, 6, new QTableWidgetItem(pi->pPlayer[i].LastIP));
		ui.PlayerList->setItem(i, 7, new QTableWidgetItem(pi->pPlayer[i].isBanned ? "Banned" : "x"));
		if (pi->pPlayer[i].isBanned)ui.PlayerList->item(i, 0)->setTextColor(QColor("red"));
	}
}

void MCServerHelper::PlayerInfo4Table()
{
	delete[] pi->pPlayer2;
	pi->pPlayer2 = new Player[ui.PlayerList->rowCount()];
	for (int i = 0; i < ui.PlayerList->rowCount(); i++)
	{
		pi->pPlayer2[i].ID = ui.PlayerList->item(i, 0)->text();
		pi->pPlayer2[i].UUID = ui.PlayerList->item(i, 1)->text();
		if (pi->getPlayerInfoIndex(pi->pPlayer2[i].ID, pi->pPlayer2[i].UUID) < 0 || pi->pPlayer[pi->getPlayerInfoIndex(pi->pPlayer2[i].ID, pi->pPlayer2[i].UUID)].FirstLogin == "")
			pi->pPlayer2[i].FirstLogin = ui.PlayerList->item(i, 2)->text();
		else
			pi->pPlayer2[i].FirstLogin = pi->pPlayer[pi->getPlayerInfoIndex(pi->pPlayer2[i].ID, pi->pPlayer2[i].UUID)].FirstLogin;
		pi->pPlayer2[i].Times = pi->pPlayer[pi->getPlayerInfoIndex(pi->pPlayer2[i].ID, pi->pPlayer2[i].UUID)].Times;
		pi->pPlayer2[i].LastIP = ui.PlayerList->item(i, 6)->text();
		pi->pPlayer2[i].LastLogin = ui.PlayerList->item(i, 2)->text();
		pi->pPlayer2[i].LastLost = ui.PlayerList->item(i, 3)->text();
	}
	pi->storePlayerHistory(pi->pPlayer2, ui.PlayerList->rowCount());
	pi->WriteJson<PlayerHistory>(ui.PlayerList->rowCount(), pi->pPlayerHistory, qApp->applicationDirPath().toStdString() + "/Server/player-history.json");
}

void MCServerHelper::PlayerLogined(QString uName, QString uUUID, QString uIP)
{
	int flag = 0, index = 0;
	char* cmd;
	QByteArray tmp;
	for (int i = 0; i < ui.PlayerList->rowCount() && flag == 0; i++)
	{
		if (ui.PlayerList->item(i, 0)->text() == uName && ui.PlayerList->item(i, 1)->text() == uUUID)
		{
			ui.PlayerList->item(i, 0)->setTextColor(QColor("green"));
			ui.PlayerList->item(i, 6)->setText(uIP);
			ui.PlayerList->item(i, 2)->setText(ui.labSysTime->text());
			int id = pi->getPlayerInfoIndex(uName, uUUID);
			pi->pPlayer[id].Times++;
			index = i;
			tmp = ("/title " + ui.PlayerList->item(index, 0)->text() + " title \"Welcome " + ui.PlayerList->item(index, 0)->text() + "!\" \n").toLocal8Bit();
			cmd = tmp.data();
			m_server->write(cmd);
			tmp = ("/title " + ui.PlayerList->item(index, 0)->text() + " subtitle \"" + ui.ServerName->text() + "\" \n").toLocal8Bit();
			cmd = tmp.data();
			m_server->write(cmd);
			if (ui.chkWelcomNote->isChecked()) WelcomeNote(i, uIP);
			pi->pPlayer[id].LastIP = uIP;
			pi->pPlayer->LastLogin = ui.PlayerList->item(i, 2)->text();
			flag = 1;
		}
	}
	if (!flag)
	{
		ui.PlayerList->setRowCount(ui.PlayerList->rowCount() + 1);//把行数设置为用户数
		ui.PlayerList->setItem(ui.PlayerList->rowCount() - 1, 0, new QTableWidgetItem(uName));
		ui.PlayerList->setItem(ui.PlayerList->rowCount() - 1, 1, new QTableWidgetItem(uUUID));
		ui.PlayerList->setItem(ui.PlayerList->rowCount() - 1, 2, new QTableWidgetItem(ui.labSysTime->text()));
		ui.PlayerList->setItem(ui.PlayerList->rowCount() - 1, 3, new QTableWidgetItem("-"));
		ui.PlayerList->setItem(ui.PlayerList->rowCount() - 1, 4, new QTableWidgetItem("x"));
		ui.PlayerList->setItem(ui.PlayerList->rowCount() - 1, 5, new QTableWidgetItem("x"));
		ui.PlayerList->setItem(ui.PlayerList->rowCount() - 1, 6, new QTableWidgetItem(uIP));
		ui.PlayerList->setItem(ui.PlayerList->rowCount() - 1, 7, new QTableWidgetItem("x"));
		PlayerInfo4Table();
		pi->ReadPlayerInfo();
		index = ui.PlayerList->rowCount() - 1;
		tmp = ("/title " + ui.PlayerList->item(index, 0)->text() + " title \"Welcome " + ui.PlayerList->item(index, 0)->text() + "!\" \n").toLocal8Bit();
		cmd = tmp.data();
		m_server->write(cmd);
		tmp = ("/title " + ui.PlayerList->item(index, 0)->text() + " subtitle \"" + ui.ServerName->text() + "\" \n").toLocal8Bit();
		cmd = tmp.data();
		m_server->write(cmd);
		if (ui.chkWelcomNote->isChecked()) WelcomeNote(ui.PlayerList->rowCount() - 1, uIP, 1);
	}

}

void MCServerHelper::PlayerLosted(QString uName, QString reason)
{
	for (int i = 0; i < ui.PlayerList->rowCount(); i++)
	{
		if (ui.PlayerList->item(i, 0)->text() == uName)
		{
			ui.PlayerList->item(i, 0)->setTextColor(QColor("black"));
			ui.PlayerList->item(i, 3)->setText(ui.labSysTime->text());
			int id = pi->getPlayerInfoIndex(uName);
			pi->pPlayer[id].LastLost = ui.PlayerList->item(i, 3)->text();
		}
	}
}

void MCServerHelper::WelcomeNote(int index, QString uIP, int isFirst)
{
	char* cmd;
	QByteArray tmp = ("/w " + ui.PlayerList->item(index, 0)->text() + " \"[WelcomeNote]\"\n").toLocal8Bit();
	int id = pi->getPlayerInfoIndex(ui.PlayerList->item(index, 0)->text(), ui.PlayerList->item(index, 1)->text());
	if (id < 0) return;
	if (isFirst)
	{
		tmp.replace("[WelcomeNote]", ui.WelcomeNote1->toPlainText().toLocal8Bit());
	}
	else
	{
		tmp.replace("[WelcomeNote]", ui.WelcomeNote2->toPlainText().toLocal8Bit());
	}
	tmp.replace("%Times%", QString::number(pi->pPlayer[id].Times).toLocal8Bit());
	tmp.replace("%LastLogin%", pi->pPlayer[id].LastLogin.toLocal8Bit());
	tmp.replace("%LastIP%", pi->pPlayer[id].LastIP.toLocal8Bit());
	tmp.replace("%ServerName%", ui.ServerName->text().toLocal8Bit());
	tmp.replace("%LastLost%", pi->pPlayer[id].LastLost.toLocal8Bit());
	tmp.replace("%PlayerName%", pi->pPlayer[id].ID.toLocal8Bit());
	tmp.replace("%ThisIP%", uIP.toLocal8Bit());
	cmd = tmp.data();
	m_server->write(cmd);
	tmp = ("/title " + ui.PlayerList->item(index, 0)->text() + " title \"Welcome " + ui.PlayerList->item(index, 0)->text() + "!\" \n").toLocal8Bit();
	cmd = tmp.data();
	m_server->write(cmd);
	tmp = ("/title " + ui.PlayerList->item(index, 0)->text() + " subtitle \"" + ui.ServerName->text() + "\" \n").toLocal8Bit();
	cmd = tmp.data();
	m_server->write(cmd);
}

void MCServerHelper::onFrpOutput()
{
	QByteArray qbt = m_frp->readAllStandardOutput();
	QString msg = QString::fromLocal8Bit(qbt);
	ui.OutFrp->append(ChangeTextColorF(msg));
	ui.OutFrp->update();
}

void MCServerHelper::ServerStart()
{
	ui.btnServerStart->setEnabled(false);
	qDebug() << "ServerStart";
	startServer();
}

void MCServerHelper::ServerInput()
{
	char* cmd;
	QByteArray tmp = ui.ServerInput->text().toLocal8Bit() + "\n";
	cmd = tmp.data();
	qDebug() << cmd;
	ui.OutServer->append(">" + ui.ServerInput->text());
	ui.OutServer->update();
	ui.ServerInput->setText("");
	m_server->write(cmd);
}

void MCServerHelper::ServerStop()
{
	ui.btnServerStop->setEnabled(false);
	ui.btnServerInput->setEnabled(false);
	qDebug() << "ServerStop";
	ui.OutServer->append(">stop");
	ui.OutServer->update();
	m_server->write("stop\n");
	PlayerInfo4Table();
}

void MCServerHelper::FrpStart()
{
	qDebug() << "FrpStart";
	ui.btnFrpStart->setEnabled(false);
	ui.btnFrpStop->setEnabled(true);
	startFrp();
}

void MCServerHelper::FrpStop()
{
	m_frp->terminate();
	m_frp->kill();
	ui.btnFrpStart->setEnabled(true);
	ui.btnFrpStop->setEnabled(false);
	ui.OutFrp->append(">stop");
	ui.OutFrp->update();
}

void MCServerHelper::MCSHConfirm()
{
	if (ui.PathJava->text() == "") ui.PathJava->setText("NULL");
	if (ui.PathServer->text() == "") ui.PathServer->setText("NULL");
	if (ui.PathFrp->text() == "") ui.PathFrp->setText("NULL");
	if (ui.PathFrpIni->text() == "") ui.PathFrpIni->setText("NULL");
	if (ui.ServerName->text() == "") ui.ServerName->setText("NULL");
	if (ui.WelcomeNote1->toPlainText() == "") ui.WelcomeNote1->setPlainText("NULL");
	if (ui.WelcomeNote2->toPlainText() == "") ui.WelcomeNote2->setPlainText("NULL");
	ie->myini.isStartWithWindow = ui.chkStartWithWindows->isChecked();
	ie->myini.isStartServerOnceStarted = ui.chkStartServerOnce->isChecked();
	ie->myini.isStartFrpWithServer = ui.chkStartFrpOnce->isChecked();
	ie->myini.isOpenedWelcomeNode = ui.chkWelcomNote->isChecked();
	if (ui.radChinese->isChecked())
		ie->myini.Language = "zh_cn";
	else
		ie->myini.Language = "en_us";
	ie->myini.JavaPath = ui.PathJava->text().replace(" ", "&nbsp;").toStdString();
	ie->myini.ServerName = ui.ServerName->text().replace(" ", "&nbsp;").toStdString();
	ie->myini.Welcome1 = ui.WelcomeNote1->toPlainText().replace("\n", " ").replace(" ", "&nbsp;").toStdString();
	ie->myini.Welcome2 = ui.WelcomeNote2->toPlainText().replace("\n", " ").replace(" ", "&nbsp;").toStdString();
	ie->myini.MinMemory = ui.MinRam->text().toInt();
	ie->myini.MaxMemory = ui.MaxRam->text().toInt();
	ie->myini.ServerPath = ui.PathServer->text().replace(" ", "&nbsp;").toStdString();
	ie->myini.FrpPath = ui.PathFrp->text().replace(" ", "&nbsp;").toStdString();
	ie->myini.FrpINIPath = ui.PathFrpIni->text().replace(" ", "&nbsp;").toStdString();
	ie->WriteMyINI();
	ChangeLanguage();
	UpdateSettings();
}

void MCServerHelper::MCSHCancel()
{
	UpdateSettings();
}

void MCServerHelper::ServerINIConfirm()
{
	ie->myini.ServerINI = ui.INIServer->toPlainText().toStdString();
	ie->WriteServerINI();
	MCSHConfirm();
}

void MCServerHelper::ServerINICancel()
{
	ui.INIServer->setPlainText(QString::fromStdString(ie->myini.ServerINI));
}

void MCServerHelper::FrpConfirm()
{
	ie->myini.FrpINI = ui.INIFrp->toPlainText().toStdString();
	ie->WriteFrpINI();
}

void MCServerHelper::FrpCancel()
{
	ui.INIFrp->setPlainText(QString::fromStdString(ie->myini.FrpINI));
}

void MCServerHelper::closeEvent(QCloseEvent* event)
{
	if (ui.btnServerStart->isEnabled() == 0)
	{
		ServerStop();
		m_server->waitForFinished();
		QMessageBox::warning(this, tr("Warning"), tr("Minrcraft Server is still running,\nin order to avoid damaging world data,\nplease stop it brfore closing MCServerHelper!"));
		//event->ignore();
		return;
	}
	ie->WriteMyINI();
	m_frp->terminate();
	m_frp->kill();
	PlayerInfo4Table();
}

void MCServerHelper::timeTik()
{
	ui.labSysTime->setText(QDateTime::currentDateTime().toString("yy/MM/dd hh:mm:ss"));
	QTimer::singleShot(1000, this, SLOT(timeTik()));
	if (m_frp->state() == QProcess::NotRunning)
	{
		ui.btnFrpStart->setEnabled(true);
		ui.btnFrpStop->setEnabled(false);
		ui.btnFrpRestart->setEnabled(false);
		ui.btnRestart->setEnabled(false);
		ui.btnRestart_2->setEnabled(false);
	}
	else if (m_frp->state() == QProcess::Running)
	{
		ui.btnFrpRestart->setEnabled(true);
	}
	if (m_server->state() == QProcess::NotRunning)
	{
		ui.btnServerStart->setEnabled(true);
		ui.btnServerStop->setEnabled(false);
		ui.btnServerInput->setEnabled(false);
		ui.btnServerRestart->setEnabled(false);
		ui.btnServerRestart_2->setEnabled(false);
		ui.btnRestart->setEnabled(false);
		ui.btnRestart_2->setEnabled(false);
	}
	else if (ui.btnServerStop->isEnabled() && m_server->state() == QProcess::Running)
	{
		ui.btnServerRestart->setEnabled(true);
		ui.btnServerRestart_2->setEnabled(true);
	}
	if (ui.btnServerStop->isEnabled() && m_server->state() == QProcess::Running && m_frp->state() == QProcess::Running)
	{
		ui.btnRestart->setEnabled(true);
		ui.btnRestart_2->setEnabled(true);
	}
}

void MCServerHelper::tabChanged(int tabid)
{
	if (tabid == 6)
	{
		ui.frmUpdateInfo->show();
		ui.labAbout->setToolTip("");
		if (!Update::isChecked)
		{
			switch (pUpdate->CheckUpdate())
			{
			case Update::InfoError:
				ui.btnDoUpdate->setEnabled(true);
				break;
			case Update::MustUpdate:
				pUpdate->doUpdate();
			case Update::NotLasted:
				ui.btnDoUpdate->setEnabled(true);
			case Update::CantAuto:
				if (ui.btnShowUpdateInfo->text() == "U")
				{
					ShowUpdateInfo();
				}
				break;
			default:
				break;
				//case Update::DownFail:
				//	break;
				//case Update::Crc32Error:
				//	break;
				//case Update::FileOperateError:
				//	break;
			}
		}
		else
		{
			if (ui.btnShowUpdateInfo->text() == "U" && !pUpdate->isLatest && !pUpdate->isError)
			{
				ShowUpdateInfo();
			}
		}
	}
	else
	{
		ui.frmUpdateInfo->hide();
	}
	if (tabid == 3 || tabid == 0)
	{
		if (ui.WelcomeNote1->toPlainText() == "Default")
			//ui.WelcomeNote1->setPlainText(ui.labW1->text());
			ui.WelcomeNote1->setPlainText("Hi, %PlayerName%! Welcome to our Server: %ServerName%.Your IP is %ThisIP%. Have a good Time.");
		if (ui.WelcomeNote2->toPlainText() == "Default")
			//ui.WelcomeNote2->setPlainText(ui.labW2->text());
			ui.WelcomeNote2->setPlainText("Hi, %PlayerName%! This is your %Times% times to login in to our Server: %ServerName%. Your Last Lost Time is \"%LastLost%\", the IP is %LastIP%, this time is %ThisIP%. Have a good Time.");
	}
}

void MCServerHelper::ShowUpdateInfo()
{
	if (ui.btnShowUpdateInfo->text() == "U")
	{
		ui.btnShowUpdateInfo->setText("D");
		pUpdateFrm->stop();
		pUpdateFrm->setDuration(1000);
		pUpdateFrm->setStartValue(QPoint(ui.frmUpdateInfo->x(), ui.frmUpdateInfo->y()));
		pUpdateFrm->setEndValue(QPoint(0, -10));
		pUpdateFrm->setEasingCurve(QEasingCurve::InOutQuad);
		pUpdateFrm->start();
	}
	else
	{
		ui.btnShowUpdateInfo->setText("U");
		pUpdateFrm->stop();
		pUpdateFrm->setDuration(1000);
		pUpdateFrm->setStartValue(QPoint(ui.frmUpdateInfo->x(), ui.frmUpdateInfo->y()));
		pUpdateFrm->setEndValue(QPoint(0, 130));
		pUpdateFrm->setEasingCurve(QEasingCurve::InOutQuad);
		pUpdateFrm->start();
	}
}

void MCServerHelper::btnDoUpdate()
{
	if (pUpdate->isChecked && pUpdate->isError)
	{
		pUpdate->isChecked = false;
		tabChanged(6);
		ui.btnDoUpdate->setEnabled(false);
	}
	else
	{
		ui.labAbout->setToolTip("");
		ui.btnDoUpdate->setEnabled(false);
		pUpdate->doUpdate();
	}
}

void MCServerHelper::ChangeLanguage()
{
	QTranslator translator;
	QTranslator translator2;
	if (ui.radChinese->isChecked())
	{
		translator.load(":/language/mcserverhelper_zh.qm");
		translator2.load(":/language/qt_zh_CN.qm");
		ui.radChinese->setChecked(true);
	}
	else
	{
		translator.load(":/language/mcserverhelper_en.qm");
		translator2.load(":/language/qt_en.qm");
	}
	qApp->installTranslator(&translator);
	ui.retranslateUi(this);
	ui.labAbout->setText(QString(tr("Build Time: ")) + __TIMESTAMP__);
#ifdef _DEBUG
	this->setWindowTitle(QString("MCServerHelper v") + VERSION + "_DEBUG By:Brownlzy");
#else
	this->setWindowTitle(QString("MCServerHelper v") + VERSION + " By:Brownlzy");
#endif
}

void MCServerHelper::MenuCommand()
{
	QMenu* popMenu = ui.menuCommand;//new QMenu(this);//定义一个右键弹出菜单
	//popMenu->addMenu(ui.menuCommand);
	popMenu->exec(QCursor::pos());//弹出右键菜单，菜单位置为光标位置
}

void MCServerHelper::RestartServer()
{
	if (m_server->state() == QProcess::Running && ui.ServerInput->isEnabled())
	{
		ServerStop();
		QEventLoop loop;
		QTimer::singleShot(5000, &loop, SLOT(quit()));
		loop.exec();
		if (ui.btnServerStart->isEnabled())ServerStart();
	}
}

void MCServerHelper::RestartFrp()
{
	if (ui.btnFrpStop->isEnabled())
	{
		FrpStop();
	}
	QEventLoop loop;
	QTimer::singleShot(3000, &loop, SLOT(quit()));
	loop.exec();
	FrpStart();
}

void MCServerHelper::RestartBoth()
{
	RestartFrp();
	RestartServer();
}

void MCServerHelper::StopBoth()
{
	if (ui.btnFrpStop->isEnabled())
	{
		FrpStop();
	}
	if (ui.ServerInput->isEnabled() && m_server->state() == QProcess::Running)
	{
		ServerStop();
	}
}

void MCServerHelper::RefreshPlayerList()
{
	PlayerInfo4Table();
	pi->ReadPlayerInfo();
	//pi->WritePlayerInfo();
	PlayerInfo2Table();
}
