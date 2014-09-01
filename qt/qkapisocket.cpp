/*
 * QkThings LICENSE
 * The open source framework and modular platform for smart devices.
 * Copyright (C) 2014 <http://qkthings.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QDebug>

QkAPISocket::QkAPISocket(QObject *parent) :
    QTcpSocket(parent)
{
    m_parseDepthLevel = 0;
    _setupConnections();
}

void QkAPISocket::_setupConnections()
{
    connect(this, SIGNAL(readyRead()), this, SLOT(_handleReadyRead()));
}

void QkAPISocket::_handleReadyRead()
{
    while(bytesAvailable() > 0)
    {
        QByteArray data = readAll();
        if(_parseIncomingData(data))
        {
            emit parsedJsonString(m_jsonStr);
            m_jsonStr.clear();
        }
    }
}

bool QkAPISocket::_parseIncomingData(const QByteArray &data)
{
    bool done = false;
    int i;
    char c;
    for(i = 0; i < data.count(); i++)
    {
        c = data.at(i);
        if(c == '{')
            m_parseDepthLevel++;
        else if(c == '}')
        {
            m_parseDepthLevel--;
            if(m_parseDepthLevel == 0)
                done = true;
        }
        m_jsonStr.append(c);
    }
    return done;
}
