#include "MD49Control.h"

char buffer[16];

int fd;

union encoder {										// To store the encoder values
	int value;
	char bytes[8];
} encoder1, encoder2;

void MD49SerialInit() {
    char *portName = "/dev/tySS0";
    struct termios options;	

    fd = open(portName, O_RDWR | O_NOCTTY);				// Open port for read and write not making it a controlling terminal
	if (fd == -1)
	{
		perror("openPort: Unable to open port ");		// If open() returns an error
	} 
	tcgetattr(fd, &options);
	cfsetispeed(&options, BAUDRATE);						// Set baud rate
	cfsetospeed(&options, BAUDRATE);					
	cfmakeraw(&options);
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &options);
    printf("Successfully opened port for serial comms\n");
}

void writeBytes(int count) {
	if ((write(fd, buffer, count)) == -1) {							// Send data out	
		perror("Error writing");
		close(fd);															// Close port if there is an error
		exit(1);
	}
}

void readBytes(int count) {
	if (read(fd, buffer, count) == -1) {								// Read back data into buffer
		perror("Error reading ");
		close(fd);															// Close port if there is an error
		exit(1);
	}
}

void driveMotors(char speed1, char speed2) {
    buffer[0] = 0;
	buffer[1] = 0x31;															// Command to set motor speed
	buffer[2] = speed1;	
	writeBytes(3);											
	usleep(10000);
	
	buffer[0] = 0;
	buffer[1] = 0x32;
	buffer[2] = speed2;
	
	writeBytes(3);
    printf("Motor speeds set to %d and %d\n", speed1, speed2);
}

void setMode(char mode) {
    buffer[0] = 0;
	buffer[1] = 0x34;															// Command to set mode
	buffer[2] = mode; 														// Mode we wish to set
	
	writeBytes(3);
    printf("MD49 mode set to %d\n", mode);
}

void MD49SoftwareVersion() {
    buffer[0] = 0;															
	buffer[1] = 0x29;		
    writeBytes(2);
	readBytes(1);
	printf("MD49 Software v: %d \n",buffer[0]);
}

void resetEncoders() {
	
	buffer[0] = 0;
	buffer[1] = 0x35;		
	
	writeBytes(2);
    printf("Encoders reset\n");
}

void setBuffer(int len, char buf[len]) {
    for(int i = 0; i < len; i++) {
        buffer[i] = buf[i];
    }
}

void readEncoders(long int* encoder1val, long int* encoder2val) {
    	
	encoder1.value = 0;
	encoder2.value = 0;
		
	buffer[0] = 0;
	buffer[1] = 0x25;															// Command to return encoder values
	
	writeBytes(2);
	readBytes(8);
	
	encoder1.bytes[0] = buffer[3];
	encoder1.bytes[1] = buffer[2];
	encoder1.bytes[2] = buffer[1];
	encoder1.bytes[3] = buffer[0];
	
	printf("\rencoder 1 : %08X ", encoder1.value);									// Display it
    *encoder1val = encoder1.value;
	
	encoder2.bytes[0] = buffer[7];
	encoder2.bytes[1] = buffer[6];
	encoder2.bytes[2] = buffer[5];
	encoder2.bytes[3] = buffer[4];
	
	printf("encoder 2 : %08X   ", encoder2.value);
    *encoder2val = encoder2.value;
	
	fflush(stdout);									
}