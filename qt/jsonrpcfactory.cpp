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

#include "jsonrpcfactory.h"
#include "QVariant"

QString JsonRpcFactory::notification(const QString &method, const QList<QVariant> &params)
{
    return _createRequest(method, params, rtNotification);
}

QString JsonRpcFactory::call(const QString &method, const QString &str, int id)
{
    return _createRequest(method, str, rtCall, id);
}

QString JsonRpcFactory::call(const QString &method, const QList<QVariant> &params, int id)
{
    return _createRequest(method, params, rtCall, id);
}

QString JsonRpcFactory::call(const QString &method, const QMap<QString, QVariant> &params, int id)
{
    return _createRequest(method, params, rtCall, id);
}

QString JsonRpcFactory::error(int code, const QString &message, int id)
{
    QString str;
    str.append("{\"jsonrpc\":\"2.0\",");
    str.append("\"error\":{");
    str.append("\"code\":" + QString::number(code) + ",");
    str.append("\"message\":\"" + message + "\"},");
    str.append("\"id\":null");
    str.append("}");
    return str;
}

QString JsonRpcFactory::batch(const QStringList &requests)
{
    QString str;
    str.append("{");
    str.append("}");
    return str;
}

QString JsonRpcFactory::_createRequest(const QString &method, const QList<QVariant> &params, RequestType type, int id)
{
    int i;
    QString request;
    request.append("{\"jsonrpc\":\"2.0\",");
    request.append("\"method\":");
    request.append("\"" + method + "\",");
    request.append("\"params\":");
    request.append("[");
    for(i = 0; i < params.count(); i++)
    {
        QVariant param = params[i];
        switch(param.type())
        {
        case QVariant::String:
            request.append("\"" + param.toString() + "\"");
            break;
        case QVariant::Int:
            request.append(QString::number(param.toInt()));
            break;
        case QVariant::Double:
            request.append(QString::number(param.toDouble()));
            break;
        default:
            return QString();
        }
        if(i < params.count() - 1)
            request.append(",");
    }
    request.append("]");
    if(type == rtCall)
    {
        request.append(",\"id\":");
        request.append(QString::number(id));
    }
    request.append("}");
    return request;
}

QString JsonRpcFactory::_createRequest(const QString &method, const QMap<QString, QVariant> &params, RequestType type, int id)
{
    QString request;
    request.append("{\"jsonrpc\":\"2.0\",");
    request.append("\"method\":");
    request.append("\"" + method + "\",");
    request.append("\"params\":");
    request.append("[");
    request.append("]");
    if(type == rtCall)
    {
        request.append(",\"id\":");
        request.append(QString::number(id));
    }
    request.append("}");
    return request;
}

QString JsonRpcFactory::_createRequest(const QString &method, const QString &str, RequestType type, int id)
{
    QString request;
    request.append("{\"jsonrpc\":\"2.0\",");
    request.append("\"method\":");
    request.append("\"" + method + "\",");
    request.append("\"params\":");
    request.append("\"" + str + "\"");
    if(type == rtCall)
    {
        request.append(",\"id\":");
        request.append(QString::number(id));
    }
    request.append("}");
    return request;
}

