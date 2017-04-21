#include<sys/types.h>
#include<wait.h>
#include<unistd.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
typedef void (*sighandler_t) (int);
void sigcat_SIGINT(){
	int pri=getpriority(PRIO_PROCESS,0);
	int ppri=pri+1;
	setpriority(PRIO_PROCESS,getpid(),ppri);
	printf("Change PID = %d from priority = %d to %d\n",getpid(),pri,ppri);
}
void sigcat_SIGTSTP(){
	int pri=getpriority(PRIO_PROCESS,0);
	int ppri=pri-1;
	setpriority(PRIO_PROCESS,getpid(),ppri);
	printf("Change PID = %d from priority = %d to %d\n",getpid(),pri,ppri);
}

