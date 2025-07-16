#ifndef F_STEAMUSERQUERY_H
#define F_STEAMUSERQUERY_H

#include <QList>
#include <QObject>
#include <QString>
#include <vdf_parser.hpp>

struct SteamUserInfo {
    QString userId;
    QString userShortId;
    QString accountName;
    QString personaName;
    bool rememberPassword;
    bool wantsOfflineMode;
    bool skipOfflineModeWarning;
    bool allowAutoLogin;
    bool mostRecent;
    qint64 timestamp;
};

class F_SteamUserQuery : public QObject {
    Q_OBJECT
public:
    static QList<SteamUserInfo> parseUsersFile(const QString& filePath);
    static SteamUserInfo getUserById(const QList<SteamUserInfo>& userList, const QString& userId);
    static QList<SteamUserInfo> getMostRecentUsers(const QList<SteamUserInfo>& userList);
private:
    static qint32 ConvertSteamID64ToShortID(qint64 steamID64) ;
};

#endif // F_STEAMUSERQUERY_H
