//filexm.c
#include "filexm.h"
void manu(char* data)
{
	int i;
	char ID[20],name[20],score[10];
	sscanf(data,"%s %s %s",ID,name,score);
	for(i=strlen(ID);i<20;i++) ID[i]=' ';
	for(i=strlen(name);i<20;i++) name[i]=' ';
	for(i=strlen(score);i<10;i++) score[i]=' ';
	ID[20]=name[20]=score[10]='\0';
	sprintf(data,"%s%s%s",ID,name,score);
}
int main(int argc, char *argv[])
{
	int fd; //打开文件的文件描述字
	struct stat buf;//保存打开文件信息的缓冲区
	char data[BUFSZ]; //文件读写缓冲区
	// 检查命令是否带有指定的文件名
	if(argc!=2){
		perror("USE AS ./filexm filename");
		exit(EXIT_FAILURE);
	}
	if((fd = open(argv[1],O_RDWR)) <0){
		perror("File not exist!");
		exit(EXIT_FAILURE);
	}
	if((fstat(fd,&buf)) < 0){
		perror("File stat error!");
		exit(EXIT_FAILURE);
	}
	while(1){
	printf("Enter a number:1-->append;2-->search;3-->insert;4-->delete;5-->exit\n");
	int i;
	scanf("%d",&i);
	if(i==5) break;
	switch(i){
		case 1:{
			printf("Input the information:\n");
			lseek(fd,0,SEEK_END);
			do gets(data);
			while(strlen(data)==0);
			manu(data);
			int err=write(fd,data,BUFSZ);
			if(err!=-1)
				printf("Append complete.\n");
			else puts("Error on append.");
			break;
			   }
		case 2:{
			printf("Input the number to search\n");
			scanf("%d",&j);
			if(-1!=lseek(fd,(j-1)*BUFSZ,SEEK_SET))
				read(fd,data,BUFSZ);
			else puts("Not found.");
			printf("%s",data);
			puts("");
			break;
			   }
		case 3:{
			printf("Input the number to insert\n");
			scanf("%d",&j);
			lseek(fd,(j-1)*BUFSZ,SEEK_SET);
			printf("Input the information:\n");
			do gets(data);
			while(strlen(data)==0);
			manu(data);
			write(fd,data,BUFSZ);
			printf("\n");
			break;
			   }
		case 4:{
			printf("Input the number to delete\n");
			scanf("%d",&j);
			lseek(fd,(j+1)*BUFSZ,SEEK_SET);
			read(fd,buff,1000);
			lseek(fd,(j-1)*BUFSZ,SEEK_SET);
			write(fd,buff,1000);
			   }
	}
	}
	//关闭文件
	if(close(fd) <0){
		perror("Close error");
		exit(EXIT_FAILURE);
	}
	return EXIT_SUCCESS;
}
