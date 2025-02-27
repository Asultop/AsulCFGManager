#include "Headers/T_CFGSettings.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QVBoxLayout>
#include <ElaWindow.h>
// #include "ElaImageCard.h"
// #include "ElaText.h"


T_CFGSettings::T_CFGSettings(QWidget* parent, QMap<QString,QVariant> *data)
    : ElaWindow(parent)
{
    this->makeIndependence();

}

T_CFGSettings::~T_CFGSettings()
{
}
