/*
    bubble: bubble sort for 10 integers
    ======
        a[0] @ 0x600000-600004
        a[1] @ 0x600004-600008
        a[2] @ 0x600008-60000c
        a[3] @ 0x60000c-600010
        a[4] @ 0x600010-600014
        a[5] @ 0x600014-600018
        a[6] @ 0x600018-60001c
        a[7] @ 0x60001c-600020
        a[8] @ 0x600020-600024
        a[9] @ 0x600024-600028
    ======
*/
/*
    function bubble_sort (array, length) {
        var i, j;
        for(i from 0 to length-1){
            for(j from 0 to length-1-i){
                if (array[j] > array[j+1])
                    swap(array[j], array[j+1])
            }
        }
    }
*/
    mov ecx,    0                       /*  i=0             */
outloop:
    mov edx,    0                       /*  j=0             */
inloop:
    mov eax,    [0x600000+4*edx]        /*  eax = a[j]      */
    mov ebx,    [0x600000+4*edx+4]      /*  ebx = a[j+1]    */
    cmp eax,    ebx                     /*  if(a[j]>a[j+1]) */
    jg  exch                            /*  jmup exchange   */

check:
    inc edx                             /*  j++             */
    mov eax,    ecx                     /*  if(j>=(length-1-i)))    */
    neg eax                         
    add eax,    9
    cmp edx,    eax
    jge exitin                          /*  break   inloop  */
    jmp inloop
exch:
    mov [0x600000+4*edx],  ebx          /*  swap(a[j],a[j+1])*/
    mov [0x600000+4*edx+4],eax
    jmp check
    
exitin:
    inc ecx                             /*  i++             */
    cmp ecx,     9                      /*  if(i>=length-1) */
    jge exitout                         /*  break   outloop */
    jmp outloop
exitout:
    
done: