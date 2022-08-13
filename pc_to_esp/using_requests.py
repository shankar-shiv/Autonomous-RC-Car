import requests
import numpy as np
import os
import time

# create labels
k = np.zeros((4, 4), 'float')

for i in range(4):
    k[i, i] = 1

y = np.empty((0, 4))


while True:
    inp = input('Enter command number --> ')
    inp = int(inp)

    if inp == 1:  # Left
        for _ in range(10):
            y = np.vstack((y, k[0]))
        r = requests.post('http://192.168.18.23:80/left')
        print(r.text)
    elif inp == 2:  # Right
        for _ in range(10):
            y = np.vstack((y, k[1]))
        r = requests.post('http://192.168.18.23:80/right')
        print(r.text)
    elif inp == 3:  # Forward
        for _ in range(10):
            y = np.vstack((y, k[2]))
        r = requests.post('http://192.168.18.23:80/forward')
        print(r.text)
    elif inp == 4:  # Reverse
        for _ in range(10):
            y = np.vstack((y, k[3]))
        r = requests.post('http://192.168.18.23:80/backward')
        print(r.text)
    elif inp == 5:  # Stop
        r = requests.post('http://192.168.18.23:80/reset_all')
        print(r.text)
    elif inp == 6:
        # Press 6 to save data, to be donw after stopping the vehicle
        break

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
