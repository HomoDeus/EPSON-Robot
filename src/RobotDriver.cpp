//
// Created by sail on 17-3-10.
//
#include "RobotDriver.h"
#include <QTcpSocket>
RobotDriver::RobotDriver(QObject *parent):networkSession(0){
    IPAddr="192.168.10.85";
    PortID=33006;
    in.setDevice(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readTCPMessage()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(TCPError(QAbstractSocket::SocketError)));
}

bool RobotDriver::SendCmd(std::string cmd){

}

void RobotDriver::readTCPMessage(){
    if (blockSize == 0) {
        if (tcpSocket->bytesAvailable() < (int)sizeof(quint16))
            return;
        in >> blockSize;
    }

    if (tcpSocket->bytesAvailable() < blockSize)
        return;

    QString msg;
    in >> msg;
    message = msg;
}

Frame_Type RobotDriver::GetPosition(){
    Frame_Type frame;
    frame.position=Eigen::Vector3d(0,0,0);
    frame.rpy=Eigen::Vector3d(0,0,0);
    return frame;
}

void RobotDriver::connectRobot(){
    blockSize = 0;
    tcpSocket->abort();
    tcpSocket->connectToHost(IPAddr,PortID);
}

void RobotDriver::TCPError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
            break;
        case QAbstractSocket::HostNotFoundError:
            qDebug()<<"The host was not found. Please check the host name and port settings.";
            break;
        case QAbstractSocket::ConnectionRefusedError:
            qDebug()<<"The connection was refused by the peer. Make sure the fortune server is running,"
                    " and check that the host name and port settings are correct.";
            break;
        default:
            qDebug()<<QString("The following error occurred: %1.").arg(tcpSocket->errorString());
    }
}