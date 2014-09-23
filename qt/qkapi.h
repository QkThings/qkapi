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
