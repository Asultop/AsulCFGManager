#ifndef T_CFGSETTINGS_H
#define T_CFGSETTINGS_H

#include <ElaWindow.h>
#include "BaseScrollpage.h"
#include "BaseInclude.h"
#include "Headers/F_SteamUserQuery.h"

class T_CFGSettings : public BaseScrollPage
{
    Q_OBJECT
public:
    explicit T_CFGSettings(QWidget* parent = nullptr);

    ~T_CFGSettings();
private:
signals:
    void initFinished();
};

#endif // T_ABOUT_H
