# Receive camera image stream from raspi to pc, save image data in npz format
# Written by Shankar

__author__ = 'shankar'

import numpy as np
import cv2
import socket
import pygame
from pygame.locals import *
# To get the host ip address, use
# print(socket.gethostbyname("Shankar"))


class VideoStreamingTest(object):
    def __init__(self, host, port):

        self.server_socket = socket.socket()
        self.server_socket.bind((host, port))

        # Enable a server to accept connections.
        # If backlog is specified, it must be at least 0 (if it is lower, it is set to 0); it specifies the number of unaccepted connections that the system will allow before refusing new connections. If not specified, a default reasonable value is chosen.
        self.server_socket.listen(0)
        self.connection, self.client_address = self.server_socket.accept()
        self.connection = self.connection.makefile('rb')

        self.host_name = socket.gethostname()  # shiva-shankar
        self.host_ip = socket.gethostbyname(self.host_name)  # 192.168.18.28

    def streaming(self):
        try:
            print("Host: ", self.host_name + ' ' + self.host_ip)
            print("Connection from: ", self.client_address)
            print("Streaming...")
            print("Press 'q' to exit")

            # need bytes here
            stream_bytes = b' '
            while True:
                stream_bytes += self.connection.read(1024)
                first = stream_bytes.find(b'\xff\xd8')
                last = stream_bytes.find(b'\xff\xd9')
                if first != -1 and last != -1:
                    jpg = stream_bytes[first:last + 2]
                    stream_bytes = stream_bytes[last + 2:]
                    image = cv2.imdecode(np.frombuffer(
                        jpg, dtype=np.uint8), cv2.IMREAD_COLOR)
                    cv2.imshow('image', image)

                    if cv2.waitKey(1) & 0xFF == ord('q'):
                        break
        finally:
            self.connection.close()
            self.server_socket.close()


if __name__ == '__main__':
    # host, port
    print(socket.gethostbyname("shiva-shankar"))
    h, p = ("192.168.18.28", 12346)

    # Create a VideoStreaming Object
    VideoStreaming = VideoStreamingTest(h, p)

    VideoStreaming.streaming()
