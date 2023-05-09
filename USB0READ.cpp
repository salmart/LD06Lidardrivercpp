#include <iostream>
#include <string.h>

#include <fcntl.h> //Contains file controls e.g. opening a file, retrieving  and changing permisssions
#include <errno.h> //indicates when something goes wrong
#include <termios.h>//contains definitions used by the terminal I/O interfaces
#include <unistd.h>// write(), read(), close()

int main(){

int serial_port = open("/dev/ttyUSB0",O_RDONLY); //only reads from the port

if (serial_port <0){
  std::cout<< "Error "<<errno<<" from open: "<<strerror(errno)<<std:: endl;
}//if file cannot be accessed errno returns with the type of error

struct termios tty;//creating a new termios struct containing
//cc_t data type is used for terminal special characters
//speed_t data type Used for terminal baud rates
//tcflag_t data type is used for terminal modes

//tcgetattr gets the parameters associated wit the terminal "serial_port"
// and stores them in the termios struct
if (tcgetattr(serial_port, &tty)!= 0){
std::cout <<"Error "<< errno<< " from tcgetattr: "<<strerror(errno)<<std::endl;
}
//Configuring serial port without a GUI
tty.c_cflag &= ~PARENB;// Clear parity bit
tty.c_cflag  &= ~ CSTOPB;// Clear stop field, 1 stop bit is used for the LD06 lidar
tty.c_cflag &= ~CSIZE;// Clear all the size bits, and uses the one below
tty.c_cflag |=CS8;
tty.c_cflag &= ~CRTSCTS;// disables hardware flow control which is what LD06 lidar requires
tty.c_cflag |= CREAD | CLOCAL; //CREAD enables the receiver and CLOCAL Ignores modem status lines
tty.c_lflag &= ~ICANON;//Disables receiving applications receiving data line-by-line
tty.c_lflag &= ~ECHO; //Disable echo
tty.c_lflag &= ~ECHOE;//Disable erasure
tty.c_lflag &= ~ECHONL;//Disable newline echo
tty.c_lflag &= ~ISIG;//Disable interpretation of INTR, QUIT and susp
tty.c_iflag &= ~(IXON | IXOFF | IXANY); //Turn off software flow control
tty.c_iflag &= ~(IGNBRK | BRKINT| PARMRK | ISTRIP| INLCR|IGNCR | ICRNL);//jijijiji
tty.c_oflag &= ~OPOST; //Prevents special interpretaion of output bytes
tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to CRLF
tty.c_cc[VTIME]= 8;//lmaoo DECISECONDS
tty.c_cc[VMIN] = 0;//lmaoo
cfsetispeed(&tty, B230400);//input baud rate speed
//
if (tcsetattr(serial_port, TCSANOW, &tty)!=0){

std::cout << "Error "<<errno<<" from tcsetattr "<<strerror(errno)<<std::endl;

}
//buffer to store data
uint8_t data_buffer[47];

uint16_t numberOfBytes = read(serial_port, &data_buffer, sizeof(data_buffer));

std::cout<<"Read "<< numberOfBytes << " Bytes and Received message: "<< data_buffer<<std::endl;

close(serial_port);


return 0 ;
}