/*
math2: signed arithmetic
	eax = (-var1 * var2) + var3
======
      var1 @ 0x600000-600004
      var2 @ 0x600004-600008
      var3 @ 0x600008-60000c
======
*/
    mov eax,    [0x600000]
    mov ebx,    [0x600004]
    mov ecx,    [0x600008]
    neg eax
    mul ebx
    add eax,    ecx
done: