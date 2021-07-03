/*
loop15:
	str1 is a string contains 15 lowercase and uppercase alphbets.
	implement a loop to convert all alplabets to lowercase,
	and store the result in str2.
======
      str1 @ 0x600000-600010
      str2 @ 0x600010-600020
======
*/
/*
If we compare the ASCII codes of capital A and lowercase a, it becomes clear that only bit 5 is different:
    0 1 1 0 0 0 0 1 = 61h ('a') 
    0 1 0 0 0 0 0 1 = 41h ('A')
*/
/*	Method	1	*/
	mov ecx, 15
L1:
	mov al, [0x600000+ecx-1]
	cmp al, 'a'
	jae L2
	add	al,	0x20
	mov [0x600010+ecx-1], al
	loop L1
L2:
	mov [0x600010+ecx-1], al
	loop L1
done:


/*	Method	2	*/
; 	mov	rax,	[0x600000]
; 	mov	rcx,	8
; L1:/*	handle	8-bytes	*/
; 	cmp	al,		'a'
; 	jl	tolower1
; L2:
; 	rol	rax,	8
; 	loop	L1
; 	mov	[0x600010],	rax
; 	mov	eax,	[0x600008]
; 	mov	rcx,	4
; 	jmp	L3
; tolower1:
; 	xor al, 	0x20
; 	jmp	L2

; L3:/*	handle	4-bytes	*/
; 	cmp	al,		'a'
; 	jl	tolower2
; L4:
; 	rol	eax,	8
; 	loop	L3
; 	mov	[0x600018],	eax
; 	mov	ax,		[0x60000c]
; 	mov	rcx,	2
; 	jmp	L5
; tolower2:
; 	xor al, 	0x20
; 	jmp	L4

; L5:/*	handle	2-bytes	*/
; 	cmp	al,		'a'
; 	jl	tolower3
; L6:
; 	rol	ax,	8
; 	loop	L5
; 	mov	[0x60001c],	ax
; 	mov	al,		[0x60000e]
; 	jmp	L7
; tolower3:
; 	xor al, 	0x20
; 	jmp	L6
; L7:/*	handle	1-bytes	*/
; 	cmp	al,		'a'
; 	jge	over
; 	xor al, 	0x20	
; over:
; 	mov	[0x60001e],	al
	
; done: