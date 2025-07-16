#ifndef F_TRANSLATOR_H
#define F_TRANSLATOR_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>
#include <QEventLoop>

class F_Translator : public QObject
{
    Q_OBJECT

public:
    explicit F_Translator(QObject *parent = nullptr);
    ~F_Translator();

    // 非阻塞式翻译
    void translateText(const QString &text, const QString &sourceLang, const QString &targetLang);

    // 阻塞式翻译
    QString translateTextBlocking(const QString &text, const QString &sourceLang, const QString &targetLang);

signals:
    void translationFinished(const QString &translatedText); // 翻译完成信号
    void translationError(const QString &errorMessage);      // 翻译错误信号

private slots:
    void onTranslationFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *networkManager;
    QString apiKey = "your_api_key_here"; // 替换为你的小牛翻译API密钥

    // 用于阻塞式翻译
    QEventLoop eventLoop;
    QString translatedResult;
    bool translationSuccess = false;
};

#endif // F_TRANSLATOR_H
