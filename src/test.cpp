//
// Created by sail on 17-3-9.
//
#include "filesender.h"
#include <iostream>
using namespace std;
int main(){
    std::cout<<"transfer file through tcp/ip";
    FileSender fileSender;
    fileSender.receivefile();
    fileSender.sendfile();
    return 0;
}
