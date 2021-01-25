#include <sys/ptrace.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>


int main(int argc,char** args) {
    int pid = atoi(args[1]);
    long ret = ptrace(PTRACE_ATTACH,pid);
    if (ret == 0) {
        printf("ptrace success\n");
        sleep(3);
        printf("start waid pid\n");
        waitpid(pid,NULL,0);
        printf("has wait\n");
        int addr_value =  ptrace(PTRACE_PEEKDATA,pid,0x7ffcf41a9b74);
        printf("%d\n",addr_value);
        ptrace(PTRACE_POKEDATA,pid,0x7ffcf41a9b74,0);
    }else{
        printf("ptrace failed\n");
    }
    return 0;
}
