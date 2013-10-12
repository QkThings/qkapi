#include "qkapi.h"
#include "qkapisocket.h"
#include "jsonrpcfactory.h"

#include <QDebug>
#include <QFile>
#include <QVector>

QkAPI::QkAPI(QObject *parent) :
    QObject(parent)
{
    m_socket = new QkAPISocket(this);
    _parseTree();
}

QkAPI::~QkAPI()
{
    delete m_socket;
}

void QkAPI::connectToHost(const QString &hostName, int port)
{
    if(!isConnected())
        m_socket->connectToHost(hostName, port);
}

void QkAPI::disconnectFromHost()
{
    m_socket->disconnectFromHost();
}

bool QkAPI::isConnected()
{
    return !(m_socket->state() == QkAPISocket::UnconnectedState);
}

QkAPI::Error QkAPI::send(MethodType methodType, QString uri)
{
    //if(!_validateURI(uri))
      //  return errInvalidURI;

    QString methodName;
    switch(methodType)
    {
    case mtGET: methodName = "GET"; break;
    case mtPOST: methodName = "POST"; break;
    case mtPUT: methodName = "PUT"; break;
    case mtDELETE: methodName = "DELETE"; break;
    }

    QString request = JsonRpcFactory::call(methodName, uri);
    m_socket->write(request.toUtf8());

    return OK;
}

bool QkAPI::_validateURI(const QString &uri)
{
    return true;
}

void QkAPI::_parseTree()
{
    QFile file(":/apitree.txt");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
       qDebug() << "Unable to read apitree.txt";
       return;
    }

    QTextStream in(&file);

    QString line;
    int level, maxLevel = 0;

    while(!in.atEnd())
    {
        line = in.readLine();
        level = line.count('\t');
        if(level > maxLevel)
            maxLevel = level;
    }

    _APITreeItem *treeItem, *parentItem;
    QVector<_APITreeItem*> parentArray(maxLevel);
    in.seek(0);
    while(!in.atEnd())
    {
        line = in.readLine();

        if(line.at(0) == '#') // comment
            continue;

        level = line.count('\t');

        QString name = line.remove(' ');
        name = name.remove('\\');
        name = name.remove('\t');
        name = name.remove('\n');

        if(level == 0)
        {
            if(m_apiTreeRoot == 0)
            {
                parentItem = new _APITreeItem(name);
                m_apiTreeRoot = parentItem;
            }
            else
            {
                qDebug() << "ERROR: multiple items at level 0";
                return;
            }
        }
        else
        {
            parentItem = parentArray.at(level - 1);
            if(parentItem == 0)
                qDebug() << "ERROR: parent is null";
            treeItem = new _APITreeItem(name, parentItem);
            parentItem->children.append(treeItem);
        }

        parentArray[level] = parentItem;
        qDebug() << level << name;
    }
}




QkAPI::_APITreeItem::_APITreeItem(QString name, _APITreeItem *parent)
{
    this->parent = parent;
    this->name = name;
}


