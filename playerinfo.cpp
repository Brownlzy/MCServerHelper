#include "playerinfo.h"

void from_json(const json& j, UserCache& p)
{
	j.at("name").get_to(p.name);
	j.at("uuid").get_to(p.uuid);
	j.at("expiresOn").get_to(p.expiresOn);
}

void from_json(const json& j, WhiteList& p)
{
	j.at("name").get_to(p.name);
	j.at("uuid").get_to(p.uuid);
}

void from_json(const json& j, OP& p)
{
	j.at("name").get_to(p.name);
	j.at("uuid").get_to(p.uuid);
	j.at("level").get_to(p.level);
	j.at("bypassesPlayerLimit").get_to(p.bypassesPlayerLimit);
}

void from_json(const json& j, BannedIP& p)
{
	j.at("ip").get_to(p.ip);
	j.at("created").get_to(p.created);
	j.at("source").get_to(p.source);
	j.at("expires").get_to(p.expires);
	j.at("reason").get_to(p.reason);
}

void from_json(const json& j, PlayerHistory& p)
{
	j.at("name").get_to(p.name);
	j.at("uuid").get_to(p.uuid);
	j.at("times").get_to(p.times);
	j.at("firstlogin").get_to(p.firstlogin);
	j.at("lastlogin").get_to(p.lastlogin);
	j.at("lastlost").get_to(p.lastlost);
	j.at("lastip").get_to(p.lastip);
}

void from_json(const json& j, BannedPlayer& p)
{
	j.at("name").get_to(p.name);
	j.at("uuid").get_to(p.uuid);
	j.at("created").get_to(p.created);
	j.at("source").get_to(p.source);
	j.at("expires").get_to(p.expires);
	j.at("reason").get_to(p.reason);
}

PlayerInfo::PlayerInfo(QObject* parent)
	: QObject(parent)
{
}

PlayerInfo::~PlayerInfo()
{
}

int PlayerInfo::ReadPlayerInfo()
{
	int exec = 0;
	pUserCache = ReadJson<UserCache>(numUserCache, qApp->applicationDirPath().toStdString() + "/Server/userCache.json");
	pOP = ReadJson<OP>(numOP, qApp->applicationDirPath().toStdString() + "/Server/ops.json");
	pWhiteList = ReadJson<WhiteList>(numWhiteList, qApp->applicationDirPath().toStdString() + "/Server/whitelist.json");
	pBannedIP = ReadJson<BannedIP>(numBannedIP, qApp->applicationDirPath().toStdString() + "/Server/banned-ips.json");
	pBannedPlayer = ReadJson<BannedPlayer>(numBannedPlayer, qApp->applicationDirPath().toStdString() + "/Server/banned-players.json");
	pPlayerHistory = ReadJson<PlayerHistory>(numPlayerHistory, qApp->applicationDirPath().toStdString() + "/Server/player-history.json");
	toPlayer();
	return exec;
}

template<typename T>
T* PlayerInfo::ReadJson(int& num, string filePath)
{
	T* pInfo;
	json jInfo;
	std::ifstream jfile(filePath);
	if (!jfile.is_open())
	{
		return nullptr;
	}
	try
	{
		jfile >> jInfo;//json类支持文件流输入进行解析。
		if (jInfo.is_discarded())
		{
			return nullptr;
		}
		num = jInfo.size();
		pInfo = new T[jInfo.size()];
		for (int i = 0; i < jInfo.size(); i++)
		{
			pInfo[i] = jInfo[i];
		}
	}
	catch (std::exception& e)
	{
		e.what();
		return nullptr;
	}
	jfile.close();
	return pInfo;
}

template<typename T>
int PlayerInfo::WriteJson(const int& num, const T* pInfo, const string filePath)
{
	std::ofstream jfile(filePath);
	if (!jfile.is_open())
	{
		return -1;
	}
	jfile << "[";
	for (int i = 0; i < num; i++)
	{
		if (i != 0) jfile << ",";
		jfile << pInfo[i];
	}
	jfile << "]";
	jfile.close();
	return 0;
}

bool PlayerInfo::isOP(QString player)
{
	return false;
}

bool PlayerInfo::isWhiteList(QString player)
{
	return false;
}

