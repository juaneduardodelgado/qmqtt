#ifndef QMQTT_ABSTRACT_NETWORK_H
#define QMQTT_ABSTRACT_NETWORK_H

#include <QObject>
#include <QTcpSocket>
#include <QPointer>
#include <QBuffer>
#include <QByteArray>

#include "qmqtt_frame.h"

namespace QMQTT {

class AbstractNetwork: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(AbstractNetwork)
public:
    explicit AbstractNetwork(QObject *parent = 0): QObject(parent){}
    virtual void disconnect() = 0;
    virtual void sendFrame(Frame & frame) = 0;

    virtual bool isConnected() = 0;

    virtual bool autoReconnect() const = 0;
    virtual void setAutoReconnect(bool value) = 0;

public slots:
    virtual void connectTo(const QString & host, quint32 port) = 0;
};

} // namespace QMQTT

#endif // QMQTT_ABSTRACT_NETWORK_H
