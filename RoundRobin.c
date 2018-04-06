#include<stdio.h>
#include<conio.h>
int n;
int process[10][7];
float priority[10];
int readyQueue[10],burst[10],arrivalTime[10],execution[100];
int front=0,rear=-1,max=-1,minPriorityProcess;
int timeQuantum,cpuTime=0,ch=0;

void Enqueue(int a){
	// Enqueue the id of the process in the circular queue........
	
	rear=(rear+1)%n;
	readyQueue[rear]=a;
}

int sortAsPerPriority(){
	// Sorting the process as per priority excluding the present running process........
	
	int temp[n],count=0,i,j;
	for(i=0;i<n;i++){
		if(readyQueue[i]>0 && i!=front)
		// Excluding the process which is running right now.......
		temp[count++]=readyQueue[i];
	}
	// Sorting the process as per priority using bubble sort.......
	
	for(i=count;i>0;i--){
		for(j=0;j<i-1;j++){
			if(priority[temp[j+1]-1]>priority[temp[j]-1]){
				int k=temp[j+1];
				temp[j+1]=temp[j];
				temp[j]=k;
			}
		}
	}
	j=0;
	i=(front+1)%n;
	if(count!=0)
	for(;i!=(rear+1)%n;){
		readyQueue[i]=temp[j++];
		i=(i+1)%n;
	}
	else
	return -1;
	return temp[0]-1;
}

int sort(){
	// Sorting all the processes including the present running process.......
	
	int temp[n],count=0,i,j;
	for(i=0;i<n;i++){
		if(readyQueue[i]>0 ){
			temp[count++]=readyQueue[i];
		}
		i=(i+1)%6;
	}
	// Sorting the process as per priority using bubble sort.......
	
	for(i=count;i>0;i--){
		for(j=0;j<i-1;j++){
			if(priority[temp[j+1]-1]>priority[temp[j]-1]){
				int k=temp[j+1];
				temp[j+1]=temp[j];
				temp[j]=k;
			}
		}
	}
	j=1;
	i=(front+1)%n;
	if(count!=0){
		readyQueue[front]=temp[0];
		for(;i!=(rear+1)%n;){
			readyQueue[i]=temp[j++];
			i=(i+1)%n;
		}
	}
	else
		return-1;
	return readyQueue[front]-1;
}

int Dequeue(){
	// Dequeue the process completed its execution......
	
	int a=readyQueue[front];
	sortAsPerPriority(0);
	readyQueue[front]=-1;
	front=(front+1)%n;
	
	// Adding the completion time at process[id][4].......
	process[a-1][4]=cpuTime;
	return readyQueue[front];
}

int IsAllExecuted(){
	// checking whether all the process has completed their execution or not......
	
	int i;
	for(i=0;i<n;i++){
		if(readyQueue[i]!=-1)
			break;
	}
	
	if(i<n)
	return 1;
	else
	return 0;
}

int switchProcess(){
	//Switching process....
	
	int b,a=readyQueue[front];
	int id,i;
	id=sortAsPerPriority();
			readyQueue[front]=-1;
			front=(front+1)%n;
			rear=(rear+1)%n;
			readyQueue[rear]=a;
			if(readyQueue[front]!=-1){
				printf("|  P%d  ",readyQueue[front]);
				execution[ch++]=cpuTime;
			}
			else
			return -1;
			return readyQueue[front]-1;
}

void priorityAllocator(){
	// Calculating priority for each process based on burst time.....
	
	int i,j;
	float least=100;
	for(i=0;i<n;i++)
	{
		priority[i]=50/burst[i];
		if(least>priority[i]){
			least=priority[i];
			minPriorityProcess=i;
		}
	}
	for(i=0;i<n;i++){
		//process[i][0]=i+1;
		priority[i]-=least;
		//process[i][1]=priority[i];
		process[i][2]=burst[i];
		process[i][3]=arrivalTime[i];
	}
	// minimum priority process represents the process with priority 0.
	//printf("\n\nThe Process has the minimum Priority is %d\n\n",minPriorityProcess+1);
}

