#include "ipc.h"
int main(int argc,char *argv[])
{
	int rate;
	if(argv[1] != NULL) rate = atoi(argv[1]);
	else rate = 3; 
	smok1_key = 101;
	smok2_key = 102;
	smok3_key = 103;
	prov_key = 201;
	mesg_key = 301;
	sem_flg = IPC_CREAT | 0644;

	sem_val = 1;
	smok1_sem = set_sem(smok1_key,sem_val,sem_flg);
	smok2_sem = set_sem(smok2_key,sem_val,sem_flg);
	smok3_sem = set_sem(smok3_key,sem_val,sem_flg);
	sem_val = 2;
	prov_sem = set_sem(prov_key,sem_val,sem_flg);
	mesg_id=set_msq(mesg_key,sem_flg);

	int i=0;
	while (1){
		for (i=1;i<=3;i++){
			msg_buf.mtype=i;
			msgrcv(mesg_id,&msg_buf,1,i,0);
			if (i==1){
				printf("The provider %d got a request from Smoker %d.\n",getpid(),msg_buf.mtext[0]);
				down(smok1_sem);
			}
			if (i==2){
				printf("The provider %d got a request from Smoker %d.\n",getpid(),msg_buf.mtext[0]);
				down(smok2_sem);
			}
			if (i==3){
				printf("The provider %d got a request from Smoker %d.\n",getpid(),msg_buf.mtext[0]);
				down(smok3_sem);
			}
			sleep(rate);
			msg_buf.mtype=i+3;
			msg_buf.mtext[0]=getpid();
			if (i==1) {
				msgsnd(mesg_id,&msg_buf,1,0);
				printf("The provider %d provides glue and paper for Smoker %d.\n",getpid(),msg_buf.mtext[0]);
			}
			if (i==2) 
			{
				msgsnd(mesg_id,&msg_buf,1,0);
				printf("The provider %d provides glue and tobacoo for Smoker %d.\n",getpid(),msg_buf.mtext[0]);
			}
			if (i==3) 
			{
				msgsnd(mesg_id,&msg_buf,1,0);
				printf("The provider %d provides tobacoo and paper for Smoker %d.\n",getpid(),msg_buf.mtext[0]);
			}
			up(prov_sem);
		}
	}

	return EXIT_SUCCESS;

}
