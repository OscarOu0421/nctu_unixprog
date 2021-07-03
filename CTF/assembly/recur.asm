/*
recur: implement a recursive function

   r(n) = 0, if n <= 0
        = 1, if n == 1
        = 2*r(n-1) + 3*r(n-2), otherwise
   
   please call r(25) and store the result in RAX
======
======
*/ 
    push    25                      /*  call r(25)                  */
    call    r
    add     esp,    8               /*  restore stack address       */
    jmp     exit
r:
    push    rbp
    mov     rbp,    rsp
    mov     rbx,    [rbp+16]        /*  rbx=n                       */

    cmp     rbx,    0               /*  if n<=0: return 0           */
    jle     L1
    cmp     rbx,    1               /*  if n==1: return 1           */
    je      L2

    
    dec     rbx                     /*  call    r(n-1)              */
    push    rbx
    call    r
    add     esp,    8
Return1:
    mov     rcx,    2               /*  2*r(n-1)                    */
    mul     rcx
    mov     rdx,    rax             /*  save 2*r(n-1) into rdx      */
    push    rdx

    mov     rbx,    [rbp+16]        /*  call r(n-2)                 */
    sub     rbx,    2
    push    rbx
    call    r    
    add     esp,    8
Return2:
    mov     rcx,    3               /*  3*r(n-2)                    */
    mul     rcx                     
    pop     rdx                     /*  restore 2*r(n-1) from rdx   */
    add     rax,    rdx             /*  2*r(n-1)+3*r(n-2)           */
    pop     rbp
    ret   
L1:
    mov     rax,    0
    pop     rbp
    ret 
L2:
    mov     rax,    1
    pop     rbp
    ret  
exit:
done: