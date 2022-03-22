#ifndef POLYGONS_H
#define POLYGONS_H
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
typedef enum { false = 0, true = !false } bool;
extern int polyCount;
extern double*** polygons;
extern double** polyInfo;
void loadPolygons(int polygonCount, int polygonMaxSize, double py_polygons[polygonCount][polygonMaxSize][3], int polySizes[polygonCount]);
void polygonCentroid(int polygonIdx);
void minMax(double* v1, double* v2, double* min, double* max);
void Max(double* v1, double* v2, double* max);
void Min(double* v1, double* v2, double* min);
bool pointInPolygon(int polyIdx, double pos[3], double** rpolygon);
void rayCast(int polyIdx, int* intersections, int idxI, int idxIp1, double px, double py, double** rpolygon);
bool vecPolygonIntersect(int polyIdx, double point1[3], double point2[3], double** rpolygon);
bool segmentIntersection(int polyIdx, double point1[3], double point2[3], int idxI, int idxIp1, double** rpolygon);
#endif