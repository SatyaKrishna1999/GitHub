#include<stdio.h>
#include<conio.h>
int n;
int readyQueue[10],process[10][6],arrivalTime[10],burst[10],execution[150];
int front=0,rear=-1,max=-1,flag=0,from,cpuTime=0,ch=0,c=0;
int status[10];

void Enqueue(int a){
	// Enqueue the id of the process in the circular queue........
	
	rear=(rear+1)%n;
	readyQueue[rear]=a;
	status[a-1]=1;
}

int Dequeue(){
	// Dequeue the process completed its execution......
	
	int a=readyQueue[front];
	status[a-1]=-1;
	readyQueue[front]=-1;
	front=(front+1)%n;
	from=a;
	flag=1;
	
	// Adding the completion time at process[id][4].......
	process[a-1][3]=cpuTime+1;
	return a;
}

int sortReadyQueue(){
	// Sorting all the processes including the present running process.......
	int array[n];
	if(flag==0)
	from=readyQueue[front];
	int temp[n],count=0,i,j,count1=0,temp1[6];
	for(i=0;i<n;i++){
		if(readyQueue[i]>0 && status[readyQueue[i]-1]==1){
			temp[count]=readyQueue[i];
			array[count++]=i;
		}
		else if(status[readyQueue[i]-1]==2){
			temp1[count1++]=readyQueue[i];
		}
	}
	// Sorting the process as per priority using bubble sort.......
	
	for(i=count;i>0;i--){
		for(j=0;j<i-1;j++){
			if(process[temp[j+1]-1][2]<process[temp[j]-1][2]){
				int k=temp[j+1];
				temp[j+1]=temp[j];
				temp[j]=k;
			}
		}
	}
	j=1;
	int j1=0;
	if(count!=0){
		i=(front+1)%6;
		c++;
	readyQueue[front]=temp[0];
	for(;i!=(rear+1)%n;){
		if(j<count){
			readyQueue[i]=temp[j++];
			i=(i+1)%n;
			continue;
		}
		if(j==count)
		break;
		
		else{
			readyQueue[i]=temp1[j1++];
		}
		i=(i+1)%6;
		}
	}
	else if(IsAllExecuted()){
		for(i=0;i<n;i++){
			if(process[i][2]>0){
				status[i]=1;
			}
		}
	}
	else
	return -1;
	if(from!=readyQueue[front] || c==1){
	printf("|  P%d  ",readyQueue[front]);
	
	execution[ch++]=cpuTime;
	}
	flag=0;
	return readyQueue[front]-1;
}

int switching(){
	// Sorting the process as per priority excluding the present running process........
	status[readyQueue[front]-1]=2;
	int a= readyQueue[front];
	readyQueue[front]=-1;
	front=(front+1)%n;
	rear=(rear+1)%n;
	readyQueue[rear]=a;
	return readyQueue[front]-1;
}
int IsAllExecuted(){
	// checking whether all the process has completed their execution or not......
	
	int i;
	for(i=0;i<n;i++){
		if(process[i][2]>0)
			break;
	}
	
	if(i<n)
	return 1;
	return 0;
}

void scheduler(){
	int id,i,count=0,flag1=0;
	while(IsAllExecuted()){
		if(cpuTime<3){
			// CPU is Idle....
			printf("|      ");
			execution[ch++]=cpuTime;
			cpuTime++;
			continue;
		}
		if(cpuTime==3){
			for(i=0;i<n;i++){
				if(cpuTime>arrivalTime[i]){
					Enqueue(i+1);
				}
			}
		}
		if(cpuTime<=max)
		for(i=0;i<n;i++)
			if(cpuTime==arrivalTime[i])
				Enqueue(i+1);
		if(flag1!=1)
		id=sortReadyQueue();
		flag1=0;
		if(id!=-1){
			process[id][2]=process[id][2]-1;
			status[id]=1;
			count++;
			if(process[id][2]<=0){
				Dequeue();
				count=0;
			}
		}
		else if(IsAllExecuted()){
			printf("|      ");
			execution[ch++]=cpuTime;	
		}
		else{
			break;	
		}
		cpuTime++;
		if(count==10){int a;	
			id=switching();
			if(id!=-1){
			printf("|  P%d  ",id+1);
			execution[ch++]=cpuTime;
			}
			flag1=1;
			count=0;
		}
	}
}



void calculateWaitingTime(){
	int i,avg=0;
	printf("\n=======================================================================================================================\n\n");
	printf("Process		WaitingTime\n");
	printf("--------------------------------\n");
	for(i=0;i<n;i++){
		process[i][5]=process[i][4]-process[i][0];
		avg+=process[i][5];
		printf("  P%d			%d\n",i+1,process[i][5]);
	}
	avg/=n;
	printf("\nThe Average Waiting Time is %d\n",avg);
}


void calculateTurnAroundTime(){
	int i,avg=0;
	printf("\n=======================================================================================================================\n\n");
	printf("Process		TurnAroundTime\n");
	printf("------------------------------------\n");
	for(i=0;i<n;i++){
		process[i][4]=process[i][3]-process[i][1];
		avg+=process[i][4];
		printf("  P%d			%d\n",i+1,process[i][4]);
	}
	avg/=n;
	printf("\nThe Average TurnAround Time is %d\n",avg);
	
}
int main(){
	int i;
	printf("Enter no of processes :: ");
	scanf("%d",&n);
	for(i=0;i<n;i++){
		printf("Enter P%d arrival time :: ",i+1);
		scanf("%d",&arrivalTime[i]);
		process[i][1]=arrivalTime[i];
		if(max<arrivalTime[i])
			max=arrivalTime[i];
		printf("Enter P%d burst time :: ",i+1);
		scanf("%d",&burst[i]);
		process[i][2]=burst[i];
		process[i][0]=burst[i];
	}
	printf("=======================================================================================================================\n\n");
	scheduler();
	execution[ch++]=cpuTime;
	printf("|\n");
	for(i=0;i<ch;i++){
		if(execution[i]<9)
			printf("%d      ",execution[i]);
		else
			printf("%d     ",execution[i]);
	}
	calculateTurnAroundTime();
	calculateWaitingTime();
	
	
	return 0;
}
