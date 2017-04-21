#include<ctime>
#include<cstdio>
#include<cstdlib>
int main(){
	int n=1000000;
	printf("%d\n",n);
	srand(time(NULL));
	for(int i=0;i<n;i++){
		printf("%d ",rand()%5+1);
	}puts("");
	printf("%d\n",3);
	return 0;
}
