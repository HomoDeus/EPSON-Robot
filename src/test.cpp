//
// Created by sail on 17-3-9.
//
#include <QApplication>
#include <QtCore>

#include "filesender.h"
#include "RobotDriver.h"
#include <stdlib.h>
#include "client.h"
#include "server.h"

int main(int argc, char * argv[]){
    QApplication app(argc,argv);
    bool clientmode=false;
    if(clientmode){
        Client client;
        client.show();
    }else{
        Server server;
        server.show();
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    }
//    FileSender fileSender;
//    fileSender.receivefile();
//    fileSender.sendfile();
//    RobotDriver epson;
//    epson.connectRobot();

    return app.exec();
}
