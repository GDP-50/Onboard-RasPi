#include "control.h"

double xIncrement = -1;
double yIncrement = -1;
double target[3];
bool activeTarget = false;
bool caddyHasRotated = true;

void caddyControl(double golferPos[3], double caddyPos[3], double caddyRotation) {
    double dist;
    double gx, gy, gz;
    double cx, cy, cz;
    double dx, dy, dz;
    gx = golferPos[0]; gy = golferPos[1]; gz = golferPos[2];
    cx = caddyPos[0]; cy = caddyPos[1]; cz = caddyPos[2];
    dx = gx - cx; dy = gy - cy; dz = gz - cz;
    dist = sqrt(pow(dx, 2.0) + pow(dy, 2.0) + pow(dz, 2.0));
    printf("Dist in c is %f\n", dist * 111194);
    /* Check if the caddy is already trying to reach somewhere */
    if(!activeTarget) {
    /* Check if caddy is already close to golfer */
        if(dist < meters2deg(5)) return;
        /* 
        Now check if golfer in in no go zone 
        If it is then do nothing. 
        */
        for(int i = 0; i < polyCount; i++) {
            if(pointInPolygon(i, golferPos, NULL)) return;
        }
        /* 
        Now check if direct path intersects hazard
        If not then set target to golferPos
        If so then do pathfinding 
        */
        bool vecIntersects = false;
        int polyIntersectionIdx;
        for(int i = 0; i < polyCount; i++) {
            if(vecPolygonIntersect(i, caddyPos, golferPos, NULL)) {
                vecIntersects = true;
                polyIntersectionIdx = i;
                break;
            }
        }
        if(vecIntersects) {
            /* pathfinding */
            pathFind(polyIntersectionIdx, caddyPos, golferPos);
        } else {
            target[0] = golferPos[0];
            target[1] = golferPos[1];
            target[2] = golferPos[2];
        }
        activeTarget = true;
    } else {
        reachTarget(caddyPos, &caddyRotation);
    }
}

void reachTarget(double caddyPos[3], double* caddyRotation) {
    printf("Trying to reach target");
    double caddyDir[3], chord[3];
    double** caddyRotationMatrix;
    caddyRotationMatrix = gen3x3RotationMatrix_Z(deg2rad(*caddyRotation));
    chord[0] = target[0] - caddyPos[0];
    chord[1] = target[1] - caddyPos[1];
    chord[2] = target[2] - caddyPos[2];
    matrixVectorMultiplication(3, chord, caddyRotationMatrix, caddyDir);
    double angle = angleBetweenVecN(3, caddyDir, chord);
    double dist = vecNmag(3, chord);
    double angleErr = atan(MAX_ERR / dist);
    if(angle>=angleErr) {
        /* Rotate Caddy */
        rotateCaddy(angle);
    }
    /* Translate caddy */
    translateCaddy(dist);
    activeTarget = false;
    (*caddyRotation) += angle;
}

void translateCaddy(double dist) {
    long encoder1, encoder2;
    long encoderTarget;
    double wheel_circumference;
    wheel_circumference = 2 * M_PI * WHEEL_RADIUS_M;
    encoderTarget = dist / wheel_circumference * 360;
    resetEncoders();
    while(encoder1 < encoderTarget) {
        driveMotors(200, 200);
        guardOverCurrent();
        readEncoders(&encoder1, &encoder2);
    }
    driveMotors(0, 0);
}

void rotateCaddy(double angle) {
    long encoder1, encoder2;
    long encoderTarget;
    double wheel_circumference;
    double dist = WHEEL_BASE_M / 2 * angle;
    wheel_circumference = 2 * M_PI * WHEEL_RADIUS_M;
    encoderTarget = dist / wheel_circumference * 360;
    resetEncoders();
    while(encoder1 < encoderTarget) {
        driveMotors(128 +50, 128 -50);
        guardOverCurrent();
        readEncoders(&encoder1, &encoder2);
    }
    driveMotors(0, 0);
    caddyHasRotated = true;
}


double meters2deg(double meters) {
    double deg = meters * (360 / (2 * M_PI * RADIUS_EARTH));
    return deg;
}

