CC = gcc
CXX = g++
CFLAGS = -c -g -Wall -fno-stack-protector -nostdlib
DYLINK = ld -m elf_x86_64 --dynamic-linker /lib64/ld-linux-x86-64.so.2

ASM64	= yasm -f elf64 -DYASM -D__x86_64__ -DPIC

PROGS = alarm1 alarm2 alarm3 jmp1

all: libmini.so

%.o: %.asm
	$(ASM64) $< -o $@

libmini.so:	libmini64.asm libmini.c start.o
	$(ASM64) $< -o libmini64.o
	$(CC) -c -g -Wall -fno-stack-protector -fPIC -nostdlib libmini.c
	ar rc libmini.a libmini64.o libmini.o
	ld -shared -o libmini.so libmini64.o libmini.o

alarm1:	alarm1.c start.o libmini.so
	$(CC) $(CFLAGS) -I. -I.. -DUSEMINI alarm1.c
	$(DYLINK) -o alarm1 alarm1.o start.o -L. -L.. -lmini
	rm alarm1.o

alarm2: alarm2.c start.o libmini.so
	$(CC) $(CFLAGS) -I. -I.. -DUSEMINI alarm2.c
	$(DYLINK) -o alarm2 alarm2.o start.o -L. -L.. -lmini
	rm alarm2.o

alarm3: alarm3.c start.o libmini.so
	$(CC) $(CFLAGS) -I. -I.. -DUSEMINI alarm3.c
	$(DYLINK) -o alarm3 alarm3.o start.o -L. -L.. -lmini
	rm alarm3.o

jmp1: jmp1.c start.o libmini.so
	$(CC) -o jmp1.o $(CFLAGS) -I. -I.. -DUSEMINI jmp1.c
	$(DYLINK) -o jmp1 jmp1.o start.o -L. -L.. -lmini
	rm jmp1.o

clean:
	rm -f *.o *.so *.a $(PROGS)