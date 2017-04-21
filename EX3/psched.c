#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <wait.h>
#include <unistd.h>
#include <signal.h>

typedef void (*sighandler_t) (int);
void ADD(){
	int pri=getpriority(PRIO_PROCESS,0);
	int ppri=pri+1;
	setpriority(PRIO_PROCESS,getpid(),ppri);
	printf("Change PID = %d from priority = %d to %d\n",getpid(),pri,ppri);
}
void MINUS(){
	int pri=getpriority(PRIO_PROCESS,0);
	int ppri=pri-1;
	setpriority(PRIO_PROCESS,getpid(),ppri);
	printf("Change PID = %d from priority = %d to %d\n",getpid(),pri,ppri);
}

int pid;
int stk[2],top=0;
void check(int a,int b)
{
	if(a&&b){
		if(pid) ADD();
	}else if(a&&!b){
		if(pid) MINUS();
	}else if(!a&&b){
		if(!pid) ADD();
	}else{
		if(!pid) MINUS();
	}
}
void BC(){
	stk[top++]=1;
	if(top>=2){
		check(stk[0],stk[1]);
		top=0;
	}
}
void BZ(){
	stk[top++]=0;
	if(top>=2){
		check(stk[0],stk[1]);
		top=0;
	}
}

int main(int argc, char *argv[])
{
  int i,j,status;
  signal(SIGINT,(sighandler_t)BC);
  signal(SIGTSTP,(sighandler_t)BZ);
  pid=fork();
  if(pid==0){
	while(1){
        printf("PID = %d policy = %d priority = %d\n",getpid(),sched_getscheduler(getpid()),getpriority(PRIO_PROCESS,0));
		sleep(1);
	}
  }else{
	while(1){
        printf("PID = %d policy = %d priority = %d\n",getpid(),sched_getscheduler(getpid()),getpriority(PRIO_PROCESS,0));
		sleep(1);
	}
  }
  return EXIT_SUCCESS;
}
