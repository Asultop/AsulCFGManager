#ifndef BANELACOMBOBOX_H
#define BANELACOMBOBOX_H

#include "ElaComboBox.h"
#include "qevent.h"
#include <QWidget>

class BanElaComboBox:public ElaComboBox{
    Q_OBJECT
public:
    explicit BanElaComboBox(QWidget* parent = nullptr);
    ~BanElaComboBox();
    void wheelEvent(QWheelEvent *e) override
    {
        //disable the scroll function
        e->ignore();
    }
};

#endif // BANELACOMBOBOX_H
