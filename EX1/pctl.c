#include "pctl.h"
int main(int argc, char *argv[])
{

	int i,pid_ls,pid_ps,status;
	char *args_ls[]={"/bin/ls","-a",NULL};
	char *args_ps[]={"/bin/ps","-a",NULL};
	signal(SIGINT,(sighandler_t)sigcat);
	pid_ls=fork();
	if(pid_ls<0){
		printf("Create Process fail!\n");
		exit(EXIT_FAILURE);
	}
	if(pid_ls==0){
		printf("I am Child process:ls %d\nMy father is %d\n",getpid(),getppid());
		pause();
		printf("%d child will Running:\n",getpid());
		for(i=0;args_ls[i]!=NULL;i++)
			printf("%s ",args_ls[i]);
		puts("");
		status=execve(args_ls[0],args_ls,NULL);
	}
	else{
		printf("I am Parent process %d\n",getpid());
		pid_ps=fork();
		if(pid_ps==0){
			printf("I am Child process:ps %d\nMy father is %d\n",getpid(),getppid());
			pause();
			printf("%d child will Running:\n",getpid());
			for(i=0;args_ps[i]!=NULL;i++)
				printf("%s ",args_ps[i]);
			puts("");
			status=execve(args_ps[0],args_ps,NULL);
		}else{
			usleep(1000);
			kill(pid_ps,SIGINT);
			waitpid(pid_ps,&status,0);
			printf("Child to process ps %d is done. status:%d\n",pid_ps,status);
			kill(pid_ls,SIGINT);
			waitpid(pid_ls,&status,0);
			printf("Child to process ls %d is done. status:%d\n",pid_ls,status);
			printf("Parent process exit.\n");

		}
	}
	return EXIT_SUCCESS;
}
