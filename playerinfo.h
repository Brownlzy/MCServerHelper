#pragma once

#include <QObject>
#include <QMessageBox>
#include <fstream>
#include <iostream>
#include <QApplication>
#include <QDebug>
#include "json.hpp"
using namespace std;
using json = nlohmann::json;//此处是别名，方便使用

struct UserCache
{
	string name = "";
	string uuid = "";
	string expiresOn = "";
	friend ostream& operator<<(ostream& out, const UserCache& obj)
	{
		out << "{\"name\":\"" << obj.name << "\",\"uuid\":\"" << obj.uuid << "\",\"expiresOn\":\"" << obj.expiresOn << "\"}";
		return out;
	}
};

struct OP
{
	string uuid = "";
	string name = "";
	int level = 0;
	bool bypassesPlayerLimit = false;
	friend ostream& operator<<(ostream& out, const OP& obj)
	{
		out << "{\"uuid\":\"" << obj.uuid << "\",\"name\":\"" << obj.name << "\",\"level\":" << obj.level << ",\"bypassesPlayerLimit\":" << boolalpha << obj.bypassesPlayerLimit << "}";
		return out;
	}
};

struct WhiteList
{
	string uuid = "";
	string name = "";
	friend ostream& operator<<(ostream& out, const WhiteList& obj)
	{
		out << "{\"uuid\":\"" << obj.uuid << "\",\"name\":\"" << obj.name << "\"}";
		return out;
	}
};

struct BannedIP
{
	string ip = "";
	string created = "";
	string source = "";
	string expires = "";
	string reason = "";
	friend ostream& operator<<(ostream& out, const BannedIP& obj)
	{
		out << "{\"ip\":\"" << obj.ip << "\",\"created\":\"" << obj.created << "\",\"source\":\"" << obj.source << "\",\"expires\":\"" << obj.expires << "\",\"reason\":\"" << obj.reason << "\"}";
		return out;
	}
};

struct BannedPlayer
{
	string uuid = "";
	string name = "";
	string created = "";
	string source = "";
	string expires = "";
	string reason = "";
	friend ostream& operator<<(ostream& out, const BannedPlayer& obj)
	{
		out << "{\"uuid\":\"" << obj.uuid << "\",\"name\":\"" << obj.name << "\",\"created\":\"" << obj.created << "\",\"source\":\"" << obj.source << "\",\"expires\":\"" << obj.expires << "\",\"reason\":\"" << obj.reason << "\"}";
		return out;
	}
};

struct PlayerHistory
{
	string uuid = "";
	string name = "";
	string firstlogin = "";
	string lastlogin = "";
	string lastlost = "";
	string lastip = "";
	friend ostream& operator<<(ostream& out, const PlayerHistory& obj)
	{
		out << "{\"uuid\":\"" << obj.uuid << "\",\"name\":\"" << obj.name << "\",\"firstlogin\":\"" << obj.firstlogin << "\",\"lastlogin\":\"" << obj.lastlogin << "\",\"lastlost\":\"" << obj.lastlost << "\",\"lastip\":\"" << obj.lastip << "\"}";
		return out;
	}
};

struct Player
{
	QString ID = "";
	QString UUID = "";
	bool isOP = false;
	bool isWhitelist = false;
	bool isBanned = false;
	int OPLevel = 0;
	QString FirstLogin = "";
	QString LastIP = "";
	QString LastLogin = "-";
	QString LastLost = "-";
};


class PlayerInfo : public QObject
{
	Q_OBJECT

public:
	PlayerInfo(QObject* parent);
	~PlayerInfo();
	int ReadPlayerInfo();
	template <typename T>
	T* ReadJson(int& num, const string filePath);
	bool isOP(QString player);
	bool isWhiteList(QString player);
	int WritePlayerInfo();
	template <typename T>
	int WriteJson(const int& num, const T* pInfo, const string filePath);
	int toPlayer();
	int fromPlayer();
	int getPlayerInfoIndex(QString name, QString uuid);
	void storePlayerHistory(Player* p, int num);

	int numUserCache = 0;
	int numOP = 0;
	int numWhiteList = 0;
	int numPlayer = 0;
	int numBannedIP = 0;
	int numBannedPlayer = 0;
	int numPlayerHistory = 0;

	UserCache* pUserCache = nullptr;
	OP* pOP = nullptr;
	WhiteList* pWhiteList = nullptr;
	BannedIP* pBannedIP = nullptr;
	BannedPlayer* pBannedPlayer = nullptr;
	PlayerHistory* pPlayerHistory = nullptr;
	Player* pPlayer = nullptr;
	Player* pPlayer2 = nullptr;

};
