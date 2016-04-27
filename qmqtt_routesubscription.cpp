/*
 * qmqtt_router.cpp - qmqtt router
 *
 * Copyright (c) 2013  Ery Lee <ery.lee at gmail dot com>
 * Router added by Niklas Wulf <nwulf at geenen-it-systeme dot de>
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
#include "qmqtt_routesubscription.h"

#include "qmqtt_message.h"
#include "qmqtt_router.h"
#include "qmqtt_routedmessage.h"
#include <QDebug>
//#include <QLoggingCategory>

namespace QMQTT {

//Q_LOGGING_CATEGORY(routerSubscription, "qmqtt.routersubscription")

RouteSubscription::RouteSubscription(Router *parent) : QObject(parent)
{
}

QString RouteSubscription::route() const
{
    return _topic;
}

void RouteSubscription::setRoute(const QString &route)
{
    qDebug() << "Parsing route:" << route;

    QRegExp parameterNamesRegExp("\\:([a-zA-Z0-9]+)"); // note how names must not contain dashes or underscores

    // Remove paramter names to get the actual topic "route"
    QString topic = route;
    topic.replace(parameterNamesRegExp, "");
    qDebug() << "Topic:" << topic;

    // Remove the MQTT wildcards to get a regular expression, which matches the parameters
    QString parameterRegExp = route;
    parameterRegExp
            .replace("+", "")
            .replace(parameterNamesRegExp, "([a-zA-Z0-9_-]+)") // note how parameter values may contain dashes or underscores
            .replace("#", "")
            .replace("$", "\\$");
    qDebug() << "Parameter regexp:" << parameterRegExp;

    // Extract the parameter names
    parameterNamesRegExp.indexIn(route);
    QStringList _names = parameterNamesRegExp.capturedTexts();
    QStringList::iterator it = _names.begin();
    QStringList names;
    while(it != _names.end()) {
        QString parameterName = *it;
        ++it;
        names << parameterName;
    }
    qDebug() << "Parameter names: " << names;

    _topic = topic;
    _parameterNames = names;
    _regularExpression = QRegExp(parameterRegExp);
}

void RouteSubscription::routeMessage(const Message &message)
{
    QString topic = message.topic();
    qDebug() << "Routing topic" << topic;

    int res = _regularExpression.indexIn(topic);
    if(res== -1) {
        qDebug() << "No match";
        return;
    }

    RoutedMessage routedMessage(message);
    qDebug() << "Matching paramters:";

    for(int i = 0, c = _parameterNames.size(); i < c; ++i) {
        QString name = _parameterNames.at(i);
        QString value = _regularExpression.cap(i + 1);
        qDebug() << name << "=" << value;

        routedMessage._parameters.insert(name, value);
    }

    emit received(routedMessage);
}

} // namespace QMQTT
