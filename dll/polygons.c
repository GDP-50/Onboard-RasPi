#include "polygons.h"

int polyCount;
double*** polygons;
double** polyInfo;

void loadPolygons(int polygonCount, int polygonMaxSize, double py_polygons[polygonCount][polygonMaxSize][3], int polySizes[polygonCount]) {
    polyCount = polygonCount;
    polygons = (double***)malloc(polygonCount * sizeof(double**));
    if(!polygons) {
        printf("Could not allocate for polygons first dimension, exiting... \n");
        exit(1);
    }
    printf("Successfully allocated memory for %d polygons\n", polygonCount);
    for(int i = 0; i < polygonCount; i++) {
        polygons[i] = (double**)malloc(polySizes[i] * sizeof(double*));
        if(!polygons[i]) {
        printf("Could not allocate for polygons second dimension, exiting... \n");
        exit(1);
        }  
        printf("Successfully allocated memory for %d points for polygon %d\n", polySizes[i], i+1);
        for(int j = 0; j < polySizes[i]; j++) {
            polygons[i][j] = (double*)malloc(3 * sizeof(double));
            if(!polygons[i][j]) {
            printf("Could not allocate for polygons third dimension, exiting... \n");
            exit(1);
            }
            polygons[i][j][0] = py_polygons[i][j][0];
            polygons[i][j][1] = py_polygons[i][j][1];
            polygons[i][j][2] = py_polygons[i][j][2];
        }
    }
    printf("Successfully finished allocation for polygons\n");

    polyInfo = (double**)malloc(polygonCount * sizeof(double*));
    if(!polyInfo) {
        printf("Could not allocate for polyInfo first dimension, exiting... \n");
        exit(1);
    }
    for(int i = 0; i < polyCount; i++) {
        polyInfo[i] = (double*)malloc(3 * sizeof(double));
        if(!polyInfo[i]) {
        printf("Could not allocate for polyInfo second dimension, exiting... \n");
        exit(1);
        }
        polyInfo[i][2] = polySizes[i];
        polygonCentroid(i);
    }
    printf("Successfully allocated for polyInfo\n");
}

void polygonCentroid(int polygonIdx) {
    //First calculate centroid (cx, cy) cz = 0.0
    double** polygon = polygons[polygonIdx];
    double A, cxa, cya, cx, cy;
    double xi, yi, xip1, yip1;
    A = 0;
    cxa = 0;
    cya = 0;
    for(int i = 0; i < polyInfo[polygonIdx][2] - 1; i++) {
        xi = polygon[i][0];
        yi = polygon[i][1];
        xip1 = polygon[i + 1][0];
        yip1 = polygon[i + 1][1];
        A += 0.5 * (xi * yip1 - xip1 * yi);
        cxa += (xi + xip1) * (xi*yip1 - xip1*yi);
        cya += (yi + yip1) * (xi*yip1 - xip1*yi);
    }
    xi = polygon[(int)polyInfo[polygonIdx][2] - 1][0];
    yi = polygon[(int)polyInfo[polygonIdx][2] - 1][1];
    xip1 = polygon[0][0];
    yip1 = polygon[0][1];
    A += 0.5 * (xi * yip1 - xip1 * yi);
    cxa += (xi + xip1) * (xi*yip1 - xip1*yi);
    cya += (yi + yip1) * (xi*yip1 - xip1*yi);

    cx = cxa / (6.0 * A);
    cy = cya / (6.0 * A);
    polyInfo[polygonIdx][0] = cx;
    polyInfo[polygonIdx][1] = cy;
}

void minMax(double* v1, double* v2, double* min, double* max) {
    double temp;
    if(*v1 <= *v2) {
        *max = *v2;
        *min = *v1;
    } else {
        temp = *v2;
        *max = *v1;
        *min = temp;
    }
}
void Max(double* v1, double* v2, double* max) {
    *max = (*v1 <= *v2) ? *v2 : *v1; 
}
void Min(double* v1, double* v2, double* min) {
    *min = (*v1 <= *v2) ? *v1 : *v2; 
}

bool pointInPolygon(int polyIdx, double pos[3], double** rpolygon) {
    double** polygon;
    polygon = polygons[polyIdx];
    if(!(rpolygon == NULL)) polygon = rpolygon;
    int polygonSize = polyInfo[polyIdx][2];
    double px, py;
    px = pos[0];
    py = pos[1];
    int intersections;
    intersections = 0;
    for(int i = 0; i < (polygonSize) - 1; i++) {
        rayCast(polyIdx, &intersections, i, i+1, px, py, rpolygon);
    }
    rayCast(polyIdx, &intersections, (polygonSize) - 1, 0, px, py, rpolygon);
    if(intersections % 2 == 0) {
        return false;
    } else {
        return true;
    }
}

