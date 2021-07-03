#!/usr/bin/python

# x86_64 disassemble quizzes
#  sample quiz: 584809f8504889fbfd
#  sample answer: 706f70207261780a6f72207261782c207264690a70757368207261780a6d6f76207262782c207264690a7374640a

import binascii
import sys
from capstone import *
from pwn import *

r = remote('up.zoolab.org' , 2530)
while 1:
    try:
        print("**********")
        z = r.recvuntil('Your answer: ').decode('utf-8').split()
        quiz = z[-3]
        print("quiz: {}".format(quiz))
        code = binascii.a2b_hex(z[-3])
        ans = ''

        md = Cs(CS_ARCH_X86, CS_MODE_64)
        for i in md.disasm(code, 0x1000):
            ans = ans + "{} {}\n".format(i.mnemonic, i.op_str)

        print(ans)
        ans = binascii.b2a_hex(ans.encode('ascii')).decode('utf-8').split()
        print("answer: {}".format(ans[0])) 

        r.sendline(ans[0])
        print("**********\n\n")
    except EOFError as e:
        print(e)
        break

r.interactive()