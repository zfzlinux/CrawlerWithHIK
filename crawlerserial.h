#ifndef CRAWLERSERIAL_H
#define CRAWLERSERIAL_H

#include <QObject>

class CrawlerSerial : public QObject
{
    Q_OBJECT
public:
    explicit CrawlerSerial(QObject *parent = nullptr);

signals:

public slots:
};

#endif // CRAWLERSERIAL_H