void rayCast(int polyIdx, int* intersections, int idxI, int idxIp1, double px, double py, double** rpolygon) {
    double** polygon;
    polygon = polygons[polyIdx];
    if(!(rpolygon == NULL)) polygon = rpolygon;
    double x1, x2, y1, y2, xMin, xMax, yMin, yMax;
    x1 = polygon[idxI][0];
    y1 = polygon[idxI][1];
    x2 = polygon[idxIp1][0];
    y2 = polygon[idxIp1][1];
    minMax(&x1, &x2, &xMin, &xMax);
    minMax(&y1, &y2, &yMin, &yMax);
    if(px <= xMax) {
        if(py == yMin || py == yMax) {
            py += 0.001;
        }
        if(py > yMin && py < yMax) {
            (*intersections) += 1;
        }
    }
}

bool vecPolygonIntersect(int polyIdx, double point1[3], double point2[3], double** rpolygon) {
    double** polygon;
    polygon = polygons[polyIdx];
    if(!(rpolygon == NULL)) polygon = rpolygon;
    int polygonSize = polyInfo[polyIdx][2];
    for(int i = 0; i < polygonSize - 1; i++) {
        if(segmentIntersection(polyIdx, point1, point2, i, i + 1, NULL)) return true;
    }
    if(segmentIntersection(polyIdx, point1, point2, polygonSize - 1, 0, NULL)) return true;
    return false;
}

bool segmentIntersection(int polyIdx, double point1[3], double point2[3], int idxI, int idxIp1, double** rpolygon) {
    /* Vector a is caddy to golfer, vector b is polygon edge */
    /* x and y are the intersection point of the line segments */
    double** polygon;
    polygon = polygons[polyIdx];
    if(!(rpolygon == NULL)) polygon = rpolygon;
    double x, y, ax1, ax2, ay1, ay2, bx1, bx2, by1, by2;
    double grad_a, grad_b;
    double ac, bc;
    double minX, maxX, minY, maxY;
    ax1 = point2[0]; ay1 = point2[1];
    ax2 = point1[0]; ay2 = point1[1];
    bx1 = polygon[idxI][0]; by1 = polygon[idxI][1];
    bx2 = polygon[idxIp1][0]; by2 = polygon[idxIp1][1];

    /* First check that segments aren't parallel */
    grad_a = (ay2 - ay1) / (ax2 - ax1);
    grad_b = (by2 - by1) / (bx2 - bx1);
    //printf("grad a: %f, grad b: %f\n", grad_a, grad_b);
    ac = ay1 - grad_a * ax1;
    bc = by1 - grad_b * bx1;
    if(isinf(fabs(grad_a)) && isinf(fabs(grad_b))) return false;
    if(grad_a == grad_b) {
        /* We also have to consider that they might be collienar, they are if c i.e y = mc + c is equal for both*/
        printf("were parallel\n");
        if(ac == bc)  {
            printf("collinear\n");
            return true;
        } else {
            return false;
        } 
    }
    /* printf("idxI: %d, idxIp1: %d\n", idxI, idxIp1);
    if(isnan(grad_b)) {
        printf("was nan\n");
        printf("by2: %f, by1: %f, bx2: %f, bx1: %f\n", by2, by1, bx2, bx1);
    } */

    /* Expressions for x and y can be obtained by solving:
    Eq1: 0 = (ay2 - ay1) / (ax2 - ax1) * (x - ax1) + ay1 - y
    Eq2: 0 = (by2 - by1) / (bx2 - bx1) * (x - bx1) + by1 - y
    Eq1 and Eq2 can be derived from y = mx + c
    */
    x = (ax1*ay2*bx1 - ax1*ay2*bx2 - ax1*bx1*by2 + ax1*bx2*by1 - ax2*ay1*bx1 + ax2*ay1*bx2 + ax2*bx1*by2 - ax2*bx2*by1)/(ax1*by1 - ax1*by2 - ax2*by1 + ax2*by2 - ay1*bx1 + ay1*bx2 + ay2*bx1 - ay2*bx2);
    y = (ax1*ay2*by1 - ax1*ay2*by2 - ax2*ay1*by1 + ax2*ay1*by2 - ay1*bx1*by2 + ay1*bx2*by1 + ay2*bx1*by2 - ay2*bx2*by1)/(ax1*by1 - ax1*by2 - ax2*by1 + ax2*by2 - ay1*bx1 + ay1*bx2 + ay2*bx1 - ay2*bx2);
    /* Set valid intersection region
    This must be done after calculating x and y because a and b vec values are modified
    */
    /* sets smaller value to 1 index and larger value to 2 index*/
    minMax(&ax1, &ax2, &ax1, &ax2);
    minMax(&bx1, &bx2, &bx1, &bx2);
    minMax(&ay1, &ay2, &ay1, &ay2);
    minMax(&by1, &by2, &by1, &by2);
    /* Set intersectino region min/max x/y  */
    Max(&ax1, &bx1, &minX);//Min x = largest of ax1 and bx1
    Min(&ax2, &bx2, &maxX);//max x = smallest of ax2 and bx2
    Max(&ay1, &by1, &minY);
    Min(&ay2, &by2, &maxY);
    
    if(x >= minX && x <= maxX && y >= minY && y <= maxY) {
        printf("Valid intersection\n");
        return true;
    } else {
        return false;
    }
}