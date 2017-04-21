#include"ipc.h"
int main(int argc,char *argv[])
{
	int rate;
	if(argv[1]!=NULL) rate=atoi(argv[1]);
	else rate=3;
	buff_key=101;
	buff_num=8;
	pput_key=102;
	pput_num=1;
	shm_flg=IPC_CREAT|0644;
	buff_ptr=(char*) set_shm(buff_key,buff_num,shm_flg);
	pput_ptr=(int*) set_shm(pput_key,pput_num,shm_flg);

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
	pmtx_sem=set_sem(pmtx_key,sem_val,sem_flg);

	while(1){
		down(prod_sem);
		down(pmtx_sem);
		buff_ptr[*pput_ptr]='A'+*pput_ptr;
		sleep(rate);
		printf("%d producer put: %c to Buffer[%d]\n",getpid(),buff_ptr[*pput_ptr],*pput_ptr);
		*pput_ptr=(*pput_ptr+1)%buff_num;
		up(pmtx_sem);
		up(cons_sem);
	}
	return EXIT_SUCCESS;
}

