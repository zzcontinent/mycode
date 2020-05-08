#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int * data;   //原始数据；
int * hash;   //哈希表，初始化为0；
int N = 0;
int LEN = 0;
int EMPTY_NUM = -9999;

void GenData(int len)
{
	N = len;
	LEN = len + 10;

	int* p = malloc(sizeof(int)*len);	
	for(int i=0;i<len;i++)
		p[i] = EMPTY_NUM;
	srand((int)time(0));
	for(int i=0;i<len-5;i++)
	{
		p[i] = (int)(65535.0*rand()/(RAND_MAX));
	}
	data = (int*)p;
	hash = (int*)malloc(sizeof(int)*LEN);
}

void Create()
{
	for(int i=0;i<N;i++)  //循环将原始数据保存到哈希表中；
	{
		//将关键字插入到哈希表hash中；
		int j=data[i]%LEN;  //计算哈希地址；
		while(hash[j])  //元素位置已被占用；
			j=(++j)%LEN;  //线性探测法解决冲突；
		hash[j]=data[i];
	}
}


int Haxi_Sou(int key)
{
	int i=key%LEN;  //计算哈希地址；
	if(hash[i] == EMPTY_NUM)  //查找到开放单元，表示查找失败；
		return -1;  //返回失败值；
	int j = i;
	while(hash[i] != key)
	{
		i = (i+1)%LEN;
		if(j == i)
			return -1;	
	}
	return i;   //返回对应的元素下标；
}


int main(void)
{
	int key;
	GenData(100);
	Create();  //调用函数创建哈希表；
	printf("哈希表中各元素的值:");
	for(int i=0;i<LEN;i++)
		printf("%d ",hash[i]);
	printf("\n");


	while(1)
	{
		printf("输入查找的关键字:");
		scanf("%d",&key);

		int pos=Haxi_Sou(key);  //调用函数在哈希表中查找；
		if(pos>=0)
			printf("查找成功，该关键字在数组中的下标为 %d !!!",pos);
		else
			printf("查找失败!!!");
		printf("\n");
	}
	return 0;
}
