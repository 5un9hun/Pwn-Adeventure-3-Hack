from Crypto.Util.number import *
import os

def custom_base32_encode(dec):
    dec_length = len(dec)
    ret = [0 for _ in range(int(dec_length * (8 / 5)))]

    bit_i = 0
    for i in range(dec_length):
        for j in range(bit_i, bit_i+8):
            if(dec[i] & (1 << (j % 8))):
                ret[j // 5] |= 1 << (j % 5)
        bit_i += 8

    return ret

def custom_base32_decode(enc):
    enc_length = len(enc)
    ret = [0 for _ in range(enc_length * 5 // 8)]

    bit_i = 0
    for i in range(enc_length):
        for j in range(bit_i, bit_i+5):
            if(enc[i] & (1 << (j % 5))):
                ret[j >> 3] |= 1 << (j & 7)
        bit_i += 5

    return ret

class RSA:
    def __init__(self, p, q, e):
        self.e = e
        self.p = p
        self.q = q
        self.n = p * q
        self.phi = (p - 1) * (q - 1)
        self.d = inverse(e, self.phi)

    def encrypt(self, dec):
        return pow(dec, self.e, self.n)

    def decrypt(self, enc):
        return pow(enc, self.d, self.n)

alpha = '0123456789ABCDEFHJKLMNPQRTUVWXYZ'

N = 0x3C9921AC0185B3AAAE37E1B
e = 0x10001
p = 33759901540733
q = 34719860683127

rsa = RSA(p, q, e)

random_key = int.from_bytes(os.urandom(4), byteorder='little')
random_key_hex = hex((random_key ^ 0xAEB7037B) >> 2)[2:].rjust(8, '0')
prefix = b'PWNADV3'[::-1].hex()
enc = int(prefix + random_key_hex, 16)

dec = bytearray(rsa.decrypt(enc).to_bytes(12, byteorder='little'))
dec[11] |= random_key & 0xfc
dec += random_key.to_bytes(4, byteorder="little")[1:]

orig = custom_base32_encode(dec)

dlc_key = ''
checksum = 0

for k in orig:
    dlc_key += alpha[k]
    checksum += k

checksum &= 0x1f
dlc_key += alpha[checksum]

result = ''
for i in range(0, len(dlc_key), 5):
    result += dlc_key[i:i+5]
    if(i != len(dlc_key) - 5):
        result += '-'

print(result)



