QMQTT
=====

mqtt client for Qt with support for WSS connections. Tested with the AWS IoT broker.


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
