#ifndef F_LINEEDITCOLLECTOR_H
#define F_LINEEDITCOLLECTOR_H
#include "Headers/BaseInclude.h"

class Collector : public QObject {
    Q_OBJECT
private:
    QList<ElaText*> labels;  // 存储所有QLabel
    QList<QString> values;  // 存储对应的值

public:
    Collector(QWidget * parent=nullptr);
    ~Collector();
    int addLabel(ElaText* label) ;
    void setValue(int index, const QString& value) ;

    QList<QString> getValues() ;

signals:
    void valuesChanged(const QList<QString>& newValues);
};

#endif // F_LINEEDITCOLLECTOR_H
