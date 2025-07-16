#include "Headers/F_Translator.h"
#include <QNetworkRequest>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

F_Translator::F_Translator(QObject *parent)
    : QObject(parent)
{
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &F_Translator::onTranslationFinished);
}

F_Translator::~F_Translator()
{
    delete networkManager;
}

void F_Translator::translateText(const QString &text, const QString &sourceLang, const QString &targetLang)
{
    QUrl url("https://api.niutrans.com/NiuTransServer/translation");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery postData;
    postData.addQueryItem("from", sourceLang);
    postData.addQueryItem("to", targetLang);
    postData.addQueryItem("apikey", apiKey);
    postData.addQueryItem("src_text", text);

    networkManager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
}

QString F_Translator::translateTextBlocking(const QString &text, const QString &sourceLang, const QString &targetLang)
{
    // 重置状态
    translationSuccess = false;
    translatedResult.clear();

    // 发起翻译请求
    translateText(text, sourceLang, targetLang);

    // 进入事件循环，等待翻译完成
    eventLoop.exec();

    // 返回翻译结果
    if (translationSuccess) {
        return translatedResult;
    } else {
        return QString(); // 返回空字符串表示翻译失败
    }
}

void F_Translator::onTranslationFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(response);
        QJsonObject jsonObject = jsonResponse.object();

        if (jsonObject.contains("tgt_text")) {
            translatedResult = jsonObject["tgt_text"].toString();
            translationSuccess = true;
            emit translationFinished(translatedResult); // 发射翻译完成信号
        } else {
            emit translationError(tr("翻译失败，返回结果无效。"));
        }
    } else {
        emit translationError(tr("网络请求失败: ") + reply->errorString());
    }

    // 退出事件循环
    eventLoop.quit();

    reply->deleteLater();
}
