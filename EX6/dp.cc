#include "dp.h"
Sema::Sema(int id)
{
	sem_id = id;
}
Sema::~Sema(){ }
/*
 * 信号灯上的 down/up 操作
 * semid:信号灯数组标识符
 * semnum:信号灯数组下标
 * buf:操作信号灯的结构
 */
int Sema::down()
{
	struct sembuf buf;
	buf.sem_op = -1;
	buf.sem_num = 0;
	buf.sem_flg = SEM_UNDO;
	if((semop(sem_id,&buf,1)) <0) {
		perror("down error ");
		exit(EXIT_FAILURE);
	}
	return EXIT_SUCCESS;
}
int Sema::up()
{
	Sem_uns arg;
	struct sembuf buf;
	buf.sem_op = 1;
	buf.sem_num = 0;
	buf.sem_flg = SEM_UNDO;
	if((semop(sem_id,&buf,1)) <0) {
		perror("up error ");
		exit(EXIT_FAILURE);
	}
	return EXIT_SUCCESS;
}
/*
 * 用于管程的互斥执行
 */
Lock::Lock(Sema * s)
{
	sema = s;
}
Lock::~Lock(){ }
//上锁
void Lock::close_lock()
{
	sema->down();
}
//开锁
void Lock::open_lock()
{
	sema->up();
}
/*
 * get_ipc_id() 从/proc/sysvipc/文件系统中获取 IPC 的 id 号
 * pfile: 对应/proc/sysvipc/目录中的 IPC 文件分别为
 *
 msg-消息队列,sem-信号量,shm-共享内存
 * key: 对应要获取的 IPC 的 id 号的键值
 */
int dp::get_ipc_id(char *proc_file,key_t key)
{
#define BUFSZ 256
	FILE *pf;
	int i,j;
	char line[BUFSZ],colum[BUFSZ];
	if((pf = fopen(proc_file,"r")) == NULL){
		perror("Proc file not open");
		exit(EXIT_FAILURE);
	}
	fgets(line, BUFSZ,pf);
	while(!feof(pf)){
		i = j = 0;
		fgets(line, BUFSZ,pf);
		while(line[i] == ' ') i++;
		while(line[i] !=' ') colum[j++] = line[i++];
		colum[j] = '\0';
		if(atoi(colum) != key) continue;
		j=0;
		while(line[i] == ' ') i++;
		while(line[i] !=' ') colum[j++] = line[i++];
		colum[j] = '\0';
		i = atoi(colum);
		fclose(pf);
		return i;
	}
	fclose(pf);
	return -1;
}
/*
 *
 set_sem 函数建立一个具有 n 个信号灯的信号量
 *
 如果建立成功,返回 一个信号量的标识符 sem_id
 *
 输入参数:
 *
 sem_key 信号量的键值
 *
 sem_val 信号量中信号灯的个数
 *
 sem_flag 信号量的存取权限
 */
int dp::set_sem(key_t sem_key,int sem_val,int sem_flg)
{
	int sem_id;
	Sem_uns sem_arg;
	//测试由 sem_key 标识的信号量是否已经建立
	if((sem_id=get_ipc_id("/proc/sysvipc/sem",sem_key)) < 0 ){
		//semget 新建一个信号灯,其标号返回到 sem_id
		if((sem_id = semget(sem_key,1,sem_flg)) < 0){
			perror("semaphore create error");
			exit(EXIT_FAILURE);
		}
	}
	//设置信号量的初值
	sem_arg.val = sem_val;
	if(semctl(sem_id,0,SETVAL,sem_arg) < 0){
		perror("semaphore set error");
		exit(EXIT_FAILURE);
	}
	return sem_id;
}
/*
 *
 set_shm 函数建立一个具有 n 个字节 的共享内存区
 *
 如果建立成功,返回 一个指向该内存区首地址的指针 shm_buf
 *
 输入参数:
 *
 shm_key 共享内存的键值
 *
 shm_val 共享内存字节的长度
 *
 shm_flag 共享内存的存取权限
 */
char * dp::set_shm(key_t shm_key,int shm_num,int shm_flg)
{
	int i,shm_id;
	char * shm_buf;
	//测试由 shm_key 标识的共享内存区是否已经建立
	if((shm_id=get_ipc_id("/proc/sysvipc/shm",shm_key))<0){
		//shmget 新建 一个长度为 shm_num 字节的共享内存
		if((shm_id= shmget(shm_key,shm_num,shm_flg)) <0){
			perror("shareMemory set error");
			exit(EXIT_FAILURE);
		}
		//shmat 将由 shm_id 标识的共享内存附加给指针 shm_buf
		if((shm_buf=(char *)shmat(shm_id,0,0)) < (char *)0){
			perror("get shareMemory error");
			exit(EXIT_FAILURE);
		}
		for(i=0; i<shm_num; i++) shm_buf[i] = 0; //初始为 0
	}
	//共享内存区已经建立,将由 shm_id 标识的共享内存附加给指针 shm_buf
	if((shm_buf = (char *)shmat(shm_id,0,0)) < (char *)0) {
		perror("get shareMemory error");
		exit(EXIT_FAILURE);
	}
	return shm_buf;
}

