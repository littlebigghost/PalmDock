#include "server.h"
#include <QDebug>

Server::Server() {}

Server::~Server() {
    if (m_socket != nullptr)
        m_socket->disconnectFromHost();
    if (m_server != nullptr)
        m_server->close();
    delete m_socket;
    delete m_server;
}

bool Server::isStart() {
    return m_is_start;
}

void Server::initServer(int port){
    if (m_server != nullptr)
        return;
    m_server = new QTcpServer();
    m_port = port;
}

void Server::startServer(){
    if (m_server == nullptr)
        return;
    m_is_start = !m_is_start;
    m_server->listen(QHostAddress::Any, m_port);
    connect(m_server, &QTcpServer::newConnection, [=](){
        emit newConnect();
        // 获取到新的客户端连接
        m_socket = m_server->nextPendingConnection();
        qDebug() << "客户端连接到服务器";

        // 处理与客户端的通讯
        connect(m_socket, &QTcpSocket::readyRead, [=](){
            QByteArray data = m_socket->readAll();
            // 处理接收到的数据
            qDebug() << "服务器接收到消息";
            emit receiveData(QString::fromUtf8(data));
        });
        // 当客户端断开时，进行清理
        connect(m_socket, &QTcpSocket::disconnected, [=](){
            qDebug() << "客户端断开连接";
            m_socket->deleteLater();
            m_socket = nullptr;
        });
    });
}

void Server::stopServer() {
    if (m_server == nullptr)
        return;
    m_is_start = !m_is_start;
    m_server->close();
}

void Server::sendData(QString data){
    if (m_server == nullptr)
        return;
    if (m_socket != nullptr && m_socket->state() == QAbstractSocket::ConnectedState) {
        qDebug() << "test";
        m_socket->write((data + "\n").toUtf8());
        m_socket->flush();
        qDebug() << "服务端成功发送数据：";
    }
}

int Server::getPort() {
    if (m_server == nullptr)
        return -1;
    return m_server->serverPort();
}
