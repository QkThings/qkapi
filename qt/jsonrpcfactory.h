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
