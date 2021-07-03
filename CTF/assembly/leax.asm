/*
leax:
	eax = edi * 2
	ebx = edi * 3
	ecx = edi * 5
	edx = edi * 9
======
======
*/
    lea eax,    [edi*2]
    lea ebx,    [edi+edi*2]
    lea ecx,    [edi+edi*4]
    lea edx,    [edi+edi*8]
done: