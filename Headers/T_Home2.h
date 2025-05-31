#ifndef T_HOME2_H
#define T_HOME2_H

#include "Headers/T_BasePage.h"
#include "ElaPopularCard.h"
class ElaMenu;
class T_Home2 : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_Home2(QWidget* parent = nullptr);
    ~T_Home2();
    ElaPopularCard* homeCard2;
Q_SIGNALS:
    Q_SIGNAL void elaScreenNavigation();
    Q_SIGNAL void elaBaseComponentNavigation();
    Q_SIGNAL void elaSceneNavigation();
    Q_SIGNAL void elaCardNavigation();
    Q_SIGNAL void elaIconNavigation();

protected:
    virtual void mouseReleaseEvent(QMouseEvent* event);

private:
    ElaMenu* _homeMenu{nullptr};
};

#endif // T_HOME_H
