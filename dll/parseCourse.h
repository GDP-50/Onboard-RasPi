#ifndef PARSECOURSE_H
#define PARSECOURSE_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
typedef enum { false = 0, true = !false } bool;
void loadCourse(const char * path, int* greenSize, int* bunkerCount, int bunkerSizes[128]);
void prepareCourse(const char * path, int greenSize, double green[greenSize][3], int bunkerCount, int bunkerMaxSize, double bunkers[bunkerCount][bunkerMaxSize][3], int bunkerSizes[128], double teePos[3], double* scaleVal);
#endif