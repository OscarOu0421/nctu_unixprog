/*
eval1:
	Rval = -Xval + (Yval – Zval)
======
      Xval @ 0x600000-600004
      Yval @ 0x600004-600008
      Zval @ 0x600008-60000c
      Rval @ 0x60000c-600010
======
*/
    mov eax,    [0x600000]
    neg eax
    add eax,    [0x600004]
    sub eax,    [0x600008]
    mov [0x60000c], eax
done: