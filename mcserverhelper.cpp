#include "mcserverhelper.h"

MCServerHelper::MCServerHelper(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ie = new iniEdit(this);
	UpdateSettings();
	QTranslator translator;
	if (ie->ReadMyINI(QString("Language")) == "zh_cn")
	{
		translator.load(":/language/mcserverhelper_zh.qm");
		ui.radChinese->setChecked(true);
	}
	else
	{
		translator.load(":/language/mcserverhelper_en.qm");
	}
	qApp->installTranslator(&translator);
	ui.retranslateUi(this);
	this->setFixedSize(this->width(), this->height());
	this->setWindowTitle(QString("MCServerHelper v") + VERSION + " By:Brownlzy");
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
	ui.frmUpdateInfo->move(0, 110);
	connect(m_server, SIGNAL(readyReadStandardOutput()), this, SLOT(onServerOutput()));
	connect(m_frp, SIGNAL(readyReadStandardOutput()), this, SLOT(onFrpOutput()));
	connect(ui.btnServerStart, SIGNAL(clicked()), this, SLOT(ServerStart()));
	connect(ui.btnFrpStart, SIGNAL(clicked()), this, SLOT(FrpStart()));
	connect(ui.btnServerInput, SIGNAL(clicked()), this, SLOT(ServerInput()));
	connect(ui.btnServerStop, SIGNAL(clicked()), this, SLOT(ServerStop()));
	connect(ui.btnFrpStop, SIGNAL(clicked()), this, SLOT(FrpStop()));
	connect(ui.btnMCSHConfirm, SIGNAL(clicked()), this, SLOT(MSCHConfirm()));
	connect(ui.btnServerSConfirm, SIGNAL(clicked()), this, SLOT(ServerINIConfirm()));
	connect(ui.btnFrpSConfirm, SIGNAL(clicked()), this, SLOT(FrpConfirm()));
	connect(ui.btnMCSHCancel, SIGNAL(clicked()), this, SLOT(MSCHCancel()));
	connect(ui.btnServerSCancel, SIGNAL(clicked()), this, SLOT(ServerINICancel()));
	connect(ui.btnFrpSCancel, SIGNAL(clicked()), this, SLOT(FrpCancel()));
	connect(ui.tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
	connect(ui.btnShowUpdateInfo, SIGNAL(clicked()), this, SLOT(ShowUpdateInfo()));
	connect(ui.btnDoUpdate, SIGNAL(clicked()), this, SLOT(btnDoUpdate()));
	connect(ui.btnChangeLanguage, SIGNAL(clicked()), this, SLOT(ChangeLanguage()));
	QTimer::singleShot(1000, this, SLOT(timeTik()));
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
	ui.PathJava->setText(QString::fromStdString(ie->myini.JavaPath).replace("&nbsp;", " "));
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
		colorText += "<span style=\"color:";
		if (oText.split("<br/>")[i].contains("ERROR") || oText.split("<br/>")[i].contains("错误"))
			colorText += "red;\">";
		else if (oText.contains("[") && oText.contains("]") && oText.split("<br/>")[i].split("[")[2].split("]")[0].contains("INFO"))
			colorText += "green;\">";
		else if (oText.contains("[") && oText.contains("]") && oText.split("<br/>")[i].split("[")[2].split("]")[0].contains("WARN"))
			colorText += "#ff7e00;\">";
		else if (oText.contains("[") && oText.contains("]") && oText.split("<br/>")[i].split("[")[2].split("]")[0].contains("FATAL"))
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
	QByteArray tmp = ui.ServerInput->text().toLatin1() + "\n";
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

void MCServerHelper::MSCHConfirm()
{
	if (ui.PathJava->text() == "") ui.PathJava->setText("NULL");
	if (ui.PathServer->text() == "") ui.PathServer->setText("NULL");
	if (ui.PathFrp->text() == "") ui.PathFrp->setText("NULL");
	if (ui.PathFrpIni->text() == "") ui.PathFrpIni->setText("NULL");
	ie->myini.isStartWithWindow = ui.chkStartWithWindows->isChecked();
	ie->myini.isStartServerOnceStarted = ui.chkStartServerOnce->isChecked();
	ie->myini.isStartFrpWithServer = ui.chkStartFrpOnce->isChecked();
	if (ui.radChinese->isChecked())
		ie->myini.Language = "zh_cn";
	else
		ie->myini.Language = "en_us";
	ie->myini.JavaPath = ui.PathJava->text().replace(" ", "&nbsp;").toStdString();
	ie->myini.MinMemory = ui.MinRam->text().toInt();
	ie->myini.MaxMemory = ui.MaxRam->text().toInt();
	ie->myini.ServerPath = ui.PathServer->text().replace(" ", "&nbsp;").toStdString();
	ie->myini.FrpPath = ui.PathFrp->text().replace(" ", "&nbsp;").toStdString();
	ie->myini.FrpINIPath = ui.PathFrpIni->text().replace(" ", "&nbsp;").toStdString();
	ie->WriteMyINI();
	UpdateSettings();
	ChangeLanguage();
}

void MCServerHelper::MSCHCancel()
{
	UpdateSettings();
}

void MCServerHelper::ServerINIConfirm()
{
	ie->myini.ServerINI = ui.INIServer->toPlainText().toStdString();
	ie->WriteServerINI();
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
}

void MCServerHelper::timeTik()
{
	ui.labSysTime->setText(QDateTime::currentDateTime().toString("yy/MM/dd hh:mm:ss"));
	QTimer::singleShot(1000, this, SLOT(timeTik()));
}

void MCServerHelper::tabChanged(int tabid)
{
	if (tabid == 6)
	{
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
}

void MCServerHelper::ShowUpdateInfo()
{
	if (ui.btnShowUpdateInfo->text() == "U")
	{
		ui.btnShowUpdateInfo->setText("D");
		pUpdateFrm->stop();
		pUpdateFrm->setDuration(1000);
		pUpdateFrm->setStartValue(QPoint(ui.frmUpdateInfo->x(), ui.frmUpdateInfo->y()));
		pUpdateFrm->setEndValue(QPoint(0, -34));
		pUpdateFrm->setEasingCurve(QEasingCurve::InOutQuad);
		pUpdateFrm->start();
	}
	else
	{
		ui.btnShowUpdateInfo->setText("U");
		pUpdateFrm->stop();
		pUpdateFrm->setDuration(1000);
		pUpdateFrm->setStartValue(QPoint(ui.frmUpdateInfo->x(), ui.frmUpdateInfo->y()));
		pUpdateFrm->setEndValue(QPoint(0, 110));
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
		ui.btnDoUpdate->setEnabled(false);
		pUpdate->doUpdate();
	}
}

void MCServerHelper::ChangeLanguage()
{
	QTranslator translator;
	if (ui.radChinese->isChecked())
	{
		translator.load(":/language/mcserverhelper_zh.qm");
		ui.radChinese->setChecked(true);
	}
	else
	{
		translator.load(":/language/mcserverhelper_en.qm");
	}
	qApp->installTranslator(&translator);
	ui.retranslateUi(this);
	ui.labAbout->setText(QString(tr("Build Time: ")) + __TIMESTAMP__);
	this->setWindowTitle(QString("MCServerHelper v") + VERSION + " By:Brownlzy");
}
