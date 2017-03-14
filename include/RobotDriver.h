//
// Created by sail on 17-3-10.
//

#ifndef EPSON_ROBOT_ROBOTDRIVER_H
#define EPSON_ROBOT_ROBOTDRIVER_H
#include <Eigen/Geometry>
#include <QTcpSocket>
#include <QtNetwork>
#include <QDataStream>
#include <QThread>
#include <boost/thread/thread.hpp>
#include <queue>
#include <memory>


struct Frame_Type{
    Eigen::Vector3d position;
    Eigen::Vector3d rpy;
};

QT_BEGIN_NAMESPACE
class QTcpSocket;
class QNetworkSession;
QT_END_NAMESPACE

class CommandQueue : public QThread {
Q_OBJECT
public:
    CommandQueue(QObject* parent = 0);

    // reimplement run
    void run();

    QString popFront();

private:
    void sendCmd(std::string cmd);
    void executeCmd(std::string cmd);

private:
    std::queue<std::string> m_cmdQueue;
    boost::mutex m_mutex;
};

class NetworkThread : public QThread {
    Q_OBJECT
public:
    NetworkThread(const std::string& address, unsigned short port,
                  const std::shared_ptr<CommandQueue>& cmdQueue,
                  QObject* parent = 0);

    //reimplement run
    void run();

private:
    void connectRobot();

public slots:
    void receivedMessage();
    void receivedError(QAbstractSocket::SocketError socketError);
    void loop();

private:
    std::shared_ptr<QTcpSocket> m_tcpSocket;
    std::string m_address;
    unsigned short m_port;
    std::shared_ptr<CommandQueue> m_cmdQueue;
};

class RobotDriver: public QObject {
    Q_OBJECT
public:
    RobotDriver(QObject *parent=0);
    Frame_Type GetPosition();
    void connectRobot(std::string ipAddress, quint16 portID);
    void run();

public slots:
    void read_message();
    void TCPError(QAbstractSocket::SocketError socketError);

private:
    bool cmdSendLock;//Lock the send cmd process when execution of last command is unfinished.
    void send_command_thread();
    //void receive_message_thread();
    void manager_thread();
    //void TCPError(QAbstractSocket::SocketError socketError);
    void sendCmd(std::string cmd);
    void executeCmd(std::string cmd);
    QTcpSocket *tcpSocket;
    QString robot_ip_address;
    quint16 robot_port_ID;
    QString message;
    boost::thread* manager_thread_; ///< thread for running the main functionaility of the manager
    boost::thread* send_command_thread_;
    boost::thread* receive_message_thread_;
    std::queue<std::string> cmd_queue_;//TODO:change it to array;
};


#endif //EPSON_ROBOT_ROBOTDRIVER_H
