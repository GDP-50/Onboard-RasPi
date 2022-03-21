#ifndef POLYGONS_H
#define POLYGONS_H
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
typedef enum { false = 0, true = !false } bool;
void polygonCentroid(int polygonSize, double polygon[polygonSize][3], double pos[2]);
void minMax(double* v1, double* v2, double* min, double* max);
void Max(double* v1, double* v2, double* max);
void Min(double* v1, double* v2, double* min);
bool pointInPolygon(int polygonSize, double polygon[polygonSize][3], double pos[3]);
void rayCast(int polygonSize, double polygon[polygonSize][3], int* intersections, int idxI, int idxIp1, double px, double py);
bool vecPolygonIntersect(double point1[3], double point2[3], int polygonSize, double polygon[polygonSize][3]);
bool segmentIntersection(double point1[3], double point2[3], int polygonSize, double polygon[polygonSize][3], int idxI, int idxIp1);
#endif