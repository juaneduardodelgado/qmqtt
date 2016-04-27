QMQTT
=====

A Mqtt client implementation.
The project was derived from its original repository to support QT4 projects and to add functionality like WSS connections.
The WSS support was added mostly to use the AWS IoT Broker.


DEPENDENCIES
=====

QT version >= QT4.7

QtWebSockets project for QT4
https://github.com/juaneduardodelgado/QtWebsocket.git

Usage
=====

	#include "qmqtt.h"

	QMQTT::Client *client = new QMQTT::Client("localhost", 1883);

	client->setClientId("clientId");

	client->setUsername("user");

	client->setPassword("password");

	client->connect();

For WSS support use:
	
	QMQTT::Client *client = new QMQTT::Client("wss://mysecure.domain.com/resource-path?with=some&parameters", 443);

Slots
=====

	void connect();

	quint16 publish(Message &message);

	void puback(quint8 type, quint16 msgid);

	quint16 subscribe(const QString &topic, quint8 qos);

	void unsubscribe(const QString &topic);

	void ping();

	void disconnect();

Signals
=======

	void connected();

	void error(QAbstractSocket::SocketError);

	void connacked(quint8 ack);

	void published(Message &message);

	void pubacked(quint8 type, quint16 msgid);

	void received(const Message &message);

	void subscribed(const QString &topic);

	void subacked(quint16 mid, quint8 qos);

	void unsubscribed(const QString &topic);

	void unsubacked(quint16 mid);

	void pong();

	void disconnected();


License
=======

The MIT License (MIT)


Contributors
=============

[@juaneduardodelgado](https://github.com/juaneduardodelgado)


Original Author
======

Feng Lee <feng@emqtt.io>

wuming123057 <huacai123057@163.com>
