#ifndef T_CFGSETTINGS_H
#define T_CFGSETTINGS_H

#include <ElaWindow.h>
#include "BaseInclude.h"

class T_CFGSettings : public ElaWindow
{
    Q_OBJECT
public:
    explicit T_CFGSettings(QWidget* parent = nullptr,QMap<QString,QVariant> *data=nullptr);
    ~T_CFGSettings();
};

#endif // T_ABOUT_H
