#include "ipc.h"
int main(int argc, char *argv[]){
	srand((unsigned) time(NULL));
	struct msqid_ds msg_sofa_info;
	Msg_buf msg_arg;
	sem_flg = IPC_CREAT | 0644;
	s_account_key=100;
	s_account_val=1;
	s_account_sem=set_sem(s_account_key,s_account_val,sem_flg);
	s_customer_key=200;
	s_customer_val=0;
	s_customer_sem=set_sem(s_customer_key,s_customer_val,sem_flg);
	q_flg=IPC_CREAT | 0644;
	q_sofa_key=300;
	q_sofa_id=set_msq(q_sofa_key,q_flg);
	q_wait_key=400;
	q_wait_id=set_msq(q_wait_key,q_flg);
	int pid[3];
	int i;
	for(i=0;i<3;i++){
		pid[i]=fork();
		if(pid[i]==0){
			while(1){
				msgctl(q_sofa_id, IPC_STAT, &msg_sofa_info);
				if(msg_sofa_info.msg_qnum==0)
					printf("Barber %d is sleeping.\n",getpid());
				msgrcv(q_sofa_id,&msg_arg,sizeof(msg_arg),SOFA,0);
				up(s_customer_sem);
				printf("Barber %d is working for the customer %d.\n",getpid(),msg_arg.mid);
				sleep(rand()%4+4);
				down(s_account_sem);
				printf("Barber %d is charging for the customer %d.\n",getpid(),msg_arg.mid);
				up(s_account_sem);
			}
		}
	}
	return EXIT_SUCCESS;
}
