//
// Created by sail on 17-3-10.
//
#include "RobotDriver.h"

RobotDriver::RobotDriver(QObject *parent)
        :QObject(parent){
    //TODO IP and port is written here directly. It will be removed to config file later.
    cmdSendLock= false;
    tcpSocket = new QTcpSocket(this);

    connect(tcpSocket, &QTcpSocket::readyRead, this, &RobotDriver::read_message);
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(TCPError(QAbstractSocket::SocketError)));
}
void RobotDriver::manager_thread(){
    sendCmd("login");
    sendCmd("getStatus1");
    sendCmd("getStatus2");
    sendCmd("getStatus3");
}
void RobotDriver::sendCmd(std::string cmd){
    cmd_queue_.push(cmd);
}
void RobotDriver::run(){
    qDebug()<<"Starting manager thread..";
    this->manager_thread_ =
        new boost::thread(boost::bind(&RobotDriver::manager_thread, this));
    this->send_command_thread_=
        new boost::thread(boost::bind(&RobotDriver::send_command_thread,this));
}
void RobotDriver::send_command_thread(){
    while(1){
        if(!cmd_queue_.empty()&&!cmdSendLock){
            cmdSendLock=true;
            QString package("$");
            package.append(cmd_queue_.front().data()).append("\n\n");
            cmd_queue_.pop();
            qDebug() << "package" << package;
            tcpSocket->write(package.toStdString().data());
        }
    }
}

void RobotDriver::read_message(){
    message=tcpSocket->readAll();
    qDebug()<<"Receive a message from TCP:"<<message;
    cmdSendLock=false;
}

Frame_Type RobotDriver::GetPosition(){
    Frame_Type frame;
    frame.position=Eigen::Vector3d(0,0,0);
    frame.rpy=Eigen::Vector3d(0,0,0);
    return frame;
}

void RobotDriver::connectRobot(std::string ipAddress, quint16 portID){
    robot_ip_address=ipAddress.data();
    qDebug()<<"robot_ip_address: "<<robot_ip_address;
    robot_port_ID=portID;
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