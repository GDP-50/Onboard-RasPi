#ifndef MD25Control_H
#define MD25Control_H
#include <unistd.h>			
#include <fcntl.h>			
#include <sys/ioctl.h>		
#include <linux/i2c-dev.h>	
#include <stdio.h>

int setupI2C(int address);
int readI2C(int length);
int writeI2C(int length);
void setMotorSpeed(int speed1, int speed2);
void encoderValues(long* encoder1, long* encoder2);
void resetEncoders();
#endif