//
// Created by sail on 17-3-9.
//
#include "filesender.h"
#include "RobotDriver.h"
#include "client.h"
#include <QApplication>
int main(int argc, char * argv[]){
    QApplication app(argc,argv);
//    FileSender fileSender;
//    fileSender.receivefile();
//    fileSender.sendfile();
//    RobotDriver epson;
//    epson.connectRobot();
    Client client;
    client.show();
    return app.exec();
}
