/*
 * qmqtt_will.cpp - qmqtt will
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
#include "qmqtt_will.h"

namespace QMQTT {

Will::Will(const QString & topic, const QString & msg, quint8 qos, bool retain, QObject * parent) :
    QObject(parent),
    _topic(topic),
    _message(msg),
    _qos(qos),
    _retain(retain)
{
}

Will::~Will()
{
    //NOTHING TODO;
}

quint8 Will::qos()
{
    return _qos;
}

void Will::setQos(quint8 qos)
{
    _qos = qos;
}

bool Will::retain()
{
    return _retain;
}

void Will::setRetain(bool retain)
{
    _retain = retain;
}

QString Will::topic() const
{
    return _topic;
}

void Will::setTopic(const QString & topic)
{
    _topic = topic;
}

QString Will::message() const
{
    return _message;
}

void Will::setMessage(const QString & message)
{
    _message = message;
}

} // namespace QMQTT
