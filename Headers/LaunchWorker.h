#ifndef LAUNCHWORKER_H
#define LAUNCHWORKER_H

#include <QObject>
#include <QRunnable>

class LaunchWorker : public QObject, public QRunnable {
    Q_OBJECT
public:
    explicit LaunchWorker(QObject *parent = nullptr);
    void run() override;

signals:
    void progressUpdated(int value);
    void finished();
};

#endif // LAUNCHWORKER_H