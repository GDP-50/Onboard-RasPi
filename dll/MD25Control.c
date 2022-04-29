#include "MD25Control.h"

int file_i2c;
unsigned char buffer[16] = {0};


int setupI2C(int address) {
    char *filename = (char*)"/dev/i2c-1";
	if ((file_i2c = open(filename, O_RDWR)) < 0)
	{
		printf("Failed to open the i2c bus");
		return -1;
	}
	
	if (ioctl(file_i2c, I2C_SLAVE, address) < 0)
	{
		printf("Failed to acquire bus access or talk to slave.\n");
		return -1;
	}
}

int readI2C(int length) {
	if (read(file_i2c, buffer, length) != length)		
	{
		printf("Failed to read from the i2c bus.\n");
        return -1;
	}
	else
	{
		printf("Data read: %s\n", buffer);
	}
    return 0;
}

int writeI2C(int length) {
    if (write(file_i2c, buffer, length) != length)
	{
		printf("Failed to write to the i2c bus.\n");
        return -1;
	}
}

void setMotorSpeed(int speed1, int speed2) {
    buffer[0] = 0;
    buffer[1] = speed1;
    if(writeI2C(2) == -1) printf("Could not set motor 1 speed\n");
    buffer[0] = 1;
    buffer[1] = speed2;
    if(writeI2C(2) == -1) printf("Could not set motor 2 speed\n");
}

void encoderValues(long* encoder1, long* encoder2) {
    buffer[0] = 2;
    writeI2C(1);
    if(readI2C(8) == -1) {
        printf("Read encoders failed\n");
    } else {
        *encoder1 = (buffer[0] <<24) + (buffer[1] << 16) + (buffer[2] << 8) + buffer[3];	
	    *encoder2 = (buffer[4] <<24) + (buffer[5] << 16) + (buffer[6] << 8) + buffer[7];
    }
}

void resetEncoders(void) {
	buffer[0] = 16;												// Command register
	buffer[1] = 32;												// command to set decoders back to zero
	writeI2C(2);
	printf("Encoders have been reset\n");
}

void setMode(int mode) {
	buffer[0] = 15;
	buffer[1] = mode;
	if(writeI2C(2) == -1) printf("Could not set mode\n");
}

void setI2CSelect() {
	
}