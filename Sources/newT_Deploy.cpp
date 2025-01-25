// T_Deploy.cpp

#include "T_Deploy.h"
#include "ElaText.h"
#include "ElaLineEdit.h"
#include "ElaPushButton.h"
#include "ElaLabel.h"
#include "ElaScrollPageArea.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

void T_Deploy::generateScrollPageLayout(QString CFGFileLocation, ElaFlowLayout *cVLayout) {
    // ... 现有代码 ...

    ElaFlowLayout *settingsLayout = new ElaFlowLayout(this);
    settingsLayout->setContentsMargins(10, 10, 10, 10);

    for (auto it = settings.constBegin(); it != settings.constEnd(); ++it) {
        QJsonObject setting = it.value().toObject();
        QString title = setting.value("title").toString();

        if (setting.contains("keyPreference")) {
            QJsonObject keyPreference = setting["keyPreference"].toObject();
            for (auto keyIt = keyPreference.constBegin(); keyIt != keyPreference.constEnd(); ++keyIt) {
                QJsonObject keyObj = keyIt.value().toObject();
                QString text = keyObj["text"].toString();
                QString value = keyObj["value"].toString();
                QString defaultValue = keyObj["default"].toString();

                ElaScrollPageArea *itemArea = new ElaScrollPageArea(this);
                QHBoxLayout *itemLayout = new QHBoxLayout(itemArea);
                ElaText *label = new ElaText(tr(text), this);
                ElaLineEdit *lineEdit = new ElaLineEdit(this);
                lineEdit->setText(value);
                lineEdit->setProperty("default", defaultValue); // 设置默认值以便重置

                itemLayout->addWidget(label);
                itemLayout->addWidget(lineEdit);

                // 添加默认值指示图标
                ElaLabel *defaultIcon = new ElaLabel(this);
                defaultIcon->setPixmap(QPixmap(":/icons/default.png"));
                itemLayout->addWidget(defaultIcon);

                settingsLayout->addWidget(itemArea);
            }
        } else if (setting.contains("optPreference")) {
            // 处理optPreference的情况
            // ... (之前的代码)
        }
    }

    // 添加保存和重置按钮
    ElaPushButton *saveButton = new ElaPushButton(tr("Save"), this);
    ElaPushButton *resetButton = new ElaPushButton(tr("Reset to Default"), this);
    settingsLayout->addWidget(saveButton);
    settingsLayout->addWidget(resetButton);

    // 连接保存按钮
    connect(saveButton, &ElaPushButton::clicked, this, &T_Deploy::saveSettings);
    // 连接重置按钮
    connect(resetButton, &ElaPushButton::clicked, this, &T_Deploy::resetToDefault);

    // 将设置布局添加到主布局中
    cVLayout->addLayout(settingsLayout);
}

void T_Deploy::saveSettings() {
    QJsonObject keyPrefsObj, optPrefsObj;
    for (int i = 0; i < layout()->count(); ++i) {
        QLayoutItem *item = layout()->itemAt(i);
        if (item->widget()) {
            ElaScrollPageArea *area = qobject_cast<ElaScrollPageArea*>(item->widget());
            if (area) {
                QHBoxLayout *hLayout = qobject_cast<QHBoxLayout*>(area->layout());
                ElaLineEdit *lineEdit = qobject_cast<ElaLineEdit*>(hLayout->itemAt(1)->widget());
                if (lineEdit) {
                    QString key = hLayout->itemAt(0)->widget()->property("text").toString();
                    QString value = lineEdit->text();

                    if (key.startsWith("keyPreference")) {
                        keyPrefsObj.insert(key, value);
                    } else if (key.startsWith("optPreference")) {
                        optPrefsObj.insert(key, value);
                    }
                }
            }
        }
    }

    // 保存到文件
    QFile keyPrefFile("keyPreference.cfg");
    if (keyPrefFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&keyPrefFile);
        out << QJsonDocument(keyPrefsObj).toJson(QJsonDocument::Compact);
    }

    QFile optPrefFile("optPreference.cfg");
    if (optPrefFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&optPrefFile);
        out << QJsonDocument(optPrefsObj).toJson(QJsonDocument::Compact);
    }
}

void T_Deploy::resetToDefault() {
    // 遍历所有ElaLineEdit并重置到默认值
    for (int i = 0; i < layout()->count(); ++i) {
        QLayoutItem *item = layout()->itemAt(i);
        if (item->widget()) {
            ElaScrollPageArea *area = qobject_cast<ElaScrollPageArea*>(item->widget());
            if (area) {
                QHBoxLayout *hLayout = qobject_cast<QHBoxLayout*>(area->layout());
                ElaLineEdit *lineEdit = qobject_cast<ElaLineEdit*>(hLayout->itemAt(1)->widget());
                if (lineEdit) {
                    QString defaultValue = lineEdit->property("default").toString();
                    lineEdit->setText(defaultValue);
                }
            }
        }
    }
}
