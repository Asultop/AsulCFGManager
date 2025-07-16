#include "Headers/F_SteamAvatarFetcher.h"

SteamAvatarFetcher::SteamAvatarFetcher(QObject *parent)
    : QObject(parent)
{
    m_manager = new QNetworkAccessManager(this);
    connect(m_manager, &QNetworkAccessManager::finished,
            this, &SteamAvatarFetcher::handleNetworkReply);

    connect(&m_retryTimer, &QTimer::timeout, this, &SteamAvatarFetcher::performFetch);

    QLOG_DEBUG() << tr("SteamAvatarFetcher initialized");
}

void SteamAvatarFetcher::fetchAvatarUrls(const QString &apiKey, quint64 steamId64)
{
    m_apiKey = apiKey;
    m_steamId64 = steamId64;
    m_retryCount = 0;

    QLOG_DEBUG() << tr("Starting avatar fetch for SteamID: %1").arg(QString::number(steamId64));

    // 立即开始第一次请求
    performFetch();
}

void SteamAvatarFetcher::setRetryInterval(int interval)
{
    if (interval >= 100) { // 最小间隔100毫秒
        m_retryInterval = interval;
        QLOG_DEBUG() << tr("Retry interval set to: %1 ms").arg(interval);
    }
}

void SteamAvatarFetcher::setMaxRetries(int max)
{
    m_maxRetries = qMax(0, max); // 0表示无限重试
    QLOG_DEBUG() << tr("Max retries set to: %1").arg(max == 0 ? tr("unlimited") : QString::number(max));
}

void SteamAvatarFetcher::performFetch()
{
    // 构建API请求URL
    QString url = QString("https://api.steampowered.com/ISteamUser/GetPlayerSummaries/v0002/"
                          "?key=%1&steamids=%2").arg(m_apiKey).arg(m_steamId64);

    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 发送GET请求
    m_manager->get(request);

    // 更新重试计数
    m_retryCount++;
    QLOG_DEBUG() << tr("Fetch attempt #%1").arg(m_retryCount);
    emit retryAttempt(m_retryCount, m_maxRetries);

    // 设置单次请求超时
    QTimer::singleShot(10000, this, [this]() {
        if (m_retryTimer.isActive()) return;

        QString error = tr("Request timed out (10 seconds)");
        QLOG_DEBUG() << error;
        emit fetchError(error, m_retryCount);

        if (m_maxRetries == 0 || m_retryCount < m_maxRetries) {
            m_retryTimer.start(m_retryInterval);
            QLOG_DEBUG() << tr("Scheduling retry in %1 ms").arg(m_retryInterval);
        } else {
            QLOG_DEBUG() << tr("Max retries reached, giving up");
            deleteLater(); // 达到最大重试次数
        }
    });
}

void SteamAvatarFetcher::handleNetworkReply(QNetworkReply *reply)
{
    // 确保处理完成后删除reply对象
    reply->deleteLater();

    // 检查是否取消重试
    if (!m_retryTimer.isActive() && m_retryCount > 0) {
        // 请求成功，停止重试计时器
        m_retryTimer.stop();
    }

    if (reply->error() != QNetworkReply::NoError) {
        // 网络错误
        QString error = tr("Network error: %1").arg(reply->errorString());
        QLOG_DEBUG() << error;
        emit fetchError(error, m_retryCount);

        // 决定是否重试
        if (m_maxRetries == 0 || m_retryCount < m_maxRetries) {
            m_retryTimer.start(m_retryInterval);
            QLOG_DEBUG() << tr("Scheduling retry in %1 ms").arg(m_retryInterval);
        } else {
            QLOG_DEBUG() << tr("Max retries reached, giving up");
            deleteLater(); // 达到最大重试次数
        }
        return;
    }

    // 解析JSON响应
    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (doc.isNull()) {
        QString error = tr("Invalid JSON response");
        QLOG_DEBUG() << error;
        emit fetchError(error, m_retryCount);

        // 决定是否重试
        if (m_maxRetries == 0 || m_retryCount < m_maxRetries) {
            m_retryTimer.start(m_retryInterval);
            QLOG_DEBUG() << tr("Scheduling retry in %1 ms").arg(m_retryInterval);
        } else {
            QLOG_DEBUG() << tr("Max retries reached, giving up");
            deleteLater(); // 达到最大重试次数
        }
        return;
    }

    QJsonObject root = doc.object();
    QJsonObject response = root.value("response").toObject();
    QJsonArray players = response.value("players").toArray();

    if (players.isEmpty()) {
        QString error = tr("Player information not found");
        QLOG_DEBUG() << error;
        emit fetchError(error, m_retryCount);

        // 决定是否重试
        if (m_maxRetries == 0 || m_retryCount < m_maxRetries) {
            m_retryTimer.start(m_retryInterval);
            QLOG_DEBUG() << tr("Scheduling retry in %1 ms").arg(m_retryInterval);
        } else {
            QLOG_DEBUG() << tr("Max retries reached, giving up");
            deleteLater(); // 达到最大重试次数
        }
        return;
    }

    // 提取三种尺寸的头像URL
    QJsonObject player = players.first().toObject();
    QStringList avatarUrls;
    avatarUrls << player.value("avatar").toString();         // 小 (32x32)
    avatarUrls << player.value("avatarmedium").toString();   // 中 (64x64)
    avatarUrls << player.value("avatarfull").toString();     // 大 (184x184)

    // 验证URL是否有效
    if (avatarUrls.isEmpty() || avatarUrls[0].isEmpty()) {
        QString error = tr("Invalid avatar URL received");
        QLOG_DEBUG() << error;
        emit fetchError(error, m_retryCount);

        // 决定是否重试
        if (m_maxRetries == 0 || m_retryCount < m_maxRetries) {
            m_retryTimer.start(m_retryInterval);
            QLOG_DEBUG() << tr("Scheduling retry in %1 ms").arg(m_retryInterval);
        } else {
            QLOG_DEBUG() << tr("Max retries reached, giving up");
            deleteLater(); // 达到最大重试次数
        }
        return;
    }

    QLOG_DEBUG() << tr("Avatar URLs successfully retrieved");
    // QLOG_DEBUG() << tr("Small avatar: %1").arg(avatarUrls[0]);
    // QLOG_DEBUG() << tr("Medium avatar: %1").arg(avatarUrls[1]);
    // QLOG_DEBUG() << tr("Full avatar: %1").arg(avatarUrls[2]);

    // 发射结果信号
    emit avatarUrlsReady(avatarUrls);

    // 自动删除对象
    deleteLater();
}
