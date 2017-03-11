//
// Created by sail on 17-3-10.
//

#ifndef EPSON_ROBOT_ROBOTDRIVER_H
#define EPSON_ROBOT_ROBOTDRIVER_H
#include <Eigen/Geometry>
#include <QTcpSocket>
#include <QtNetwork>
#include <QDataStream>
#include <boost/thread/thread.hpp>
#include <queue>
struct Frame_Type{
    Eigen::Vector3d position;
    Eigen::Vector3d rpy;
};

QT_BEGIN_NAMESPACE
class QTcpSocket;
class QNetworkSession;
QT_END_NAMESPACE

class RobotDriver: public QObject {
    Q_OBJECT
public:
    RobotDriver(QObject *parent=0);
    Frame_Type GetPosition();
    void connectRobot(std::string ipAddress, quint16 portID);
    void run();

private slots:
    void read_message();
    void TCPError(QAbstractSocket::SocketError socketError);

private:
    bool cmdSendLock;//Lock the send cmd process when execution of last command is unfinished.
    void send_command_thread();
    //void receive_message_thread();
    void manager_thread();
    //void TCPError(QAbstractSocket::SocketError socketError);
    void sendCmd(std::string cmd);
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
