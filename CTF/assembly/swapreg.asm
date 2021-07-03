/*
swapreg: swap the values in RAX and RBX
======
======
*/
    mov RCX,    RAX
    mov RAX,    RBX
    mov RBX,    RCX
done: