/*
math1: unsigned arithmetic
	var4 = (var1 + var2) * var3
======
      var1 @ 0x600000-600004
      var2 @ 0x600004-600008
      var3 @ 0x600008-60000c
      var4 @ 0x60000c-600010
======
*/
    mov eax,    [0x600000]
    mov ebx,    [0x600004]
    mov ecx,    [0x600008]
    add eax,    ebx
    mul ecx
    mov [0x60000c], eax
done:
    