#include "arduino.h"

arduino::arduino(std::string path){
        address = path;
        //bufIO=(char*)malloc(256*sizeof(char));
        //memset(bufIO, '\0', 256*sizeof(char));
        bufIO= new std::stringstream();
        receivedflag=0;
        waiting=0;
        verbose = 0;
}

int arduino::init(){
        fd = open (address.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
        usleep(100);
        if (fd < 0)
        {
                return 0;
                printf ("error %d opening %s: %s", errno, address.c_str(), strerror (errno));
        }
        set_interface_attribs (B9600, 0);
        set_blocking (1);
        startcontreadout();
        usleep(500);
        return 1;
}

void arduino::closeconn(){
        set_blocking(0);
        stopcontreadout();
        close(fd);
}


int arduino::set_interface_attribs (int speed, int parity)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                printf ("error %d from tcgetattr", errno);
                return 0;
        }

        cfsetospeed (&tty, speed);
        cfsetispeed (&tty, speed);

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        tty.c_iflag &= ~IGNBRK;         // disable break processing
        tty.c_lflag = 0;                // no signaling chars, no echo,
                                        // no canonical processing
        tty.c_oflag = 0;                // no remapping, no delays
        tty.c_cc[VMIN]  = 0;            // read doesn't block
        tty.c_cc[VTIME] = 3;            // 0.5 seconds read timeout

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                        // enable reading
        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
        tty.c_cflag |= parity;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CRTSCTS;

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
        {
                printf ("error %d from tcsetattr", errno);
                return 0;
        }
        return 1;
}

void arduino::set_blocking (int should_block)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                printf ("error %d from tggetattr", errno);
                return;
        }

        tty.c_cc[VMIN]  = should_block ? 1 : 0;
        tty.c_cc[VTIME] = 0;            // 0.5 seconds read timeout

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
                printf ("error %d setting term attributes", errno);
}

void arduino::send(std::string command)
{
        write(fd, command.c_str(), command.length());
}

std::string arduino::query(std::string command)
{
        receivedflag=0;
        send(command);
        waiting=1;
        while(!receivedflag){/*wait until something is received*/}
        std::string a = bufIO->str();
        waiting=0;
        return a;
}

void arduino::continousreadout()
{       char tempchar;
        tempchar = '\n';
	while(contreadout){
                bufIO->str(std::string());
                tcflush(fd, TCIFLUSH);
                lseek(fd, 0, SEEK_END);
                do{
		      read (fd, &tempchar, 1);
                      *bufIO << tempchar;
                } while (tempchar != '\n');
                std::cout << "DEBUG-OUT " << bufIO->str() << std::endl;
                receivedflag=1;
                while(waiting){}
                if (verbose) {usleep(100);std::cout << bufIO->str();}
        }
}