void roundRobin(int starvingLimit){
	int count=0,id,agingCount=0,pid,check;
	int flag=0,cpuIdle=1;
	int i,k=0;
	check=IsAllExecuted();
	while(check>=1){
		if(cpuTime<=max){
			for(i=0;i<n;i++){
				if(process[i][3]==cpuTime){
					Enqueue(i+1);
					k++;
					if(cpuIdle==1)
					flag=1;
				}
			}
			if(flag==1){
				id=sort();
				printf("|  P%d  ",id+1);
				execution[ch++]=cpuTime;
				cpuIdle=0;
			}
		}
		if(k>=1){
			k=-100;
		}
		else if(k==0){
			// if k=0, it shows that CPU is idle........
			printf("|      ");
			execution[ch++]=cpuTime;
			cpuTime++;
			continue;
		}
		if(count==timeQuantum){
			// Switching process for every timeQuantum seconds......
			id=switchProcess();
			if(id!=-1){
				cpuIdle=0;
			}
			else if(IsAllExecuted()){
				// i.e., a new process will and the cpu is waiting for that.....
				
				printf("|      ");
				execution[ch++]=cpuTime;
				k=0;
				cpuIdle=1;
			}
			else{
				execution[ch++]=cpuTime;
				break;
			}
			count=0;
			
		}
		cpuTime++;
		if(cpuIdle!=1){
			process[id][2]=process[id][2]-1;
			count++;
			agingCount++;
		if(process[id][2]<=0){
			int a=Dequeue();
			if(a!=-1 && a!=0){
				printf("|  P%d  ",readyQueue[front]);
				execution[ch++]=cpuTime;
			}
			else if(IsAllExecuted()){
				cpuIdle=1;
				k=0;
			}
			else{
				execution[ch++]=cpuTime;
				break;
			}
			id=readyQueue[front]-1;
			count=0;
		}
		else
		cpuIdle=0;
	}
		if(agingCount==starvingLimit){
			// for every starvingLimit time units the priority of minPriorityProcess will get incremented.........
			
			agingCount=0;
			priority[minPriorityProcess]+=1;
			if(count==timeQuantum){
				id=switchProcess();
				count=0;
			}
		}
		check=IsAllExecuted();
		flag=0;
	}
}

void calculateWaitingTime(){
	int i,avg=0;
	printf("\n=======================================================================================================================\n\n");
	printf("Process		WaitingTime\n");
	printf("--------------------------------\n");
	for(i=0;i<n;i++){
		process[i][6]=process[i][5]-process[i][0];
		avg+=process[i][6];
		printf("  P%d			%d\n",i+1,process[i][6]);
	}
	avg/=n;
	printf("\nThe Average Waiting Time is %d\n",avg);
}


void calculateTurnAroundTime(){
	int i,avg=0;
	printf("=======================================================================================================================\n\n");
	printf("Process		TurnAroundTime\n");
	printf("------------------------------------\n");
	// process[id][5] represents turnAroundTime.......
	
	for(i=0;i<n;i++){
		process[i][5]=process[i][4]-process[i][3];
		avg+=process[i][5];
		printf("  P%d			%d\n",i+1,process[i][5]);
	}
	avg/=n;
	printf("\nThe Average TurnAround Time is %d\n",avg);
	
}


int main(){
	int i,avg=0;
	printf("Enter no of processes :: ");
	scanf("%d",&n);
	printf("Enter timeQuantum :: ");
	scanf("%d",&timeQuantum);
	for(i=0;i<n;i++){
		printf("Enter ArrivalTime of P%d :: ",i+1);
		scanf("%d",&arrivalTime[i]);
		if(max<arrivalTime[i])
			max=arrivalTime[i];
		printf("Enter BurstTime of P%d :: ",i+1);
		scanf("%d",&burst[i]);
		if(burst[i]==0){
			printf("Enter value > 0 \n");
			i--;
			continue;
		}
		// max defines the maximum value of arrivalTime.........
		process[i][0]=burst[i];
		avg+=burst[i];
	}
	avg/=4;
	priorityAllocator();
	roundRobin(avg);
	printf("|\n");
	for(i=0;i<ch;i++){
		if(execution[i]<9)
		printf("%d      ",execution[i]);
		else
			printf("%d     ",execution[i]);
	}
	printf("\n");
	calculateTurnAroundTime();
	calculateWaitingTime();
	return 0;
}
