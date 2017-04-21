#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/time.h>
using namespace std;
/*信号灯控制用的共同体*/
typedef union semuns {
	int val;
} Sem_uns;
//管程中使用的信号量
class Sema{
	public:
		Sema(int id);
		~Sema();
		int down(); //信号量加 1
		int up(); //信号量减 1
	private:
		int sem_id; //信号量标识符
};
//管程中使用的锁
class Lock{
	public:
		Lock(Sema *lock);
		~Lock();
		void close_lock();
		void open_lock();
	private:
		Sema *sema; //锁使用的信号量
};
//管程中使用的条件变量
class Condition{
	public:
		Condition(Sema *smn, Sema *sms);
		~Condition();
		void Wait(Lock *lock,int dir); 
		int Signal(int dir); 
	private:
		Sema *seman; 
		Sema *semas;
};
//管程的定义
class dp{
	public:
		dp(int rate,int maxn);
		//管程构造函数
		~dp();
		void departure(int dir);
		void running(int dir);
		void arrival(int dir);
		//建立或获取 ipc 信号量的一组函数的原型说明
		int get_ipc_id(char *proc_file,key_t key);
		int set_sem(key_t sem_key,int sem_val,int sem_flag);
		//创建共享内存,放状态
		char *set_shm(key_t shm_key,int shm_num,int shm_flag);
		int *waitn,*waits,*sumco;
	private:
		int rate ;
		//控制执行速度
		Lock *lock;
		//控制互斥进入管程的锁
		Condition *cond; //控制状态的条件变量
		int *uppernum,*runnum,*dirnow;
};











