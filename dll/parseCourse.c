#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef enum { false = 0, true = !false } bool;

void loadCourse(const char * path, int* greenSize, int* bunkerCount, int bunkerSizes[128]) {
    for (int* val = bunkerSizes; val < (bunkerSizes + 128); val++) {
        *val = 0;
    }
    int maxCoordCount = 0;
    FILE * file = fopen(path, "r");
    if( file == NULL ){
        printf("File doesn't exist or I'm in the wrong directory \n");
        getchar();
        return;
    }
    int teeCount = 0;
    while( 1 ){
        static int bunkerCoordCount = 0;
        int* activeCount;
        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s \n", lineHeader);
    
        if(strcmp(lineHeader, "#") == 0) {
            
        } else if(strcmp(lineHeader, "Green") == 0) {
            activeCount = greenSize;
        } else if (strcmp(lineHeader, "Bunker" ) == 0 ) {
            activeCount = &bunkerCoordCount;
            (*bunkerCount)++;
            bunkerCoordCount = 0;
        } else if (strcmp(lineHeader, "Tee") == 0) {
            activeCount = &teeCount;
        } else if (strcmp(lineHeader, "p") == 0) {
            (*activeCount)++;
            if (activeCount == &bunkerCoordCount) {
                bunkerSizes[*bunkerCount - 1] = bunkerCoordCount;
            }
        } 
        if (res == EOF) break;
    }

    fclose(file);
   
}



void prepareCourse(const char * path, int greenSize, double green[greenSize][3], int bunkerCount, int bunkerMaxSize, double bunkers[bunkerCount][bunkerMaxSize][3], int bunkerSizes[128], double teePos[3], double* scaleVal) {
    FILE* file = fopen(path, "r");
    char lineHeader[128];
    bool bunkerActive = false;
    bool greenActive = false;
    bool teeActive = false;
    int bunker = 0;
    int coordCount = 0;
    int linecount = 0;
    while (1) {

        // read the first word of the line
        int res = fscanf(file, "%s \n", lineHeader);
        if(res == EOF) break;
        linecount++;
        if(strcmp(lineHeader, "#") == 0) {
            
        } else if(strcmp(lineHeader, "Green") == 0) {
            greenActive = true;
            bunkerActive = false;
            teeActive = false;
            coordCount = 0;
        } else if (strcmp(lineHeader, "Bunker" ) == 0 ) {
            bunkerActive = true;
            greenActive = false;
            teeActive = false;
            coordCount = 0;
            bunker++;
        } else if (strcmp(lineHeader, "Tee") == 0) {
            teeActive = true;
            greenActive = false;
            bunkerActive = false;
        } else if (strcmp(lineHeader, "p") == 0) {
            double c1, c2;
            fscanf(file, "%lf %lf\n", &c1, &c2);
            if (greenActive) {
                green[coordCount][0] = (double)c1;
                green[coordCount][1] = (double)c2;
            } else if (bunkerActive) {
                printf("C1: %lf, c2: %lf\n", c1, c2);
                bunkers[bunker - 1][coordCount][0] = (double)c1;
                bunkers[bunker - 1][coordCount][1] = (double)c2;
                printf("set to %lf and %lf\n", bunkers[bunker - 1][coordCount][0], bunkers[bunker - 1][coordCount][1]);
            } else if (teeActive) {
                teePos[0] = (double)c1;
                teePos[1] = (double)c2;
                teePos[2] = 0.0;
            } else {
                printf("what is going on here??? \n");
            }
            coordCount++;
        }
    }


    fclose(file);

    double teex, teey, teez;
    teex = teePos[0];
    teey = teePos[1];
    teez = 0.0;

    double deg2meters = 2 * M_PI * 6371000 / 360;
    double largestVal = 0;
    //Green Relative Positions in meters
    for(int i = 0; i < greenSize; i++) {
        double gpx, gpy, gpz;
        double relx, rely, relz;
        gpx = green[i][0];
        gpy = green[i][1];
        gpz = 0.0;
        relx = teex - gpx;
        relz = teez - gpz;
        relz = teez - gpz;
        green[i][0] = (double)(relx * deg2meters);
        green[i][1] = (double)(rely * deg2meters);
        green[i][2] = (double)(relz * deg2meters);
    }
    //Bunker Relative Positions in meters
    for(int i = 0; i < bunkerCount; i++) {
        for(int j = 0; j < bunkerSizes[i]; j++) {
            double bpx, bpy, bpz;
            double relx, rely, relz;
            bpx = bunkers[i][j][0];
            bpy = bunkers[i][j][1];
            bpz = 0.0;
            relx = teex - bpx;
            relz = teez - bpz;
            relz = teez - bpz;
            bunkers[i][j][0] = (double)(relx * deg2meters);
            bunkers[i][j][1] = (double)(rely * deg2meters);
            bunkers[i][j][2] = (double)(relz * deg2meters);
        }
    }

    //Find Largest Value in All arrays
    for(int i = 0; i < greenSize; i++) {
        for(int j = 0; j < 2; j++) {
            if (abs(green[i][j]) > largestVal) {
                largestVal = abs(green[i][j]);
            }
        }
    }
    for(int i = 0; i < bunkerCount; i++) {
        for(int j = 0; j < bunkerSizes[i]; j++) {
            for(int k = 0; k < 2; k++) {
                if (abs(bunkers[i][j][k]) > largestVal) {
                    largestVal = abs(bunkers[i][j][k]);
                }
            }
        }
    }
    *scaleVal = largestVal;
    //Scale between 0 and 2
    //Then move to between 1 and -1
    for(int i = 0; i < greenSize; i++) {
        for(int j = 0; j < 2; j++) {
            green[i][j] /= (0.5 * largestVal);
            green[i][j] -= 1.0;
        }
    }
    for(int i = 0; i < bunkerCount; i++) {
        for(int j = 0; j < bunkerSizes[i]; j++) {
            for(int k = 0; k < 2; k++) {
                bunkers[i][j][k] /= (0.5 * largestVal);
                bunkers[i][j][k] -= 1.0;
            }
        }
    }
    return;
}