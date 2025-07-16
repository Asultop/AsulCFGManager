#ifndef STEAMAVATARFETCHER_H
#define STEAMAVATARFETCHER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QCoreApplication>
#include <QsLog.h>

// 自定义日志宏（项目实际使用时替换为项目特定的日志系统）
#ifndef QLOG_DEBUG
#include <QDebug>
#define QLOG_DEBUG qDebug
#endif

class SteamAvatarFetcher : public QObject
{
    Q_OBJECT
public:
    explicit SteamAvatarFetcher(QObject *parent = nullptr);

    // 启动头像获取过程（带自动重试）
    void fetchAvatarUrls(const QString &apiKey, quint64 steamId64);

    // 设置重试间隔（毫秒）
    void setRetryInterval(int interval);

    // 设置最大重试次数（0表示无限重试）
    void setMaxRetries(int max);

signals:
    // 成功信号：返回三种尺寸的头像URL [小, 中, 大]
    void avatarUrlsReady(const QStringList &avatarUrls);

    // 错误信号：返回错误信息和当前重试次数
    void fetchError(const QString &error, int retryCount);

    // 重试信号：每次重试时触发
    void retryAttempt(int currentRetry, int maxRetries);

private slots:
    void handleNetworkReply(QNetworkReply *reply);
    void performFetch();

private:
    QNetworkAccessManager *m_manager;
    QString m_apiKey;
    quint64 m_steamId64;
    int m_retryCount = 0;
    int m_maxRetries = 0; // 0 = 无限重试
    int m_retryInterval = 3000; // 默认3秒重试
    QTimer m_retryTimer;
};

#endif // STEAMAVATARFETCHER_H
