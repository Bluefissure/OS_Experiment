#include "ipc.h";
int main(int argc, char *argv[]) {
	srand((unsigned) time(NULL));
	Msg_buf msg_arg;
	struct msqid_ds msg_sofa_info;
	struct msqid_ds msg_wait_info;
	q_flg = IPC_CREAT | 0644;
	q_sofa_key = 300;
	q_sofa_id = set_msq(q_sofa_key, q_flg);
	q_wait_key = 400;
	q_wait_id = set_msq(q_wait_key, q_flg);
	sem_flg = IPC_CREAT | 0644;
	s_account_key = 100;
	s_account_val = 1;
	s_account_sem = set_sem(s_account_key, s_account_val, sem_flg);
	s_customer_key = 200;
	s_customer_val = 0;
	s_customer_sem = set_sem(s_customer_key, s_customer_val, sem_flg);
	int customerNumber = 1;
	while (1) {
		msgctl(q_sofa_id, IPC_STAT, &msg_sofa_info);
		if (msg_sofa_info.msg_qnum < 4) {
			quest_flg = IPC_NOWAIT; 
			if (msgrcv(q_wait_id, &msg_arg, sizeof(msg_arg), WAIT, quest_flg) >= 0) {
				msg_arg.mtype = SOFA;
				printf("Customer %d has sitted on the sofa.\n", msg_arg.mid);
				msgsnd(q_sofa_id, &msg_arg, sizeof(msg_arg), IPC_NOWAIT);
			} else {
				msg_arg.mtype = SOFA;
				msg_arg.mid = customerNumber;
				customerNumber++;
				printf("Customer %d has sitted on the sofa.\n", msg_arg.mid);
				msgsnd(q_sofa_id, &msg_arg, sizeof(msg_arg),IPC_NOWAIT);
			}
		} else {
			msgctl(q_wait_id, IPC_STAT, &msg_wait_info);
			if (msg_wait_info.msg_qnum < 13) {
				msg_arg.mtype = WAIT;
				msg_arg.mid = customerNumber;
				printf("Customer %d has waited in the waiting room.\n",customerNumber);
			customerNumber++;
			msgsnd(q_wait_id, &msg_arg, sizeof(msg_arg), IPC_NOWAIT);
			} else {
				printf("Customer %d has leaved the barber shop without any operation.\n", customerNumber);
				down(s_customer_sem);
			}
		}
		sleep(rand()%2+1);
	}
	return EXIT_SUCCESS;
}