void pathFind(int polyIdx, double caddyPos[3], double targetPos[3]) {
    double** polygon = polygons[polyIdx];
    int polySize = polyInfo[polyIdx][2];
    double yDisp, xDisp, theta;
    double chord[3], center[3];
    double point[3], rotatedPoint[3], rcaddyPos[3], rtargetPos[3], rPathTarget[3], yUnit[3];
    double **rotationMatrix, **unRotationMatrix;
    double a1[3], a2[3]; 
    bool yShouldRepeat, xShouldRepeat;
    double** rotatedPoly;
    chord[0] = targetPos[0] - caddyPos[0];
    chord[1] = targetPos[1] - caddyPos[1];
    chord[2] = targetPos[2] - caddyPos[2];
    yUnit[0] = 0; yUnit[1] = 1; yUnit[2] = 0;
    theta = angleBetweenVecN(3, chord, yUnit);
    rotationMatrix = gen3x3RotationMatrix_Z(theta);
    rotatedPoly = (double**)malloc(polySize * sizeof(double));
    if(!rotatedPoly) {
        printf("Failed to allocate for rotated polygon, exiting... \n");
        exit(1);
    }
    /* Transform to new rotated coordinate system for ease */
    matrixVectorMultiplication(3, targetPos, rotationMatrix, rtargetPos);
    matrixVectorMultiplication(3, caddyPos, rotationMatrix, rcaddyPos);
    for(int i = 0; i < polySize; i++) {
        rotatedPoly[i] = (double*)malloc(3 * sizeof(double));
        if(!rotatedPoly[i]) {
            printf("Failed to allocated for rotatedPoly[%d], exiting...\n", i);
            exit(1);
        }
        point[0] = polygon[i][0];
        point[1] = polygon[i][1];
        point[2] = polygon[i][2];
        matrixVectorMultiplication(3, point, rotationMatrix, rotatedPoint);
        rotatedPoly[i][0] = rotatedPoint[0];
        rotatedPoly[i][1] = rotatedPoint[1];
        rotatedPoly[i][2] = rotatedPoint[2];
    }
    /* The segment points of horizontal line that we slide down */
    /* First for Y */
    yShouldRepeat = true;
    yDisp = 0.0;
    a1[0] = rcaddyPos[0]; a1[1] = rcaddyPos[1] + yDisp; a1[2] = rcaddyPos[2];
    a1[0] = rtargetPos[0]; a1[1] = rcaddyPos[1] + yDisp; a1[2] = rcaddyPos[2];
    while(yShouldRepeat) {
        while(vecPolygonIntersect(polyIdx, a1, a2, rotatedPoly)) {
            yDisp += yIncrement;
            a1[0] = rcaddyPos[0] - 1000; a1[1] = rcaddyPos[1] + yDisp; a1[2] = rcaddyPos[2];
            a1[0] = rtargetPos[0] + 1000; a1[1] = rcaddyPos[1] + yDisp; a1[2] = rcaddyPos[2];
            center[0] = (a1[0] + a2[0]) / 2.0; 
            center[1] = (a1[1] + a2[1]) / 2.0; 
            center[2] = (a1[2] + a2[2]) / 2.0;
        }
        if(pointInPolygon(polyIdx, center, rotatedPoly)) {
            yIncrement *= -1;
            yDisp = 0;
            a1[0] = rcaddyPos[0]; a1[1] = rcaddyPos[1] + yDisp; a1[2] = rcaddyPos[2];
            a1[0] = rtargetPos[0]; a1[1] = rcaddyPos[1] + yDisp; a1[2] = rcaddyPos[2];
        } else {
            yShouldRepeat = false;
        }
    }
    xDisp = 0.0;
    a1[0] = rcaddyPos[0] +xDisp; a1[1] = rcaddyPos[1]; a1[2] = rcaddyPos[2];
    a1[0] = rtargetPos[0] +xDisp; a1[1] = rcaddyPos[1] + yDisp; a1[2] = rcaddyPos[2];
    while(vecPolygonIntersect(polyIdx, a1, a2, rotatedPoly)) {
        xDisp += xIncrement;
        a1[0] = rcaddyPos[0] +xDisp; a1[1] = rcaddyPos[1]; a1[2] = rcaddyPos[2];
        a1[0] = rtargetPos[0] +xDisp; a1[1] = rcaddyPos[1] + yDisp; a1[2] = rcaddyPos[2];
    }
    unRotationMatrix = gen3x3RotationMatrix_Z(-theta);
    rPathTarget[0] = rcaddyPos[0] + xDisp;
    rPathTarget[1] = rcaddyPos[1] + yDisp;
    rPathTarget[2] = rcaddyPos[0];
    matrixVectorMultiplication(3, rPathTarget, unRotationMatrix, target);
    for(int i = 0; i < 3; i++) {
        free(rotationMatrix[i]);
        free(unRotationMatrix[i]);
    }
    for(int i = 0; i < polySize; i++) {
        free(rotatedPoly[i]);
    }
    free(rotationMatrix);
    free(unRotationMatrix);
    free(rotatedPoly);
}


void matrixVectorMultiplication(int n, double vec[n], double** matrix, double outVec[n]) {
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            outVec[i] += matrix[i][j] * vec[j];
        }
    }
}

double** gen3x3RotationMatrix_Z(double angle) {
    double** matrix = (double**)malloc(3 * sizeof(double*));
    if(!matrix) {
        printf("Couldn't allocate gen3x3RotationMatrix_Z first dim, exiting...\n");
        exit(1);
    }
    for(int i = 0; i < 3; i++) {
        matrix[i] = (double*)calloc(3, sizeof(double));
        if(!matrix[i]) {
            printf("Couldn't allocate gen3x3RotationMatrix_Z second dim, exiting...\n");
            exit(1);
        }
    }
    matrix[0][0] = cos(angle);
    matrix[0][1] = -sin(angle);
    matrix[1][0] = sin(angle);
    matrix[1][1] = cos(angle);
    matrix[2][2] = 1.0;
    return matrix;
}

double vecNmag(int n, double vec[3]) {
    double sum = 0;
    for(int i = 0; i < n; i++) {
        sum += pow(vec[i], 2.0);
    }
    return sqrt(sum);
}

double angleBetweenVecN(int n, double vec1[n], double vec2[n]) {
    double dot = 0;
    for(int i = 0; i < n; i++) {
        dot += vec1[i] * vec2[i];
    }
    double mag1 = vecNmag(n, vec1);
    double mag2 = vecNmag(n, vec2);
    return acos(dot / (mag1 * mag2));
}

double deg2rad(double deg) {
    double theta;
    theta =  deg * M_PI / 180;
    return theta;
}

bool getHasRotated() {
    return caddyHasRotated;
}

void setHasRotated(bool hasRotated) {
    caddyHasRotated = hasRotated;
}