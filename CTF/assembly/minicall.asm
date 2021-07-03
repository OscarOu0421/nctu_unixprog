/*
minicall: implement a minimal function call in the emulator
===== THE CODE
    call   a
    jmp    exit

a:  ; function a - read ret-addr in rax
    pop    rax
    push   rax
    ret
exit:
======
======
*/
    call    a
    jmp     exit
a:
    pop     rax
    push    rax
    ret
exit:
done: