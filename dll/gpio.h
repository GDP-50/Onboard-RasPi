#ifndef GPIO_H
#define GPIO_H
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define IN  0
#define OUT 1

#define LOW  0
#define HIGH 1

#define BUFFER_MAX 3
#define VALUE_MAX 30
#define DIRECTION_MAX 35

static int pinEnable(int pin);
static int pinDisable(int pin);
static int pinMode(int pin, int dir);
static int digitalRead(int pin);
static int digitalWrite(int pin, int value);
#endif
