#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <string.h>


using namespace std;

void errquit(const char *msg){
	perror(msg);
	exit(-1);
}

void dump_code(long addr, long code){
	fprintf(stderr, "## %lx: code = %02x %02x %02x %02x %02x %02x %02x %02x\n",
		addr,
		((unsigned char *) (&code))[0],
		((unsigned char *) (&code))[1],
		((unsigned char *) (&code))[2],
		((unsigned char *) (&code))[3],
		((unsigned char *) (&code))[4],
		((unsigned char *) (&code))[5],
		((unsigned char *) (&code))[6],
		((unsigned char *) (&code))[7]);
}
unsigned long to_hex(int c)
{
        unsigned long num = (long) c;
        if(num < 58 && num > 47)    return num - 48; 
        if(num < 103 && num > 96)   return num - 87;
        return num;
}

int main(int argc, char *argv[]){
	pid_t child;
	// if(argc < 2) {
	// 	fprintf(stderr, "usage: %s program [args ...]\n", argv[0]);
	// 	return -1;
	// }
	if((child = fork()) < 0) errquit("fork");
	if(child == 0) {
		if(ptrace(PTRACE_TRACEME, 0, 0, 0) < 0) errquit("ptrace@child");
		// execvp(argv[1], argv+1);
        execlp("./no_more_traps", "./no_more_traps", NULL);
		errquit("execvp");
	} else {
		long long INT_cont = 0LL, counter = 0LL;
		int wait_status;
        unsigned char INT[1] = {0xcc};
        unsigned long c1, c2, hex;
        FILE *fptr = fopen("no_more_traps.txt", "r");

		if(waitpid(child, &wait_status, 0) < 0) errquit("waitpid");
		ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_EXITKILL);
        ptrace(PTRACE_CONT, child, 0, 0);
        
        while(waitpid(child, &wait_status, 0)>0){
            struct user_regs_struct regs;
            if(!WIFSTOPPED(wait_status)) continue;
			if(ptrace(PTRACE_GETREGS, child, 0, &regs) != 0)    errquit("ptrace(GETREGS)");

            regs.rip = regs.rip-1;
            unsigned long code = ptrace(PTRACE_PEEKTEXT, child, regs.rip, 0);

            if(memcmp((unsigned char*)&code, INT, sizeof(INT))==0){
                INT_cont++;
                // dump_code(regs.rip, code);

                c1 = to_hex(fgetc(fptr));
                c2 = to_hex(fgetc(fptr));
                hex = c1<<4 | c2;
                if(ptrace(PTRACE_POKETEXT, child, regs.rip, (code & 0xffffffffffffff00) | hex) != 0)
                    errquit("ptrace(POKETEXT)");

                code = ptrace(PTRACE_PEEKTEXT, child, regs.rip, 0);
                // dump_code(regs.rip, code);

                if(ptrace(PTRACE_SETREGS, child, 0, &regs) != 0) errquit("ptrace(SETREGS)");
            }
            ptrace(PTRACE_CONT, child, 0, 0);
        }
        fprintf(stderr, "## %lld cc instruction(s) executed\n", INT_cont);
	}
    return 0;
}