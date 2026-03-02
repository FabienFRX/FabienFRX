##
## EPITECH PROJECT, 2025
## ZAPPY_IA
## File description:
## Socket
##

import sys
import socket

class Socket:
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.socket = None

    def connect(self):
        self.socket = self.create_socket()
        self.socket.settimeout(5)

    def create_socket(self):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((self.host, self.port))
        return s

    def send(self, message):
        self.socket.send(message.encode('utf-8'))

    def receive(self, timeout=0.1):
        if timeout is not None:
            self.socket.settimeout(timeout)
        try:
            return self.socket.recv(4096).decode('utf-8')
        except socket.timeout:
            return None

    def close(self):
        self.socket.close()
        
    
