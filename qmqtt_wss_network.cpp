/*
 * qmqtt_WssNetwork.cpp - qmqtt WssNetwork
 *
 * Copyright (c) 2013  Ery Lee <ery.lee at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of mqttc nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <QDataStream>
#include <QDebug>
//#include <QLoggingCategory>
#include "qmqtt_wss_network.h"

namespace QMQTT {

//Q_LOGGING_CATEGORY(WssNetwork, "qmqtt.WssNetwork")

WssNetwork::WssNetwork(QObject *parent) :
    QMQTT::AbstractNetwork(parent)
{
    _buffer = new QBuffer(this);
    _offsetBuf = 0;
    _leftSize = 0;
    _autoreconn = false;
    _timeout = 3000;
    _connected = false;
    _buffer->open(QIODevice::ReadWrite);
    initSocket();
}

void WssNetwork::initSocket()
{
    if(_socket) {
        _socket->abort();
        delete _socket;
    }

    _socket = new QtWebsocket::QWsSocket(this, NULL, QtWebsocket::WS_V13);

    QObject::connect(_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
    QObject::connect(_socket, SIGNAL(frameReceived(QByteArray)), this, SLOT(sockReadReady(QByteArray)));
    QObject::connect(_socket, SIGNAL(connected()), this, SLOT(sockConnected()));
    QObject::connect(_socket, SIGNAL(disconnected()), this, SLOT(sockDisconnected()));
    QObject::connect(_socket, SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(displaySslErrors(const QList<QSslError>&)));
}

WssNetwork::~WssNetwork()
{
    disconnect();
}

bool WssNetwork::isConnected()
{
    return _connected;
}

void WssNetwork::connectTo(const QString & host, const quint32 port)
{

    if(!_socket)
    {
        //qCWarning(WssNetwork) << "AMQP: Socket didn't create.";
        return;
    }
    QUrl url(host);
    _host = url.scheme()+"://"+url.host();
    _resource = host.right(host.length()-_host.length());
    _port = port;
    //qCDebug(WssNetwork) << "Connect to" << _host << _port;

    _socket->setProtocol("mqtt");
    //_socket->setExtensions("permessage-deflate; client_max_window_bits");
    _socket->setResourceName(_resource);
    _socket->setOrigin("qmqtt_client");
    _socket->connectToHost(_host, _port);
    //qDebug() << "Connecting..";
}

void WssNetwork::sendFrame(Frame & frame)
{

    if(_socket->state() == QAbstractSocket::ConnectedState)
    {
        QByteArray data;
        QDataStream stream(&data,QIODevice::WriteOnly);
        frame.write(stream);
        //qDebug() << "MQTT TX:" << data.toHex();
        _socket->write(data);
    }

}

void WssNetwork::disconnect()
{
    if(_socket) _socket->disconnectFromHost();
}

QAbstractSocket::SocketState WssNetwork::state() const
{
    if(_socket)
    {
        return _socket->state();
    } else {
        return QAbstractSocket::UnconnectedState;
    }
}

bool WssNetwork::autoReconnect() const
{
    return _autoreconn;
}

void WssNetwork::setAutoReconnect(bool b)
{
    _autoreconn = b;
}

//PRIVATE SLOTS
void WssNetwork::sockConnected()
{
    qDebug() << "WssNetwork connected...";
    _connected = true;
    emit connected();
}

void WssNetwork::sockReadReady(QByteArray data)
{
    QDataStream in(data);
    QDataStream out(_buffer);
    while(!in.atEnd())
    {
        if(_leftSize == 0)
        {
            _leftSize  = 0;
            _offsetBuf = 0;

            in >> _header;
            _leftSize = readRemaingLength(in);
        }
        QByteArray data;
        data.resize(_leftSize);
        _offsetBuf = in.readRawData(data.data(), data.size());
        _leftSize -= _offsetBuf;
        out.writeRawData(data.data(), _offsetBuf);
        if(_leftSize == 0)
        {
            _buffer->reset();
            Frame frame(_header, _buffer->buffer());
            _buffer->buffer().clear();
            //qCDebug(WssNetwork) << "WssNetwork emit received(frame), header: " << _header;
            emit received(frame);
        }
    }
}

/*
 * Read remaining length
 */
int WssNetwork::readRemaingLength(QDataStream &in)
{
     qint8 byte;
     int len = 0;
     int mul = 1;
     do {
         in >> byte;
         len += (byte & 127) * mul;
         mul *= 128  ;
     } while ((byte & 128) != 0);
     return len;
}

void WssNetwork::socketStateChanged(QAbstractSocket::SocketState socketState)
{
    switch (socketState)
    {
        case QAbstractSocket::UnconnectedState:
            qDebug() << "ws:  Unconnected";
            break;
        case QAbstractSocket::HostLookupState:
            qDebug() << "ws:  HostLookup";
            break;
        case QAbstractSocket::ConnectingState:
            qDebug() << "ws:  Connecting";
            break;
        case QAbstractSocket::ConnectedState:
            qDebug() << "ws:  Connected";
            break;
        case QAbstractSocket::BoundState:
            qDebug() << "ws:  Bound";
            break;
        case QAbstractSocket::ClosingState:
            qDebug() << "ws:  Closing";
            break;
        case QAbstractSocket::ListeningState:
            qDebug() << "ws:  Listening";
            break;
        default:
            qDebug() << "ws:  Unknown";
            break;
    }
}

void WssNetwork::displaySslErrors(const QList<QSslError>& errors)
{
    for (int i=0, sz=errors.size(); i<sz; i++)
    {
        QString errorString = errors.at(i).errorString();
        qDebug() << "SSL error: " << errorString;
    }
}

void WssNetwork::sockDisconnected()
{
    _connected = false;
    emit disconnected();
}

} // namespace QMQTT