int PlayerInfo::WritePlayerInfo()
{
	int exec = 0;
	exec += 0 - WriteJson<UserCache>(numUserCache, pUserCache, qApp->applicationDirPath().toStdString() + "/Server/userCache.json");
	exec += 0 - WriteJson<OP>(numOP, pOP, qApp->applicationDirPath().toStdString() + "/Server/ops.json");
	exec += 0 - WriteJson<WhiteList>(numWhiteList, pWhiteList, qApp->applicationDirPath().toStdString() + "/Server/whitelist.json");
	exec += 0 - WriteJson<BannedIP>(numBannedIP, pBannedIP, qApp->applicationDirPath().toStdString() + "/Server/banned-ips.json");
	exec += 0 - WriteJson<BannedPlayer>(numBannedPlayer, pBannedPlayer, qApp->applicationDirPath().toStdString() + "/Server/banned-players.json");
	exec += 0 - WriteJson<PlayerHistory>(numPlayerHistory, pPlayerHistory, qApp->applicationDirPath().toStdString() + "/Server/player-history.json");
	return exec;
}

int PlayerInfo::toPlayer()
{
	numPlayer = 0;
	int userAll = numBannedPlayer + numOP + numPlayerHistory + numUserCache + numWhiteList;
	QString* pTmp = new QString[userAll];
	int i = 0, j = 0;
	for (i = 0; i < numBannedPlayer; i++)
	{
		for (j = 0; j < userAll; j++)
		{
			if (pTmp[j] == "")
			{
				numPlayer++;
				pTmp[j] = QString::fromStdString(pBannedPlayer[i].uuid);
				break;
			}
			else if (pTmp[j] == QString::fromStdString(pBannedPlayer[i].uuid))
			{
				break;
			}
		}
	}
	for (i = 0; i < numUserCache; i++)
	{
		for (j = 0; j < userAll; j++)
		{
			if (pTmp[j] == "")
			{
				numPlayer++;
				pTmp[j] = QString::fromStdString(pUserCache[i].uuid);
				break;
			}
			else if (pTmp[j] == QString::fromStdString(pUserCache[i].uuid))
			{
				break;
			}
		}
	}
	for (i = 0; i < numOP; i++)
	{
		for (j = 0; j < userAll; j++)
		{
			if (pTmp[j] == "")
			{
				numPlayer++;
				pTmp[j] = QString::fromStdString(pOP[i].uuid);
				break;
			}
			else if (pTmp[j] == QString::fromStdString(pOP[i].uuid))
			{
				break;
			}
		}
	}
	for (i = 0; i < numWhiteList; i++)
	{
		for (j = 0; j < userAll; j++)
		{
			if (pTmp[j] == "")
			{
				numPlayer++;
				pTmp[j] = QString::fromStdString(pWhiteList[i].uuid);
				break;
			}
			else if (pTmp[j] == QString::fromStdString(pWhiteList[i].uuid))
			{
				break;
			}
		}
	}
	for (i = 0; i < numPlayerHistory; i++)
	{
		for (j = 0; j < userAll; j++)
		{
			if (pTmp[j] == "")
			{
				numPlayer++;
				pTmp[j] = QString::fromStdString(pPlayerHistory[i].uuid);
				break;
			}
			else if (pTmp[j] == QString::fromStdString(pPlayerHistory[i].uuid))
			{
				break;
			}
		}
	}

	pPlayer = new Player[numPlayer];//======================================

	for (i = 0; i < numBannedPlayer; i++)
	{
		for (j = 0; j < numPlayer; j++)
		{
			if (pPlayer[j].UUID == "")
			{
				pPlayer[j].UUID = QString::fromStdString(pBannedPlayer[i].uuid);
				pPlayer[j].ID = QString::fromStdString(pBannedPlayer[i].name);
				pPlayer[j].isBanned = true;
				break;
			}
			else if (pPlayer[j].UUID == QString::fromStdString(pBannedPlayer[i].uuid))
			{
				pPlayer[j].isBanned = true;
				break;
			}
		}
	}
	for (i = 0; i < numUserCache; i++)
	{
		for (j = 0; j < numPlayer; j++)
		{
			if (pPlayer[j].UUID == "")
			{
				pPlayer[j].UUID = QString::fromStdString(pUserCache[i].uuid);
				pPlayer[j].ID = QString::fromStdString(pUserCache[i].name);
				break;
			}
			else if (pPlayer[j].UUID == QString::fromStdString(pUserCache[i].uuid))
			{
				break;
			}
		}
	}
	for (i = 0; i < numOP; i++)
	{
		for (j = 0; j < numPlayer; j++)
		{
			if (pPlayer[j].UUID == "")
			{
				pPlayer[j].UUID = QString::fromStdString(pOP[i].uuid);
				pPlayer[j].ID = QString::fromStdString(pOP[i].name);
				pPlayer[j].isOP = true;
				pPlayer[j].OPLevel = pOP[i].level;
				break;
			}
			else if (pPlayer[j].UUID == QString::fromStdString(pOP[i].uuid))
			{
				pPlayer[j].isOP = true;
				pPlayer[j].OPLevel = pOP[i].level;
				break;
			}
		}
	}
	for (i = 0; i < numWhiteList; i++)
	{
		for (j = 0; j < numPlayer; j++)
		{
			if (pPlayer[j].UUID == "")
			{
				pPlayer[j].UUID = QString::fromStdString(pWhiteList[i].uuid);
				pPlayer[j].ID = QString::fromStdString(pWhiteList[i].name);
				pPlayer[j].isWhitelist = true;
				break;
			}
			else if (pPlayer[j].UUID == QString::fromStdString(pWhiteList[i].uuid))
			{
				pPlayer[j].isWhitelist = true;
				break;
			}
		}
	}
	for (i = 0; i < numPlayerHistory; i++)
	{
		for (j = 0; j < numPlayer; j++)
		{
			if (pPlayer[j].UUID == "")
			{
				pPlayer[j].UUID = QString::fromStdString(pPlayerHistory[i].uuid);
				pPlayer[j].ID = QString::fromStdString(pPlayerHistory[i].name);
				pPlayer[j].Times = pPlayerHistory[i].times + 1;
				pPlayer[j].FirstLogin = QString::fromStdString(pPlayerHistory[i].firstlogin);
				pPlayer[j].LastLogin = QString::fromStdString(pPlayerHistory[i].lastlogin);
				pPlayer[j].LastLost = QString::fromStdString(pPlayerHistory[i].lastlost);
				pPlayer[j].LastIP = QString::fromStdString(pPlayerHistory[i].lastip);
				break;
			}
			else if (pPlayer[j].UUID == QString::fromStdString(pPlayerHistory[i].uuid))
			{
				pPlayer[j].Times = pPlayerHistory[i].times + 1;
				pPlayer[j].FirstLogin = QString::fromStdString(pPlayerHistory[i].firstlogin);
				pPlayer[j].LastLogin = QString::fromStdString(pPlayerHistory[i].lastlogin);
				pPlayer[j].LastLost = QString::fromStdString(pPlayerHistory[i].lastlost);
				pPlayer[j].LastIP = QString::fromStdString(pPlayerHistory[i].lastip);
				break;
			}
		}
	}
	return 0;
}

int PlayerInfo::getPlayerInfoIndex(QString name, QString uuid)
{
	for (int i = 0; i < numPlayer; i++)
	{
		if (uuid != "NULL")
		{
			if (pPlayer[i].ID == name && pPlayer[i].UUID == uuid)
				return i;
		}
		else
		{
			if (pPlayer[i].ID == name)
				return i;
		}
	}
	return -1;
}

void PlayerInfo::storePlayerHistory(Player* p, int num)
{
	delete[]pPlayerHistory;
	pPlayerHistory = new PlayerHistory[num];
	for (int i = 0; i < num; i++)
	{
		pPlayerHistory[i].name = p[i].ID.toStdString();
		pPlayerHistory[i].uuid = p[i].UUID.toStdString();
		pPlayerHistory[i].times = p[i].Times;
		pPlayerHistory[i].firstlogin = p[i].FirstLogin.toStdString();
		pPlayerHistory[i].lastlogin = p[i].LastLogin.toStdString();
		pPlayerHistory[i].lastlost = p[i].LastLost.toStdString();
		pPlayerHistory[i].lastip = p[i].LastIP.toStdString();
	}
}
