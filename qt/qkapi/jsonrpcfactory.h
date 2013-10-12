#ifndef JSONRPCFACTORY_H
#define JSONRPCFACTORY_H

#include <QObject>
#include "qkapi_global.h"

class JsonRpcFactory : public QObject
{
    Q_OBJECT
public:    
    enum Error
    {
        ParseError = -32700,
        InvalidRequest = -32600,
        MethodNotFound = -32601,
        InvalidParams = -32602,
        InternalError = -32603
    };

    static QString versionStr;

    static QString notification(const QString &method, const QList<QVariant> &params);
    static QString call(const QString &method, const QString &str, int id = 1);
    static QString call(const QString &method, const QList<QVariant> &params, int id = 1);
    static QString call(const QString &method, const QMap<QString,QVariant> &params, int id = 1);
    static QString error(int code, const QString &message, int id);
    static QString batch(const QStringList &requests);

private:
    enum RequestType
    {
        rtNotification,
        rtCall
    };
    static QString _createRequest(const QString &method, const QString &str, RequestType type, int id = 1);
    static QString _createRequest(const QString &method, const QList<QVariant> &params, RequestType type, int id = 1);
    static QString _createRequest(const QString &method, const QMap<QString,QVariant> &params, RequestType type, int id = 1);

};

#endif // JSONRPCFACTORY_H
