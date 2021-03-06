//
// Created by sail on 17-3-10.
//
#include "RobotDriver.h"

RobotDriver::RobotDriver(QObject *parent)
        :QObject(parent){
    //TODO IP and port is written here directly. It will be removed to config file later.
    cmdSendLock= false;
    tcpSocket = new QTcpSocket();
//    tcpSocket->connect(&QTcpSocket::readyRead,this,&RobotDriver::read_message);
//    tcpSocket->setParent(0);
//    // before
//    tcpSocket->moveToThread( tcpSocketThread );
    connect(tcpSocket, &QTcpSocket::readyRead, this, &RobotDriver::read_message);
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(TCPError(QAbstractSocket::SocketError)));
}
void RobotDriver::manager_thread(){
    sendCmd("login");
    executeCmd("motor off");
    sleep(5);
    executeCmd("motor on");
    sleep(5);
    executeCmd("power high");
    sleep(1);
    executeCmd("accel 80,80");
    sleep(1);
    while(1){
        sendCmd("reset");
        executeCmd("power high");
        sleep(0.2);
        executeCmd("speed 50");
        sleep(0.3);
        executeCmd("accel 50,60");
        sleep(1);
        executeCmd("go p1");
        executeCmd("?speed");
        sleep(1);
        executeCmd("go p3");
        sleep(1);
        executeCmd("go p2");
        sleep(1);
        executeCmd("go p0");
        sleep(1);
        executeCmd("go p2");
        sleep(1);
        executeCmd("go p0");
        sleep(1);
    }
}
void RobotDriver::sendCmd(std::string cmd){
    cmd_queue_.push(cmd);
}
void RobotDriver::executeCmd(std::string cmd){
//    sendCmd("reset");
    sendCmd("getstatus");
    std::string executecmd("execute,\"");
    executecmd.append(cmd).append("\"");;
    sendCmd(executecmd);
}
void RobotDriver::run(){
    qDebug()<<"Starting manager thread..";
    this->manager_thread_ =
        new boost::thread(boost::bind(&RobotDriver::manager_thread, this));
    this->send_command_thread_=
        new boost::thread(boost::bind(&RobotDriver::send_command_thread,this));
//    send_command_thread();
}
void RobotDriver::send_command_thread(){

    while(1){
        if(!cmd_queue_.empty()&&!cmdSendLock){
            cmdSendLock=true;
            QString package("$");
            package.append(cmd_queue_.front().data()).append("\t\n");
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