import numpy as np
from ctypes import *
from C_wrappers import *



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

""" for i, polygon in enumerate(polygons):
    polygonCentroid(c_int(int(polyInfo[i][2])), polygon, polyInfo[i][0:2]) """




""" from BlueToothServer import receiveGPS, setupBluetoothListener
from gps import get_gps, parseGPS
import time


client_socket, server_socket = setupBluetoothListener()
while True:
    gpsData = receiveGPS(client_socket)
    if not gpsData:
        break
    lat, long = parseGPS(gpsData)
    time.sleep(0.5)
     """

