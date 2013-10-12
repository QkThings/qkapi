#ifndef QKAPI_H
#define QKAPI_H

#include <QObject>
#include "qkapi_global.h"

class QkAPISocket;

class QKAPISHARED_EXPORT QkAPI : public QObject
{
    Q_OBJECT
public:
    enum MethodType
    {
        mtGET,
        mtPOST,
        mtPUT,
        mtDELETE
    };
    enum Error
    {
        OK = 0,
        errInvalidURI
    };


    explicit QkAPI(QObject *parent = 0);
    ~QkAPI();
    void connectToHost(const QString &hostName, int port);
    void disconnectFromHost();
    bool isConnected();

    Error send(MethodType methodType, QString uri);


private:
    class _APITreeItem
    {
    public:
        _APITreeItem(QString name, _APITreeItem *parent = 0);
        _APITreeItem *parent;
        QList<_APITreeItem*> children;
        QString name;
    };

    bool _validateURI(const QString &uri);
    void _parseTree();
    void _createWrapper();

    _APITreeItem *m_apiTreeRoot;

    QkAPISocket *m_socket;
};

#endif // QKAPI_H
