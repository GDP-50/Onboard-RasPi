from BlueToothServer import receiveGPS, setupBluetoothListener
from gps import get_gps, parseGPS
import time


client_socket, server_socket = setupBluetoothListener()
while True:
    gpsData = receiveGPS(client_socket)
    if not gpsData:
        break
    lat, long = parseGPS(gpsData)
    time.sleep(0.5)
    

