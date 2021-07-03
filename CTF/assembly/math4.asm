/*
math4: 32-bit signed arithmetic
	var4 = (var1 * -5) / (-var2 % var3)
	note: overflowed part should be truncated
======
      var1 @ 0x600000-600004
      var2 @ 0x600004-600008
      var3 @ 0x600008-60000c
      var4 @ 0x60000c-600010
======
*/
    mov     eax,    [0x600000]
    mov     ebx,    -5
    imul    ebx
    mov     ebx,    eax

    mov     eax,    [0x600004]
    neg     eax
    cdq
    mov     ecx,    [0x600008]
    idiv    ecx

    mov     eax,    ebx
    mov     ebx,    edx
    cdq
    idiv    ebx
    mov     [0x60000c], eax
done: