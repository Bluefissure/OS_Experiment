#include "ipc.h"
int main(int argc,char *argv[])
{
	int rate;
	if(argv[1] != NULL) rate = atoi(argv[1]);
	else rate = 3; 
	smok1_key = 101;
	prov_key = 201;
	mesg_key = 301;
	sem_flg = IPC_CREAT | 0644;

	sem_val = 1;
	smok1_sem = set_sem(smok1_key,sem_val,sem_flg);
	sem_val = 2;
	prov_sem = set_sem(prov_key,sem_val,sem_flg);
	mesg_id=set_msq(mesg_key,sem_flg);

	while (1)
	{
		down(prov_sem);
		msg_buf.mtype=1;
		msg_buf.mtext[0]=getpid();
		msgsnd(mesg_id,&msg_buf,1,0);
		printf("The smoker %d who owned glue sent a request to the providers.\n",getpid());
		sleep(rate);
		msg_buf.mtype=4;
		msgrcv(mesg_id,&msg_buf,1,4,0);
		printf("The smoker %d who owned glue got all ingredients from the provider %d.\n",getpid(),msg_buf.mtext[0]);
		printf("The smoker %d who owned glue just smoked.\n",getpid());
		up(smok1_sem);
	}
return EXIT_SUCCESS;

}
