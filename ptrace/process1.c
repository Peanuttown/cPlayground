#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
int main(){
    printf("pid: %d\n",getpid());
    int i =0;
    while (1) {
        i++;
        sleep(1);
        printf("%d, %p,\n",i,&i);
    }
}
