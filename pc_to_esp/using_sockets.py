
# Client program
import socket               # Import socket module
import time
import numpy as np
import os

s = socket.socket()         # Create a socket object
host = '192.168.18.23'      # Get local machine name
port = 12345                # Reserve a port for your service.

s.connect((host, port))

# create labels
k = np.zeros((4, 4), 'float')

for i in range(4):
    k[i, i] = 1

y = np.empty((0, 4))


def send_command():

    global y
    global k
    global s

    while True:
        inp = input('Enter command number --> ')
        inp = int(inp)
        if inp == 1:  # Left
            for _ in range(10):
                y = np.vstack((y, k[0]))
            s.send(b'1')
        elif inp == 2:  # Right
            for _ in range(10):
                y = np.vstack((y, k[1]))
            s.send(b'2')
        elif inp == 3:  # Forward
            for _ in range(10):
                y = np.vstack((y, k[2]))
            s.send(b'3')
        elif inp == 4:  # Reverse
            for _ in range(10):
                y = np.vstack((y, k[3]))
            s.send(b'4')
        elif inp == 5:
            s.send(b'5')
        elif inp == 6:
            # Press 6 to save data, to be donw after stopping the vehicle
            break


if __name__ == "__main__":

    send_command()

    # The below code will function after the while loop breaks

    # save data as a numpy file
    file_name = str(int(time.time()))
    directory = "training_labels_data"

    if not os.path.exists(directory):
        os.makedirs(directory)
    try:
        np.savez(directory + '/' + file_name +
                 '.npz', train_labels=y)
    except IOError as e:
        print(e)

    print(y.shape)
