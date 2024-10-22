#ifndef T_HOME_H
#define T_HOME_H

#include <QObject>
#include "BaseInclude.h"
#include <ElaPopularCard.h>
class T_Home :public BaseScrollPage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_Home(QWidget *parent=nullptr);
    ~T_Home();
    ElaPopularCard* homeCard;
};

#endif // T_HOME_H
