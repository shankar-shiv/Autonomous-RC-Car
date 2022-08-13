# Send car command to raspi to move the car by connecting to ESP chip
# Collect the output layer data here!
# 1 - Forward
# 2 - Backward
# 3 - Left
# 4 - Right
# 5 - Stop
# So, we will have 5 output neurons in the output layer.
# Written by Shankar

__author__ = 'shankar'

import socket
import sys

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((socket.gethostname(), 1234))
s.listen(5)

# now our endpoint knows about the OTHER endpoint.
clientsocket, address = s.accept()
print(f"clientsocket from {address} has been established.")


def runExample():
    while True:
        inp = input("Enter a number --> ")
        inp = int(inp)

        if inp == 1:
            clientsocket.send(bytes('1', "utf-8"))
        elif inp == 2:
            clientsocket.send(bytes('2', "utf-8"))
        elif inp == 3:
            clientsocket.send(bytes('3', "utf-8"))
        elif inp == 4:
            clientsocket.send(bytes('4', "utf-8"))
        elif inp == 5:
            clientsocket.send(bytes('5', "utf-8"))
        else:
            clientsocket.send(bytes('0', "utf-8"))


if __name__ == "__main__":
    try:
        runExample()
    except(KeyboardInterrupt, SystemExit) as exErr:
        print('Ending')
        sys.exit(0)
