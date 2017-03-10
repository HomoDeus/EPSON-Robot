//
// Created by sail on 17-3-10.
//

#ifndef EPSON_ROBOT_ROBOTDRIVER_H
#define EPSON_ROBOT_ROBOTDRIVER_H

#include <Eigen/Geometry>
#include <QTcpSocket>
#include <QDataStream>
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
    RobotDriver();
    Frame_Type GetPosition();

    void connectRobot();
    bool SendCmd(std::string cmd);

private slots:
    void readTCPMessage();
    void TCPError(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket *tcpSocket;
    QDataStream in;
    QDataStream out;
    quint16 blockSize;
    QNetworkSession *networkSession;
    QString IPAddr;
    int PortID;
    QString message;
};


#endif //EPSON_ROBOT_ROBOTDRIVER_H
