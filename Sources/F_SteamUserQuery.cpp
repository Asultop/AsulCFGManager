#include "Headers/F_SteamUserQuery.h"
#include <QsLog.h>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QDebug>
#include <vdf_parser.hpp>

#define DBG(x) QLOG_DEBUG()<<"[Key] "<<#x<<" : "<<x;
#define _DBG(label,x) QLOG_DEBUG()<<"["<<label<<"] "<<#x<<" : "<<x;


QList<SteamUserInfo> F_SteamUserQuery::parseUsersFile(const QString& filePath) {
    QList<SteamUserInfo> userList;

    try {
        // 1. 读取文件内容
        std::ifstream fileS(filePath.toStdString());
        auto root = tyti::vdf::read(fileS);

        // 3. 获取users节点
        // auto& childs = root.childs["users"];
        for(const auto& child : root.childs){
            SteamUserInfo userInfo;
            userInfo.userId = QString::fromStdString(child.first);
            userInfo.userShortId = QString::number(ConvertSteamID64ToShortID(userInfo.userId.toLongLong()));
            userInfo.accountName = QString::fromStdString(child.second->attribs["AccountName"]);
            userInfo.personaName = QString::fromStdString(child.second->attribs["PersonaName"]);
            userInfo.wantsOfflineMode = (child.second->attribs["WantsOfflineMode"] == "1");
            userInfo.skipOfflineModeWarning = (child.second->attribs["SkipOfflineModeWarning"] == "1");
            userInfo.allowAutoLogin = (child.second->attribs["AllowAutoLogin"] == "1");
            userInfo.mostRecent = (child.second->attribs["MostRecent"] == "1");
            userInfo.timestamp = QString::fromStdString(child.second->attribs["Timestamp"]).toLongLong();
            userList.append(userInfo);

        }

        QLOG_DEBUG() << tr("成功解析") << userList.size() << tr("个用户");
    }
    catch (const std::exception& e) {
        QLOG_ERROR() << tr("VDF解析错误:") << e.what();
    }

    return userList;
}
SteamUserInfo F_SteamUserQuery::getUserById(const QList<SteamUserInfo>& userList, const QString& userId) {
    for (const auto& user : userList) {
        if (user.userId == userId) {
            return user;
        }
    }
    return SteamUserInfo(); // 返回空用户
}

QList<SteamUserInfo> F_SteamUserQuery::getMostRecentUsers(const QList<SteamUserInfo>& userList) {
    QList<SteamUserInfo> result;
    for (const auto& user : userList) {
        if (user.mostRecent) {
            result.append(user);
        }
    }
    return result;
}

qint32 F_SteamUserQuery::ConvertSteamID64ToShortID(qint64 steamID64)
{

    constexpr uint64_t STEAM_ID64_BASE = 76561197960265728ULL;
    if (steamID64 < STEAM_ID64_BASE) {
        QLOG_ERROR() << tr("错误：无效的SteamID64（小于基数）" );
        return 0;
    }
    return static_cast<uint32_t>(steamID64 - STEAM_ID64_BASE);

}
