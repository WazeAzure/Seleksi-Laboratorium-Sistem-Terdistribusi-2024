from Crypto.Cipher import AES
from secrets import token_bytes

FLAG = b'Sister{?????}'
KEY = token_bytes(16)
IV = token_bytes(16)

def encrypt(plaintext):
    cipher = AES.new(KEY, AES.MODE_OFB, IV)
    return cipher.encrypt(plaintext)

leak = b'Shikanoko nokonoko koshitantan! Shikanoko nokonoko koshitantan!'

ct1 = encrypt(FLAG)
ct2 = encrypt(leak)

print(f'ct1: {ct1.hex()}')
print(f'ct2: {ct2.hex()}')