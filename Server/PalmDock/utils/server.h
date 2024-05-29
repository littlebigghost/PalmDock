#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>

class Server : public QObject
{
    Q_OBJECT

private:
    int m_port;

    QTcpServer *m_server = nullptr;
    QTcpSocket *m_socket = nullptr;

    bool m_is_start = false;

public:
    Server();
    ~Server();
    bool isStart();
    void initServer(int port);
    void startServer();
    void stopServer();
    void sendData(QString data);
    int getPort();

signals:
    void newConnect();
    void receiveData(QString data);
};

#endif // SERVER_H
