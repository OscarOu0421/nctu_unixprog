/*
ul+lu: convert the alphabet in CH from upper to lower or from lower to upper
======
======
*/

/*
If we compare the ASCII codes of capital A and lowercase a, it becomes clear that only bit 5 is different:
    0 1 1 0 0 0 0 1 = 61h ('a') 
    0 1 0 0 0 0 0 1 = 41h ('A')
*/
    xor ch, 0x20
done: