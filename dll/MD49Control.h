#ifndef MD24Control_H
#define MD24Control_H
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>

#define BAUDRATE B38400
#define MAX_CURRENT_A 20
void MD49SerialInit();
void writeBytes(int count);
void readBytes(int count);
void driveMotors(char speed1, char speed2);
void setMode(char mode);
void MD49SoftwareVersion();
void resetEncoders();
void setBuffer(int len, char buf[len]);
void readEncoders(long int* encoder1val, long int* encoder2val);
void guardOverCurrent();
void MD49VI();
#endif