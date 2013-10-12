#ifndef QKAPISOCKET_H
#define QKAPISOCKET_H

#include "qkapi_global.h"
#include <QTcpSocket>

class QKAPISHARED_EXPORT QkAPISocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit QkAPISocket(QObject *parent = 0);
    
signals:
    void parsedJsonString(QString str);
    
public slots:


private slots:
    void _handleReadyRead();
    
protected:
    QString m_jsonStr;
    int m_parseDepthLevel;

private:
    void _setupConnections();
    bool _parseIncomingData(const QByteArray &data);

};

#endif // QKAPISOCKET_H
