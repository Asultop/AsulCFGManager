#include "Headers/BaseScrollpage.h"
#include "Headers/BaseInclude.h"
BaseScrollPage::BaseScrollPage(QWidget *parent)
    : ElaScrollPage{parent}
{
    centralWidget = new QWidget(this);
    centerVLayout = new QVBoxLayout(centralWidget);
    centerVLayout->setContentsMargins(5,5,20,30);
    // centerVLayout->setContentsMargins(0,0,0,0);


}
BaseScrollPage::~BaseScrollPage(){

}
void BaseScrollPage::initWidget(QString title,QString subTitle,QString content){
    setWindowTitle(title);
    centralWidget->setWindowTitle(subTitle);
    createCustomWidget(content);
    addCentralWidget(centralWidget,true,true,0);
}
void BaseScrollPage::createCustomWidget(QString desText)
{
    // 顶部元素
    QWidget* customWidget = new QWidget(this);
    ElaText* subTitleText = new ElaText(this);
    subTitleText->setText("     CZM @ Asul.top");
    subTitleText->setTextInteractionFlags(Qt::TextSelectableByMouse);
    subTitleText->setTextPixelSize(11);

    ElaText* descText = new ElaText(this);
    descText->setText(desText);
    descText->setTextPixelSize(13);

    QVBoxLayout* topLayout = new QVBoxLayout(customWidget);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->addWidget(subTitleText);
    topLayout->addSpacing(5);
    topLayout->addWidget(descText);
    setCustomWidget(customWidget);
}
void BaseScrollPage::addTopWidget(QWidget *widget){
    QWidget *nowWidget=getCustomWidget();
    QLayout *topLayout=nowWidget->layout();
    topLayout->addWidget(widget);
    nowWidget->setLayout(topLayout);
    setCustomWidget(nowWidget);
}
ElaScrollPageArea * BaseScrollPage::GenerateArea(QWidget *parent,ElaText * title, QWidget * widget, bool convert){
    ElaScrollPageArea * Area=new ElaScrollPageArea(parent);
    QHBoxLayout * HLayout=new QHBoxLayout(Area);
    if(convert){
        title->setMinimumHeight(45);
        title->setTextPixelSize(15);
        widget->setMinimumHeight(45);
        widget->setMinimumWidth(300);
    }
    HLayout->addWidget(title);
    HLayout->addWidget(widget);
    return Area;
}
ElaScrollPageArea * BaseScrollPage::GenerateArea(QWidget *parent,ElaText * title, QHBoxLayout * widget, bool convert){
    ElaScrollPageArea * Area=new ElaScrollPageArea(parent);
    QHBoxLayout * HLayout=new QHBoxLayout(Area);
    if(convert){
        title->setMinimumHeight(45);
        title->setTextPixelSize(15);
        for (int i = 0; i < widget->count(); ++i) {
            QLayoutItem* item = widget->itemAt(i);
            if (QWidget* wget = item->widget()) {
                wget->setMinimumHeight(45);
            }
        }
    }
    HLayout->addWidget(title);
    HLayout->addLayout(widget);
    return Area;
}
