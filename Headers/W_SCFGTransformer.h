#ifndef W_SCFGTRANSFORMER_H
#define W_SCFGTRANSFORMER_H

#include <QWidget>
#include <ElaWidget.h>
#include "Headers/stdafx.h"
#include "Headers/BaseScrollpage.h"
#include "Headers/GlobalSettings.h"

class W_SCFGTransformer : public ElaWidget
{
    Q_OBJECT
    Q_PROPERTY_CREATE(bool,selfMove);
public:
    bool programMove=false;
    int badges=0;
    explicit W_SCFGTransformer(QWidget *parent = nullptr);
    BaseScrollPage *geneFunc;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void moveEvent(QMoveEvent *event) override;

    ~W_SCFGTransformer();
signals:
};

#endif // W_SCFGTRANSFORMER_H
