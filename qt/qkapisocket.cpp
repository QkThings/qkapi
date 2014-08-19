#include "qkapisocket.h"

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
