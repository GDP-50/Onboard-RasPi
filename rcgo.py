import numpy as np
from ctypes import *
from C_wrappers import *
from BlueToothServer import receiveGPS, setupBluetoothListener
#from gps import get_gps, parseGPS
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
polySizes = np.zeros(shape=(len(polygons),), dtype=c_int)
for i, poly in enumerate(polyInfo):
    polySizes[i] = poly[2]
loadPolygons(c_int(len(polygons)), c_int(maxPolygonSize), polygons, polySizes)

def setCaddyHeading(px1, py1, px2, py2):
    dcpx = px2 - px1
    dcpy = py2 - py1
    dvec = np.array([dcpx, dcpy])
    dvec /= np.linalg.norm(dvec)
    unitVec = np.array([0,1])
    caddyHeading = np.arccos(np.dot(dvec, unitVec))
    return caddyHeading

client_socket, server_socket = setupBluetoothListener()
testBTdata = receiveGPS(client_socket)
print(testBTdata)


def init_caddy():
    caddyPosX, caddyPosY = get_gps()
    caddyPosX = float(caddyPosX)
    caddyPosY = float(caddyPosY)
    setMotorSpeed(100, 100)
    time.sleep(2)
    setMotorSpeed(0, 0)
    caddyPosX2, caddyPosY2 = get_gps()
    caddyHeading = setCaddyHeading(caddyPosX, caddyPosY, caddyPosX2, caddyPosY2)
    return caddyPosX2, caddyPosY2, caddyHeading

def get_mode():
    gpsData = receiveGPS(client_socket)
    mode = int(gpsData.decode())
    if mode is None:
        mode = 0
    return mode 

#cx1, cy1, rotation = init_caddy()

mode = get_mode()
MD49SerialInit()
setMode(0)
while True:
    mode = get_mode()
    while mode == 0:
        driveMotors(128, 128)
        print("0")
        mode = get_mode()
    while mode == 1:
        driveMotors(255, 255)
        print("F")
        mode = get_mode()
    while mode == 2:
        driveMotors(128 + 100, 128 - 100)
        mode = get_mode()
        print("R")
    while mode == 3:
        driveMotors(128 - 100, 128 + 100)
        print("L")
        mode = get_mode()
    

    



