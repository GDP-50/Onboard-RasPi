#ifndef CONTROL_H
#define CONTROL_H
#include <stdlib.h>
#include <math.h>
#include "polygons.h"
#include "MD49Control.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#define RADIUS_EARTH 6371000.0
#define MAX_ERR 1.0
#define WHEEL_BASE_M 0.6
#define WHEEL_RADIUS_M 0.115
void caddyControl(double golferPos[3], double caddyPos[3], double caddyRotation);
void reachTarget(double caddyPos[3], double caddyRotation);
double meters2deg(double meters);
void pathFind(int polyIdx, double caddyPos[3], double targetPos[3]);
void matrixVectorMultiplication(int n, double vec[n], double** matrix, double outVec[n]);
double** gen3x3RotationMatrix_Z(double angle);
double vecNmag(int n, double vec[n]);
double angleBetweenVecN(int n, double vec1[3], double vec2[3]);
double deg2rad(double deg);
void translateCaddy(double dist);
void rotateCaddy(double theta);
#endif