//
// Created by sail on 17-3-9.
//
#include "filesender.h"
#include <QFile>
#include <QDebug>
#include <QDir>
void FileSender::sendfile() {
    qDebug()<<"The file is sent!"<<endl;
}
void FileSender::receivefile() {
    qDebug()<<"The file received!"<<endl;
}