/*
math3: 32-bit unsigned arithmetic
	var4 = (var1 * 5) / (var2 - 3)
	note: overflowed part should be truncated
======
      var1 @ 0x600000-600004
      var2 @ 0x600004-600008
      var4 @ 0x600008-60000c
======
*/
    mov edx,    0
    mov eax,    [0x600000]
    mov ebx,    [0x600004]
    lea eax,    [eax+eax*4]
    sub ebx,    3
    div ebx
    mov [0x600008], eax
done: