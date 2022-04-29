import numpy as np
from ctypes import *
from C_wrappers import *
from BlueToothServer import receiveGPS, setupBluetoothListener
from gps import get_gps, parseGPS
import time


coursePath = "Hole 1.txt"
greenSize = c_int(0)
greenPos = np.zeros(3, dtype=c_double)
bunkerCount = c_int(0)
bunkerSizes = np.zeros(128, dtype=c_int)
loadCourse(c_char_p(coursePath.encode("utf-8")), byref(greenSize), byref(bunkerCount), bunkerSizes)
maxPolygonSize = max(greenSize.value, np.max(bunkerSizes))


scaleVal = c_double(0)
bunkerMaxSize = np.max(bunkerSizes)
green = np.ndarray(dtype=c_double, shape=(greenSize.value, 3))
bunkers = np.ndarray(shape=(bunkerCount.value, bunkerMaxSize, 3), dtype=c_double)
teePos = np.zeros(3, dtype=c_double)


prepareCourse(coursePath.encode("utf-8"), greenSize.value, green, bunkerCount.value, bunkerMaxSize, bunkers, bunkerSizes, teePos, byref(scaleVal))

polygons = np.ndarray(shape=(1+bunkerCount.value, maxPolygonSize, 3), dtype=c_double)
polygons[0][0:len(green)] = green
for i, bunker in enumerate(bunkers):
    polygons[i+1][0:len(bunker)] = bunker

polyInfo = np.ndarray(shape=(1+bunkerCount.value, 3), dtype=c_double)
polyInfo[0][2] = float(greenSize.value)
for i, bunker in enumerate(bunkers):
    polyInfo[i+1][2] = float(bunkerSizes[i])
polySizes = np.zeros(shape=(len(polygons),))
for i, poly in enumerate(polyInfo):
    polySizes[i] = poly[2]
loadPolygons(c_int(len(polygons)), c_int(maxPolygonSize), polygons, polySizes)





client_socket, server_socket = setupBluetoothListener()
while True:
    caddyHeading = None
    gpsData = receiveGPS(client_socket)
    if not gpsData:
        break
    golferPosX, golferPosY = parseGPS(gpsData)
    gpos = np.array([golferPosX, golferPosY, 0])
    time.sleep(0.5)

    caddyPosX, caddyPosY = get_gps()
    cpos = np.array([caddyPosX, caddyPosY, 0])
    if caddyHeading is None:
        setMotorSpeed(100, 100)
        time.sleep(2)
        setMotorSpeed(0, 0)
        caddyPosX2, caddyPosY2 = get_gps()
        dcpx = caddyPosX2 - caddyPosX
        dcpy = caddyPosY2 - caddyPosY
        dvec = np.array([dcpx, dcpy])
        dvec /= np.linalg.norm(dvec)
        unitVec = np.array([0,1])
        caddyHeading = np.acos(np.dot(dvec, unitVec))
        print("Heading is %f degrees" % np.rad2deg(caddyHeading))

    caddyControl(gpos, cpos, caddyHeading)