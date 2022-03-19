import bluetooth

onboard_addr = "DC:A6:32:F9:18:20"
port = 1
socket = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
socket.connect((onboard_addr, port))
socket.send('\x1A')