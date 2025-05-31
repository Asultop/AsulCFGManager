#include "Headers/F_Collector.h"

int Collector::addLabel(ElaText* label) {
    int index = labels.size(); // 获取当前索引
    labels.append(label);
    values.append(label->text());
    label->setProperty("num",index);
    return index;  // 返回添加的位置索引
}
Collector::Collector(QWidget *parent)
    : QObject{parent}
{

}
Collector::~Collector(){

}

void Collector::setValue(int index, const QString& value) {
    if (index >= 0 && index < labels.size()) {
        // labels[index]->setText(value);
        values[index] = value;
        emit valuesChanged(values);
    }
}

QList<QString> Collector::getValues()  {
    return values;
}
