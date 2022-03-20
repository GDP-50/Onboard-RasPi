import bluetooth
import re
def setupBluetoothListener():
    server_sock=bluetooth.BluetoothSocket( bluetooth.RFCOMM )

    port = 1
    server_sock.bind(("", port))
    server_sock.listen(1)

    client_sock,address = server_sock.accept()
    print("Accepted connection from ", address)


    """ while True:
        data = client_sock.recv(1024)
        if not data:
            break
        print("Received", data) """

    return client_sock, server_sock

def receiveGPS(client_socket):
    data = client_socket.recv(1024)
    return data
    