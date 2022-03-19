import bluetooth

bd_addr = "DC:A6:32:F9:18:20"

port = 1

sock=bluetooth.BluetoothSocket( bluetooth.RFCOMM )
sock.connect((bd_addr, port))

sock.send("hello!!")

