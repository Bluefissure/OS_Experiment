#include<sys/types.h>
#include<wait.h>
#include<unistd.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
typedef void (*sighandler_t) (int);
void sigcat(){
	printf("%d Process continue\n",getpid());
}

