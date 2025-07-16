#ifndef T_USERSETTING_H
#define T_USERSETTING_H

#include <QObject>
#include <QWidget>
#include <ElaWindow.h>
#include "Headers/BaseInclude.h"
#include "Headers/BaseScrollpage.h"
#include "Headers/F_SteamUserQuery.h"
class T_UserSetting : public ElaWindow
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_UserSetting(QWidget *parent = nullptr);
    ~T_UserSetting();
    void initWindow(const SteamUserInfo *usr);
private:
    bool m_inited=false;
signals:
};

#endif // T_USERSETTING_H
