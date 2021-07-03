/*
math5: 32-bit signed arithmetic
	var3 = (var1 * -var2) / (var3 - ebx)
	note: overflowed part should be truncated
======
      var1 @ 0x600000-600004
      var2 @ 0x600004-600008
      var3 @ 0x600008-60000c
======
*/
    mov     eax,    [0x600000]
    mov     ecx,    [0x600004]
    neg     ecx
    imul    ecx

    mov     ecx,    [0x600008]
    sub     ecx,    ebx

    cdq
    idiv    ecx
    mov     [0x600008], eax
done: