import socket
import os
from threading import Thread
import struct
import time

server_filter = []
client_filter = []

server_white = []
client_white = []

server_queue = []
client_queue = []

class Proxy2Server(Thread):
    def __init__(self, host, port):
        super(Proxy2Server, self).__init__()
        self.client = None
        self.port = port
        self.host = host
        self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server.connect((host, port))
    
    def run(self):
        while True:
            data = self.server.recv(4096)
            if data and not server_white:
                if data[0:2] not in server_filter:
                    print(f"[{self.port}] => {data[:].hex()}")
            elif data and server_filter:
                if(data[0:2] in server_white):
                    print(f"[{self.port}] => {data[:].hex()}")
            try:
                if len(client_queue):
                    self.client.sendall(client_queue.pop(0))
                self.client.sendall(data)
            except Exception as e:
                pass
                # print(f"server_err: {e}")

class Client2Proxy(Thread):
    def __init__(self, host, port):
        super(Client2Proxy, self).__init__()
        self.server = None
        self.port = port
        self.host = host
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        sock.bind((host, port))
        sock.listen(1)
        self.client,_ = sock.accept() 

    def run(self):
        while True:
            data = self.client.recv(4096)
            if data and not client_white:
                if data[0:2] not in client_filter:
                    print(f"[{self.port}] => {data[:].hex()}")
            elif data and client_white:
                if(data[0:2] in client_white):
                    print(f"[{self.port}] => {data[:].hex()}")
            try:
                if len(server_queue):
                    self.server.sendall(server_queue.pop(0))
                self.server.sendall(data)
            except Exception as e:
                pass
                # print(f"client_err: {e}")


class Proxy(Thread):
    def __init__(self, from_host, to_host, port):
        super(Proxy, self).__init__()
        self.from_host = from_host
        self.to_host = to_host
        self.port = port
        self.running = False
    
    def run(self):
        while True:
            self.c2p = Client2Proxy(self.from_host, self.port)
            self.p2s = Proxy2Server(self.to_host, self.port)
            self.c2p.server = self.p2s.server
            self.p2s.client = self.c2p.client
            self.running = True

            self.c2p.start()
            self.p2s.start()

if __name__ == "__main__":
    game_servers = []
    for port in range(3001, 3002):
        _game_server = Proxy('0.0.0.0', '127.0.0.1', port)
        _game_server.start()
        game_servers.append(_game_server)

    server_filter += [b'\x00\x00', b'\x70\x73']
    client_filter += [b'\x6d\x76', b'\x6a\x70']
    server_white += [b'\x30\x31']
    client_white += [b'\x30\x31']

    while True:
        try:
            cmd = input("$ ")
            if(cmd == "quit"):
                print("quit proxy")
                os._exit(0)
            elif(cmd == "egg"):
                egg = [
                    [-25045, 18085, 260],
                    [-51570, -61215, 5020],
                    [24512, 69682, 2659],
                    [60453, -17409, 2939],
                    [1522, 14966, 7022],
                    [11604, -13131, 411],
                    [-72667, -53567, 1645],
                    [48404, 28117, 704],
                    [65225, -5740, 4928],
                    [-2778, -11035, 10504], # BallmerPeakEgg
                    [-6095.1035, -11191.464, 10528.149] # BallmerPeakPoster
                ]
                for i in range(9):
                    server_queue += [pak :=
                        b'\x6d\x76' +
                            struct.pack('<fff', egg[i][0], egg[i][1], egg[i][2]) +
                            b'\x00\x00' * 3 +
                            b'\x00' * 2 +
                        b'\x65\x65' + 
                            struct.pack('<I', 0xb+i)
                    ]
                    print(pak)
                    time.sleep(1)
                server_queue += [pak :=  
                        b'\x6d\x76' +
                            struct.pack('<fff', egg[10][0], egg[10][1], egg[10][2]) +
                            b'\x00\x00' * 3 +
                            b'\x00' * 2 +
                        b'\x2a\x69' + 
                            struct.pack('<H', 0xb) +
                            b'CowboyCoder' +
                            struct.pack('<fff', -2.6102295, 90.32108, 0)
                    ]
                time.sleep(1)
                server_queue += [pak := 
                        b'\x6d\x76' +
                            struct.pack('<fff', egg[9][0], egg[9][1], egg[9][2]) +
                            b'\x00\x00' * 3 +
                            b'\x00' * 2 +
                        b'\x65\x65' + 
                            struct.pack('<I', 0xb+9)
                    ]
        except KeyboardInterrupt:
            os._exit(0)
        except Exception as e:
            print(e)
            os._exit(-1)