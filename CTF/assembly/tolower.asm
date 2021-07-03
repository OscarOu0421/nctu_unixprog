/*
tolower: convert the single character in val1 to uppercase and store in val2
======
      val1 @ 0x600000-600001
      val2 @ 0x600001-600002
======
*/

/*
If we compare the ASCII codes of capital A and lowercase a, it becomes clear that only bit 5 is different:
    0 1 1 0 0 0 0 1 = 61h ('a') 
    0 1 0 0 0 0 0 1 = 41h ('A')
*/
    mov al, [0x600000]
    sub al, 0x20
    mov [0x600001], al
done: