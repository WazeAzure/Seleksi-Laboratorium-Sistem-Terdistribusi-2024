from Crypto.Util.number import bytes_to_long as b2l, getPrime
from math import log2, gcd
import random

class MerkleHellmanKnapsack:
    def __init__(self, size):
        self.size = size
        
        self.W = [random.randrange(0, pow(2, 16))]
        while len(self.W) < self.size:
            w = random.randrange(sum(self.W) + 1, 2 * sum(self.W))
            assert w > sum(self.W)
            self.W.append(w)
            
        self.q = getPrime(sum(self.W).bit_length() + 1)
        self.r = random.randrange(self.q)
        while gcd(self.q, self.r) != 1:
            self.r = random.randrange(self.q)

        assert self.q > sum(self.W)

        self.B = []
        for i in range(len(self.W)):
            self.B.append((self.r * self.W[i]) % self.q)

    def get_pubkey(self):
        return self.B

    def calculate_density(self):
        return self.size / float(log2(max(self.B)))

    def encrypt(self, pt):
        assert pt.bit_length() <= self.size

        ct = 0
        for bi in self.B:
            ct += (pt & 1) * bi
            pt >>= 1
        
        return ct
    
    def decrypt(self, ct):
        ct = (ct * pow(self.r, -1, self.q)) % self.q

        pt = 0
        for i in range(len(self.W) - 1, -1, -1):
            if ct >= self.W[i]:
                pt |= 1 << i
                ct -= self.W[i]
        
        return pt


while True:
    cipher = MerkleHellmanKnapsack(270)

    if cipher.calculate_density() > 0.6463:
        break

FLAG = b'Sister{??????}'
pt = b2l(FLAG)

enc = cipher.encrypt(pt)
dec = cipher.decrypt(enc)

assert pt == dec

print(f'pubkey = {cipher.get_pubkey()}')
print(f'enc = {enc}')
