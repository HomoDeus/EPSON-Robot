//
// Created by sail on 17-3-10.
//
#include "RobotDriver.h"

RobotDriver::RobotDriver(QObject *parent)
        :QObject(parent){
    //TODO IP and port is written here directly. It will be removed to config file later.
    robot_ip_address="192.168.10.70";
    robot_port_ID=6000;
    tcpSocket = new QTcpSocket(this);

    connect(tcpSocket, &QTcpSocket::readyRead, this, &RobotDriver::readTCPMessage);
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(TCPError(QAbstractSocket::SocketError)));
}

bool RobotDriver::SendCmd(std::string cmd){
    tcpSocket->write("$login");
    tcpSocket->write("How are you?");
}

void RobotDriver::readTCPMessage(){
    message=tcpSocket->readAll();
    qDebug()<<"Receive a message from TCP:"<<message;
}

Frame_Type RobotDriver::GetPosition(){
    Frame_Type frame;
    frame.position=Eigen::Vector3d(0,0,0);
    frame.rpy=Eigen::Vector3d(0,0,0);
    return frame;
}

void RobotDriver::connectRobot(){
    tcpSocket->abort();
    tcpSocket->connectToHost(robot_ip_address,robot_port_ID);
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