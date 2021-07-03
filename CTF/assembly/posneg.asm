/*
posneg: test if registers are positive or negative.
	if ( eax >= 0 ) { var1 = 1 } else { var1 = -1 }
	if ( ebx >= 0 ) { var2 = 1 } else { var2 = -1 }
	if ( ecx >= 0 ) { var3 = 1 } else { var3 = -1 }
	if ( edx >= 0 ) { var4 = 1 } else { var4 = -1 } 
======
      var1 @ 0x600000-600004
      var2 @ 0x600004-600008
      var3 @ 0x600008-60000c
      var4 @ 0x60000c-600010
======
*/
IF1:
    cmp eax,    0
    jl  L1
    mov eax,    1
    mov [0x600000], eax
    jmp IF2
L1:
    mov eax,    -1
    mov [0x600000], eax

IF2:
    cmp ebx,    0
    jl  L2
    mov ebx,    1
    mov [0x600004], ebx
    jmp IF3

L2:
    mov ebx,    -1
    mov [0x600004], ebx
IF3:
    cmp ecx,    0
    jl  L3
    mov ecx,    1
    mov [0x600008], ecx
    jmp IF4

L3:
    mov ecx,    -1
    mov [0x600008], ecx
IF4:
    cmp edx,    0
    jl  L4
    mov edx,    1
    mov [0x60000c], edx
    jmp over

L4:
    mov edx,    -1
    mov [0x60000c], edx
over:
done: