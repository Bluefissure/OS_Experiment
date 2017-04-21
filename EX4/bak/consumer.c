#include"ipc.h"
int main(int argc,char* argv[])
{
	int rate;
	if((argv[1]!=NULL)) rate=atoi(argv[1]);
	else rate=3;
	buff_key=101;
	buff_num=8;
	cget_key=103;
	cget_num=1;
	shm_flg=IPC_CREAT|0644;

	buff_ptr=(char *)set_shm(buff_key,buff_num,shm_flg);
	cget_ptr=(int *)set_shm(cget_key,cget_num,shm_flg);

	prod_key=201;
	pmtx_key=202;
	cons_key=301;
	cmtx_key=302;
	sem_flg=IPC_CREAT|0644;

	sem_val=buff_num;
	prod_sem=set_sem(prod_key,sem_val,sem_flg);

	sem_val=0;
	cons_sem=set_sem(cons_key,sem_val,sem_flg);

	sem_val=1;
	cmtx_sem=set_sem(cmtx_key,sem_val,sem_flg);
	while(1){
		down(cons_sem);
		down(cmtx_sem);
		sleep(rate);
		printf("%d consumer get %c from Buffer[%d]\n",getpid(),buff_ptr[*cget_ptr],*cget_ptr);
		*cget_ptr=(*cget_ptr+1)%buff_num;
		up(cmtx_sem);
		up(prod_sem);
	}
	return EXIT_SUCCESS;
}
