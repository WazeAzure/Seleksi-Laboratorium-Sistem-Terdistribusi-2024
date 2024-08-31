#!/usr/bin/python3
from pwn import *
from sys import argv

e = context.binary = ELF('chall_patched')
libc = ELF('libc.so.6', checksec=False)
ld = ELF('ld-2.35.so', checksec=False)
if len(argv) > 1:
    ip, port = argv[1].split(":")
    conn = lambda: remote(ip, port)
else:
    conn = lambda: e.process()

p = conn()

p.interactive()