Condition::Condition(Sema *smn, Sema *sms){
	seman = smn;
	semas = sms;
}
void Condition::Wait(Lock *lock,int dir){
	if (dir) {
		cout << "Train "<<getpid() <<"is waiting for departing for the south city." << endl;
		lock->open_lock();
		semas->down();
		lock->close_lock();
	}
	else
	{
		cout << "Train "<<getpid()<< "is waiting for departing for the north city." << endl;
		lock->open_lock();
		seman->down();
		lock->close_lock();
	}
}
int Condition::Signal(int dir) {
	int temp;
	if (dir) {
		temp = semas->up();
	} 
	else {
		temp = seman->up();
	}
	return temp;
}
Condition::~Condition(){}

dp::dp(int ra,int maxn)
{
	int ipc_flg = IPC_CREAT | 0644;
	uppernum = (int *) set_shm(101, 1, ipc_flg); 
	runnum = (int *) set_shm(102, 1, ipc_flg); 
	dirnow = (int *) set_shm(103, 1, ipc_flg); 
	sumco = (int *) set_shm(104, 1, ipc_flg);
	waitn = (int *) set_shm(201, 1, ipc_flg);
	waits = (int *) set_shm(202, 1, ipc_flg);
	int seman_id = set_sem(301, 0, ipc_flg);
	int semas_id = set_sem(302, 0, ipc_flg);
	int selo_id = set_sem(401, maxn, ipc_flg);
	Sema *seman;
	Sema *semas;
	Sema *selo;

	*uppernum = maxn;
	*runnum = 0;
	*dirnow = 0;
	*sumco = 0;
	*waitn = 0;
	*waits = 0;
	seman = new Sema(seman_id);
	semas = new Sema(semas_id);
	selo = new Sema(selo_id);
	lock = new Lock(selo);
	cond = new Condition(seman, semas);
}
void dp::departure(int dir) 
{
	lock->close_lock();
	if(*sumco > 0){
		if ((*dirnow != dir||*runnum >= *uppernum)) 
		{
			if (dir) {
				*waits++;
			} 
			else
			{
				*waitn++;
			}}
		cond->Wait(lock, dir);
	}
	if (dir) 
	{
		*waits--;
		cout << "Train "<<getpid() << " has got onto the railway to the south city.\n";
	} 
	else 
	{
		*waitn--;
		cout << "Train "<<getpid() << " has got onto the railway to the north city.\n";
	}
	*runnum++;
	*dirnow=dir;
	*sumco++;
	lock->open_lock();
}
void dp::running(int dir) {
	lock->close_lock();
	sleep(1);
	if (dir)
		cout << "Train "<<getpid() << " is running to the south city.\n";
	else
		cout << "Train "<<getpid() << " is running to the north city.\n";
	lock->open_lock();
}
void dp::arrival(int dir) {
	lock->close_lock();
	*runnum--;
	if (dir) 
		cout << "Train "<<getpid() << " has arrived in the south city.\n";
	else{
		cout << "Train "<<getpid() << " has arrived in the nouth city.\n";
	}
	if (!*runnum && !dir && waits) 
		cond->Signal(1);
	else if (!*runnum && !dir && waitn) 
		cond->Signal(0);
	else if (!*runnum && dir && waits) 
		cond->Signal(1);
	else if (!*runnum && dir && waitn) 
		cond->Signal(0);
	lock->open_lock();
}
dp::~dp() {
	delete cond;
}
int main(int argc, char **argv) {
	int totn,maxn,rate;
	cout << "Please input the number of trains:";
	cin >> totn;
	cout << endl<<"Please input the maximum number of the trains runing on the way:";
	cin >> maxn;
	//cout<<endl<< "Please input the rate of the process running:";
	//cin >> rate;
	cout<<endl;
	dp *Dp = new dp(rate,maxn);
	int i,seed;
	int pid[totn];
	struct timeval tv;  
    struct timezone tz = {0, 0};  
    /* tv.tv_usec 是一个微秒级的时间 */  
    seed = tv.tv_usec % 65536;  
	for (i = 0; i < totn; i++) {
		pid[i] = fork();
		if (pid[i] == 0) {
//			srand((unsigned int)time(NULL));
			gettimeofday(&tv, &tz);  
			seed = tv.tv_usec % 65536;  
			srand(seed);  
			sleep(1);
			int rr=rand();
//			printf("rand:%d\n",rr);
			int randir = rr%2;
			Dp->departure(randir);
			Dp->running(randir);
			Dp->arrival(randir);
			exit(EXIT_SUCCESS);
		}
	}
	for (i = 0; i < totn; i++) {
		waitpid(pid[i], NULL, 0);
	}
	cout<<"The operation has been accomplished."<<endl;
	delete Dp;
	return EXIT_SUCCESS;
}
