import numpy as np
import cv2
import requests
import pygame
from pygame.locals import *
import socket
import time
import os


class CollectTrainingData(object):

    def __init__(self, host, port, input_size):

        self.server_socket = socket.socket()
        self.server_socket.bind((host, port))
        self.server_socket.listen(0)

        # accept a single connection
        self.connection = self.server_socket.accept()[0].makefile('rb')

        self.send_inst = True

        self.input_size = input_size

        # create labels
        self.k = np.zeros((4, 4), 'float')
        for i in range(4):
            self.k[i, i] = 1

        pygame.init()
        pygame.display.set_mode((250, 250))

    def collect(self):

        saved_frame = 0
        total_frame = 0

        # collect images for training
        print("Start collecting images...")
        print("Press 'q' or 'x' to finish...")
        start = cv2.getTickCount()

        X = np.empty((0, self.input_size))
        y = np.empty((0, 4))

        # stream video frames one by one
        try:
            stream_bytes = b' '
            frame = 1
            while self.send_inst:
                stream_bytes += self.connection.read(1024)
                first = stream_bytes.find(b'\xff\xd8')
                last = stream_bytes.find(b'\xff\xd9')

                if first != -1 and last != -1:
                    jpg = stream_bytes[first:last + 2]
                    stream_bytes = stream_bytes[last + 2:]
                    image = cv2.imdecode(np.frombuffer(
                        jpg, dtype=np.uint8), cv2.IMREAD_GRAYSCALE)

                    # select lower half of the image
                    height, width = image.shape
                    roi = image  # [int(height/2):height, :]

                    cv2.imshow('image', image)

                    # reshape the roi image into a vector
                    temp_array = roi.reshape(
                        1, height * width).astype(np.float32)  # 1 row, 76800 columns

                    frame += 1
                    total_frame += 1

                    # get input from human driver
                    for event in pygame.event.get():
                        if event.type == KEYDOWN:
                            key_input = pygame.key.get_pressed()
                            # simple orders
                            if key_input[pygame.K_UP]:
                                saved_frame += 1
                                X = np.vstack((X, temp_array))
                                y = np.vstack((y, self.k[2]))
                                r = requests.post(
                                    'http://192.168.18.23:80/forward')
                                print(r.text)

                            elif key_input[pygame.K_DOWN]:
                                r = requests.post(
                                    'http://192.168.18.23:80/backward')
                                print(r.text)

                            elif key_input[pygame.K_RIGHT]:
                                X = np.vstack((X, temp_array))
                                y = np.vstack((y, self.k[1]))
                                saved_frame += 1
                                r = requests.post(
                                    'http://192.168.18.23:80/right')
                                print(r.text)

                            elif key_input[pygame.K_LEFT]:
                                X = np.vstack((X, temp_array))
                                y = np.vstack((y, self.k[0]))
                                saved_frame += 1
                                r = requests.post(
                                    'http://192.168.18.23:80/left')
                                print(r.text)

                            elif key_input[pygame.K_SPACE]:
                                saved_frame += 1
                                r = requests.post(
                                    'http://192.168.18.23:80/reset_all')
                                print(r.text)

                            elif key_input[pygame.K_x] or key_input[pygame.K_q]:
                                print("exit")
                                self.send_inst = False
                                break

                        elif event.type == pygame.KEYUP:
                            r = requests.post(
                                'http://192.168.18.23:80/reset_all')
                            print(r.text)

                    if cv2.waitKey(1) & 0xFF == ord('q'):
                        break

            # save data as a numpy file
            file_name = str(int(time.time()))
            directory = "training_data"
            if not os.path.exists(directory):
                os.makedirs(directory)
            try:
                np.savez(directory + '/' + file_name +
                         '.npz', train=X, train_labels=y)
            except IOError as e:
                print(e)

            end = cv2.getTickCount()
            # calculate streaming duration
            print("Streaming duration: , %.2fs" %
                  ((end - start) / cv2.getTickFrequency()))

            print(X.shape)
            print(y.shape)
            print("Total frame: ", total_frame)
            print("Saved frame: ", saved_frame)
            print("Dropped frame: ", total_frame - saved_frame)

        finally:
            # Release Memory and clean evrything
            self.connection.close()
            self.server_socket.close()


if __name__ == '__main__':
    # host, port
    host, port = "192.168.18.28", 12346

    # vector size, half of the image
    input_size = 320 * 240

    ctd = CollectTrainingData(host, port, input_size)
    ctd.collect()
