//vmrp.cc
#include "vmrp.h"
Replace::Replace(){
	int i;
	cout << "Please input page numbers :" ;
	cin >> PageNumber;
	ReferencePage = new int[sizeof(int) * PageNumber];
	EliminatePage = new int[sizeof(int) * PageNumber];
	cout << "Please input reference page string :";
	for (i = 0; i < PageNumber; i++)
		cin >> ReferencePage[i];
	cout << "Please input page frames :";
	cin >> FrameNumber;
	PageFrames = new int[sizeof(int) * FrameNumber];
	ReferenceBit=new int[sizeof(int) * FrameNumber];
	count=new int[sizeof(int)*FrameNumber];
	ModifyBit=new int[sizeof(int)*FrameNumber];
}
Replace::~Replace(){
}
void Replace::InitSpace(char * MethodName)
{
	int i;
	cout << endl << MethodName << endl;
	FaultNumber=0;
	//引用还未开始,-1表示无引用页
	for (i = 0; i < PageNumber; i++)
		EliminatePage[i] = -1;
	for(i = 0; i < FrameNumber; i++){
		PageFrames[i] = -1;
		ReferenceBit[i]=0;
		count[i]=0;
		ModifyBit[i]=0;
	}
}
//分析统计选择的算法对于当前输入的页面走向的性能
void Replace::Report(void){
	//报告淘汰页顺序
	cout << endl << "Eliminate page:";
	for(int i=0; EliminatePage[i]!=-1; i++) cout << EliminatePage[i] << " ";
	//报告缺页数和缺页率
	cout << endl << "Number of page faults = " << FaultNumber << endl;
	cout << setw(6) << setprecision(3) ;
	printf("Rate of page faults = %.10f%\n",100*(double)FaultNumber/PageNumber);
}
//最近最旧未用置换算法
void Replace::Lru(void)
{
	int i,j,k,l,next;
	InitSpace("LRU");
	//循环装入引用页
	for(k=0,l=0; k < PageNumber; k++){
		next=ReferencePage[k];
		//检测引用页当前是否已在实存
		for (i=0; i<FrameNumber; i++){
			if(next == PageFrames[i]){
				//引用页已在实存将其调整到页记录栈顶
				next= PageFrames[i];
				for(j=i;j>0;j--) PageFrames[j] = PageFrames[j-1];
				PageFrames[0]=next;
				break;
			}
		}
		if(PageFrames[0] == next){
			//如果引用页已放栈顶，则为不缺页，报告当前内存页号
			for(j=0; j<FrameNumber; j++)
				if(PageFrames[j]>=0) cout << PageFrames[j] << " ";
			cout << endl;
			continue; //继续装入下一页
		}
		else
			// 如果引用页还未放栈顶，则为缺页，缺页数加1
			FaultNumber++;
		//栈底页号记入淘汰页数组中
		EliminatePage[l] = PageFrames[FrameNumber-1];
		//向下压栈
		for(j=FrameNumber-1;j>0;j--) PageFrames[j]= PageFrames[j-1];
		PageFrames[0]=next; //引用页放栈顶
		//报告当前实存中页号
		for(j=0; j<FrameNumber; j++)
			if(PageFrames[j]>=0) cout << PageFrames[j] << " ";
		//报告当前淘汰的页号
		if(EliminatePage[l]>=0)
			cout << "->" << EliminatePage[l++] << endl;
		else
			cout << endl;
	}
	//分析统计选择的算法对于当前引用的页面走向的性能
	Report();
}
//先进先出置换算法
void Replace::Fifo(void){
	int i,j,k,l,next,ii;
	InitSpace("FIFO");
	for(k=0,j=l=0; k < PageNumber; k++){
		next=ReferencePage[k];
		//模拟栈的遍历
		for(ii=0; ii<FrameNumber; ii++){
			i=(j+ii)%FrameNumber;
			if(next==PageFrames[i]) break;
		}
		//访问成功
		if (ii<FrameNumber){
			for(ii=0; ii<FrameNumber; ii++){
				i=(j+ii)%FrameNumber;
				cout << PageFrames[i] << " ";
			}
			cout << endl;
			continue;
		}
		//访问失败
		FaultNumber++;
		EliminatePage[l]= PageFrames[j];
		PageFrames[j]=next;
		//指针后移，模拟队列
		j = (j+1)%FrameNumber;
		for(ii=0; ii<FrameNumber; ii++){
			i=(j+ii)%FrameNumber;
			if(PageFrames[i]>=0) 
				cout << PageFrames[i] << " ";
		}
		if(EliminatePage[l]>=0)
			cout << "->" << EliminatePage[l++] << endl;
		else
			cout << endl;
	}
	Report();
}
//二次机会置换算法
void Replace::Clock(void)
{
	int j,i,ii,k,l,next;
	InitSpace("Clock");
	for(k=0,j=l=0; k < PageNumber; k++){
		next=ReferencePage[k];
		for(ii=0; ii<FrameNumber; ii++){
			i=(j+ii)%FrameNumber;
			if(next==PageFrames[i]){
				ReferenceBit[i]=1;
				break;
			}
		}
		if (ii<FrameNumber){
			for(ii=0; ii<FrameNumber; ii++){
				i=(j+ii)%FrameNumber;
				cout << PageFrames[i] << " ";
			}
			cout << endl;
			continue;
		}
		//访问失败，找到第一个标识位为0的页面
		while (ReferenceBit[j]==1){
			ReferenceBit[j]=0;
			j=(j+1)%FrameNumber;
		}
		EliminatePage[l]=PageFrames[j];
		PageFrames[j]=next;
		ReferenceBit[j]=1;
		FaultNumber++;
		j=(j+1)%FrameNumber;
		for(ii=0; ii<FrameNumber; ii++){
			i=(j+ii)%FrameNumber;
			if(PageFrames[i]>=0) 
				cout << PageFrames[i] << " ";
		}
		if(EliminatePage[l]>=0)
			cout << "->" << EliminatePage[l++] << endl;
		else
			cout << endl;  
	}  
	Report();              
}
void Replace::Eclock (void)
{
	int j,i,ii,k,l,next;
	InitSpace("EClock");
	for(k=0,j=l=0; k < PageNumber; k++){
		next=ReferencePage[k];
		for(ii=0; ii<FrameNumber; ii++){
			i=(j+ii)%FrameNumber;
			if(next==PageFrames[i]){
				ReferenceBit[i]=1;
				count[i]++;
				//由于缺乏读写情况的数据，在此定义对于每一个帧一次是读一次是写，交替进行
				if(count[i]%2==0)
					ModifyBit[i]=0;
				else
					ModifyBit[i]=1;
				break;
			}
		}
		if (ii<FrameNumber){
			for(ii=0; ii<FrameNumber; ii++){
				i=(j+ii)%FrameNumber;
				cout << PageFrames[i] << " ";
			}
			cout << endl;
			continue;
		}
		int min=10*ReferenceBit[j]+ModifyBit[j];
		int index=j;
		for(ii=0; ii<FrameNumber; ii++){
			i=(j+ii)%FrameNumber;
			if(10*ReferenceBit[i]+ModifyBit[i]<min){
				min=10*ReferenceBit[i]+ModifyBit[i];
				index=i;           
			}
			ReferenceBit[i]=ModifyBit[i]=0;
		}          
		EliminatePage[l]=PageFrames[index];
		PageFrames[index]=next;
		ReferenceBit[index]=0;
		ModifyBit[index]=1;
		count[index]=1;
		FaultNumber++;
		j=index;
		for(ii=0; ii<FrameNumber; ii++){
			i=(j+ii)%FrameNumber;
			if(PageFrames[i]>=0) cout << PageFrames[i] << " ";
		}
		if(EliminatePage[l]>=0)
			cout << "->" << EliminatePage[l++] << endl;
		else
			cout << endl;  
	}  
	Report();                      
}
void Replace::Lfu(void)
{
	int j,i,ii,k,l,next;
	InitSpace("Lfu");
	for(k=0,j=l=0; k < PageNumber; k++){
		next=ReferencePage[k];
		for(ii=0; ii<FrameNumber; ii++){
			i=(j+ii)%FrameNumber;
			if(next==PageFrames[i]){
				count[i]++;
				break;
			}
		}
		if (ii<FrameNumber){
			for(ii=0; ii<FrameNumber; ii++){
				i=(j+ii)%FrameNumber;
				cout << PageFrames[i] << " ";
			}
			cout << endl;
			continue;
		}
		FaultNumber++;
		int min=count[0];
		int index=0;
		for(i=0;i<FrameNumber;i++){
			if(count[i]<min){
				min=count[i];
				index=i;           
			}
		}
		EliminatePage[l]= PageFrames[index];
		PageFrames[index]=next;
		count[index]=1;
		for(ii=0; ii<FrameNumber; ii++){
			i=(j+ii)%FrameNumber;
			if(PageFrames[i]>=0)
			   cout << PageFrames[i] << " ";
		}
		if(EliminatePage[l]>=0)
			cout << "->" << EliminatePage[l++] << endl;
		else
			cout << endl;
	}
	Report();      
}
void Replace::Mfu(void)
{
	int j,i,ii,k,l,next;
	InitSpace("Mfu");
	for(i=0;i<FrameNumber;i++)
		count[i]=10000;
	for(k=0,j=l=0; k < PageNumber; k++){
		next=ReferencePage[k];
		for(ii=0; ii<FrameNumber; ii++){
			i=(j+ii)%FrameNumber;
			if(next==PageFrames[i]){
				count[i]++;
				break;
			}
		}
		if (ii<FrameNumber){
			for(ii=0; ii<FrameNumber; ii++){
				i=(j+ii)%FrameNumber;
				cout << PageFrames[i] << " ";
			}
			cout << endl;
			continue;
		}
		FaultNumber++;
		int max=count[0];
		int index=0;
		for(i=0;i<FrameNumber;i++){
			if(count[i]>max){
				max=count[i];
				index=i;           
			}
		}
		EliminatePage[l]= PageFrames[index];
		PageFrames[index]=next;
		count[index]=1;
		for(ii=0; ii<FrameNumber; ii++){
			i=(j+ii)%FrameNumber;
			if(PageFrames[i]>=0) cout << PageFrames[i] << " ";
		}
		if(EliminatePage[l]>=0)
			cout << "->" << EliminatePage[l++] << endl;
		else
			cout << endl;
	}
	Report();
}
int main(int argc,char *argv[]){
	Replace * vmpr = new Replace();
	vmpr->Lru();
	vmpr->Fifo();
	vmpr->Clock();
	vmpr->Eclock();
	vmpr->Lfu();
	vmpr->Mfu();
	return 0;
}

