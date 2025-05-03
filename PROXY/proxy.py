import socket
import os
from threading import Thread
import struct
import time
from itertools import product

server_black = []
client_black = []

server_white = []
client_white = []

server_queue = []
client_queue = []

PARSE_CURCUIT = False
FIND_CIRCUIT = False
fd = None

def parse_circuit(data):
    size = struct.unpack('<H', data[2:4])[0]
    stage = data[4:4+size].decode()
    input_state = struct.unpack('<I', data[4+size:4+size+4])[0]
    node_count = struct.unpack('<H', data[4+size+4:4+size+6])[0]
    buf_len = (node_count + 7) >> 3
    output_state = int.from_bytes(data[4+size+6:4+size+6+buf_len], byteorder='big')
    if stage == 'FinalStage':
        tmp = bin(input_state)[2:].rjust(32, '0')[1::2]
        if f'{output_state:0176b}'[:6] == '000000':
            print("\n=======================================================================")
            print()
            print(f"\t[*] Find INPUT: {input_state:032b}")
            print()
            print("=======================================================================")
            global FIND_CIRCUIT
            FIND_CIRCUIT = True
        print(f'{stage} = {int(tmp, 2)}')
    return input_state, output_state, stage, buf_len

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
            if data:
                if server_black and not server_white:
                    if data[0:2] not in server_black:
                        print(f"[client:{self.port}] <= {data[:].hex()}")
                elif server_white:
                    if data[0:2] in server_white and PARSE_CURCUIT:
                        _in, _out, stage, buf_len = parse_circuit(data)
                        print(f'{stage} = {_in:0{32 if buf_len != 1 else 8}b}|{_in:0{buf_len*8}b}')
                    elif(data[0:2] in server_white):
                        if fd != None:
                            _in, _out, _, _ = parse_circuit(data)
                            fd.write(f"{_in:032b}|{_out:0176b}" + '\n')
                try:
                    if len(client_queue) > 0:
                        self.client.sendall(client_queue.pop(0))
                except Exception as e:
                    pass
                self.client.sendall(data)

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
            if data:
                if not client_white:
                    if data[0:2] not in client_black:
                        print(f"[server:{self.port}] => {data[:].hex()}")
                elif client_white:
                    if data[0:2] in server_white and PARSE_CURCUIT:
                        pass
                    elif data[0:2] in client_white:
                        print(f"[server:{self.port}] => {data[:].hex()}")
                try:
                    if len(server_queue) > 0:
                        self.server.sendall(server_queue.pop(0))
                except Exception as e:
                    pass
                self.server.sendall(data)

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
    PORT = 3000
    for port in range(PORT, PORT+11):
        _game_server = Proxy('0.0.0.0', '127.0.0.1', port)
        _game_server.start()
        game_servers.append(_game_server)

    server_black += [b'\x00\x00', b'\x70\x73']
    client_black += [b'\x6d\x76', b'\x6a\x70']

    client_white += [b'\x30\x31']
    server_white += [b'\x30\x31']

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
                c += [pak := 
                        b'\x6d\x76' +
                            struct.pack('<fff', egg[9][0], egg[9][1], egg[9][2]) +
                            b'\x00\x00' * 3 +
                            b'\x00' * 2 +
                        b'\x65\x65' + 
                            struct.pack('<I', 0xb+9)
                    ]
            elif(cmd == "circuit_log"):
                PARSE_CURCUIT = True
            elif(cmd == "stage"):
                server_queue += [bytes.fromhex('3031060053746167653100000000')] # stage1
                time.sleep(0.4)
                server_queue += [bytes.fromhex('3031060053746167653203000000')] # stage2
                time.sleep(0.4)
                server_queue += [bytes.fromhex('3031060053746167653300000000')] # stage3
                time.sleep(0.4)
                server_queue += [bytes.fromhex('3031060053746167653402000000')] # stage4
            elif(cmd == "final"):
                # fd = open('training.data', 'a')
                PARSE_CURCUIT = True
                packet = '30310a0046696e616c5374616765'
                pattern = '0_1_1_0_1_0_1_1_1_1_1_1_1_1_1_1_'
                for i in range(0x10000):
                    if FIND_CIRCUIT:
                        server_queue = []
                        break
                    d = f'{i:016b}'
                    bf = int(pattern.replace('_', '{}').format(*d), 2)
                    server_queue += [bytes.fromhex(packet) + struct.pack('<I', bf)]
                    time.sleep(0.4)
                # i == 37660 == 0b01101001100011111010101111111010
            elif(cmd[:3] == "cmd"):
                server_queue += [bytes.fromhex(cmd[4:])]
        except KeyboardInterrupt:
            os._exit(0)
        except Exception as e:
            print(e)
            os._exit(-1)