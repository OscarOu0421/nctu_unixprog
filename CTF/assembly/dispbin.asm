/*
dispbin:
	given a number in AX, store the corresponding bit string in str1.
	for example, if AX = 0x1234, the result should be:
	str1 = 0001001000111000
======
      str1 @ 0x600000-600014
======
*/
    mov ecx,    16
    mov dx,     ax
L1:
    shl ax,     1
    jc  is1
    mov bl,     0x30
    mov edx,    ecx
    neg edx
    mov [0x600000+edx+16],   bl
    loop    L1
is1:
    mov bl,     0x31
    mov edx,    ecx
    neg edx
    mov [0x600000+edx+16],   bl
    loop    L1
done: