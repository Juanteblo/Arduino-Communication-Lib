#ifndef ARDUINO_H
#define ARDUINO_H

#include <errno.h>
#include <termios.h>
#include <unistd.h>

#include <stdio.h>    // Standard input/output definitions
#include <fcntl.h>    // File control definitions 
#include <string.h>   // String function definitions 
#include <errno.h>    // Error number definitions 
#include <iostream>

#include <sstream>
#include <thread>

class arduino{
public:
        arduino(std::string path);
        void send(std::string command);
        std::string query(std::string command);
        void startcontreadout(){contreadout=1; ArduinoReadout = new std::thread(&arduino::continousreadout, this);}
        void stopcontreadout(){contreadout=0;}
        void closeconn();
        int init();
        bool verbose; // prints every serial input
        std::string address;
private:
        int fd;
        std::stringstream *bufIO;
        bool contreadout;
        bool receivedflag;
        bool waiting;
        std::thread *ArduinoReadout;
        int set_interface_attribs (int speed, int parity);
        void set_blocking (int should_block);
        void continousreadout();
};


#endif
