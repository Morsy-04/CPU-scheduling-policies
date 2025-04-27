#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX_QUEUE_SIZE 100
typedef struct {
 char name;
 int arrival_time;
 int st_p;
 int remaining_time;
 int index;
 int waiting_time;
 int initial_priority;
 

}process;
typedef struct {
    int number;
    int q;
}algorithm;
int endtime,num_of_processes;

//queue implemetation 
typedef struct {
    process processes[MAX_QUEUE_SIZE];
    int head;
    int tail;
    int size;
    int quantum;
}processQueue;
typedef struct {
    int items[MAX_QUEUE_SIZE];
    int head;
    int tail;
    int size;
} Queue;

void init_pqueue(processQueue* q,int quantum){
q->head=0;
q->tail=0;
q->size=0;
q->quantum=quantum;
}
void init_queue(Queue* q) {
    q->head=0;
    q->tail=0;
    q->size=0;
}
int is_pqempty(processQueue* q) {
    return q->size==0;
}

int is_empty(Queue* q) {
    return q->size==0;
}
int is_pqfull(processQueue* q) {
    return q->size==MAX_QUEUE_SIZE;
}

int is_full(Queue* q) {
    return q->size==MAX_QUEUE_SIZE;
}

void penqueue(processQueue* q, process p) {
    if (is_pqfull(q)) {
        printf("Queue is full, cannot enqueue.\n");
        return;
    }
    q->processes[q->tail]=p;
    q->tail=(q->tail+1)%MAX_QUEUE_SIZE;
    q->size++;
}
void enqueue(Queue* q, int item) {
    if (is_full(q)) {
        printf("Queue is full, cannot enqueue.\n");
        return;
    }
    q->items[q->tail] = item;
    q->tail = (q->tail + 1) % MAX_QUEUE_SIZE;
    q->size++;
}
process pdequeue(processQueue* q) {
    process temp;
    temp.st_p=-1;
    if (is_pqempty(q)) {
        printf("Queue is empty, cannot dequeue.\n");
        return temp;  
    }
    process p = q->processes[q->head];
    q->head=(q->head+1)%MAX_QUEUE_SIZE;
    q->size--;
    return p;
}

int dequeue(Queue* q) {
    if (is_empty(q)) {
        printf("Queue is empty, cannot dequeue.\n");
        return -1; 
    }
    int item = q->items[q->head];
    q->head = (q->head + 1) % MAX_QUEUE_SIZE;
    q->size--;
    return item;
}
int pis_in_queue(processQueue* q, process p) {
    int i=q->head; 
    while (i!=q->tail) { 
        if (q->processes[i].name==p.name && q->processes[i].arrival_time==p.arrival_time && q->processes[i].st_p==p.st_p) {
            return 1; 
        }
        i=(i+1)%MAX_QUEUE_SIZE;  
    }
    return 0; 
}

int is_in_queue(Queue* q, int process_id) {
    int i=q->head;
    while (i != q->tail) {
        if (q->items[i]==process_id) {
            return 1; 
        }
        i=(i+1)%MAX_QUEUE_SIZE;
    }
    return 0;  
}
void sort_RT_queue(Queue *queue,int remaining_time[]){//function to sort by remaining time
    int n=queue->size;
    int ids[n];
    int index=0;
    for(int i=0;i<num_of_processes;i++){
        if(is_in_queue(queue,i)){
            ids[index]=i;
            index++;
        }
    }
    for (int i=0;i<n-1;i++) {
        for (int j=i+1;j<n;j++) {
            if (remaining_time[ids[i]]>remaining_time[ids[j]]) {
                
                int temp = ids[i];
                ids[i] = ids[j];
                ids[j] = temp;
            }
        }
    }
    Queue temp_q;
    init_queue(&temp_q);
    for(int i=0;i<n;i++){
        enqueue(&temp_q,ids[i]);
    }
    *queue=temp_q;

}
void sort_pqueue_by_priority(processQueue *q) {//function to sort by priority
    int n=q->size;
    process sorted[MAX_QUEUE_SIZE];
    int index=0,i=q->head;
    while (index<n){
        sorted[index++]=q->processes[i];
        i=(i+1)%MAX_QUEUE_SIZE;
    }

    for (int i=0;i<n-1;i++) {
        for (int j=i+1;j<n;j++) {
            if (sorted[i].st_p<sorted[j].st_p) {
                process temp=sorted[i];
                sorted[i]=sorted[j];
                sorted[j]=temp;
            }
            if(sorted[i].st_p==sorted[j].st_p){
                if(sorted[i].waiting_time<sorted[j].waiting_time){
                process temp=sorted[i];
                sorted[i]=sorted[j];
                sorted[j]=temp;
                }

            }
        }
    }

    i=q->head;
    index=0;
    while(index<n){
        q->processes[i]=sorted[index++];
        i=(i+1)%MAX_QUEUE_SIZE;
    }
}



void print_centered_number(int value, int width) {
    char buffer[10];
    snprintf(buffer, sizeof(buffer), "%d", value);
    int len = strlen(buffer);
    int padding = (width - len) / 2;
    int extra_padding = (width - len) % 2; //Handle odd widths
    printf("%*s%s%*s", padding, "", buffer, padding + extra_padding, "");
}

void print_right_aligned_float(float value, int width) {
    char buffer[10];
    snprintf(buffer, sizeof(buffer), "%.2f", value);
    int len = strlen(buffer);

    // Adjust the column width to account for floating point precision
    if (len >= width)  //If the value is larger than the column width print it without modification
        printf("%-*s", width, buffer);
    else //right align the value in the given width
        printf("%*s", width, buffer); 
    
}

void print_stats(process processes[], int num_of_processes, int finish_times[], int turn_around[], float norm_turn[], float avg_turnaround, float avg_norm_turn, int alg, int q) {
    const int column_width = 5; 

    //Print the algorithm 
    if (alg == 1) {
        printf("FCFS\n");
    }
    if (alg == 2) {
        printf("RR-%d\n", q);
    }
    if (alg==3) {
        printf("SPN\n");
    }
    if(alg==4){
        printf("SRT\n");
    }
    if(alg==5){
        printf("HRRN\n");
    }
    if(alg==6)
    printf("FB-1\n");
    if(alg==7)
    printf("FB-2i\n");
    
    printf("Process    |");
    for (int i=0;i<num_of_processes;i++) {
        printf("  %c  |",processes[i].name); 
    }
    printf("\n");
    printf("Arrival    |");
    for (int i=0;i<num_of_processes;i++) {
        print_centered_number(processes[i].arrival_time, column_width);
        printf("|");
    }
    printf("\n");

    printf("Service    |");
    for (int i=0;i<num_of_processes;i++) {
        print_centered_number(processes[i].st_p,column_width);
        printf("|");
    }
    printf(" Mean|\n"); 

    
    printf("Finish     |");
    for (int i=0;i<num_of_processes;i++) {
        print_centered_number(finish_times[i],column_width);
        printf("|");
    }
    printf("-----|\n");

    
    printf("Turnaround |");
    for (int i=0;i<num_of_processes;i++) {
        print_centered_number(turn_around[i],column_width);
        printf("|");
    }
    print_right_aligned_float(avg_turnaround,column_width); 
    printf("|\n");

   
    printf("NormTurn   |");
    for (int i=0;i<num_of_processes;i++) {
        print_right_aligned_float(norm_turn[i],column_width);
        printf("|");
    }
    print_right_aligned_float(avg_norm_turn,column_width);

    printf("|\n");
    printf("\n");
}


void print_trace(process processes[],char time_line[num_of_processes][endtime],int alg,int q){
    if(alg==1)
    printf("FCFS  ");
    if(alg==2)
    printf("RR-%d  ",q);
    if(alg==3)
    printf("SPN   ");
    if(alg==4)
    printf("SRT   "); 
    if(alg==5)
    printf("HRRN  ");  
    if(alg==6)
    printf("FB-1  ");
    if(alg==7)
    printf("FB-2i ");
    if(alg==8)
    printf("Aging ");
    for(int i=0;i<endtime+1;i++)
    printf("%d ",i%10);
    printf("\n");
    printf("------------------------------------------------\n");
    for(int i=0;i<num_of_processes;i++){
        printf("%c     |",processes[i].name);
        for(int j=0;j<endtime;j++){
            printf("%c|",time_line[i][j]);
        }
        printf(" \n");
    }
    printf("------------------------------------------------\n");
    printf("\n");

}

void FCFS(process processes[],char* type){
    int finish_times[num_of_processes];
    int turn_around[num_of_processes];
    float NormTurn[num_of_processes];
    int total_turnaround=0;
    float total_NormTurn=0.0;
    float avg_turnaround=0.0;
    float avg_NormTurn=0.0;
    char time_line[num_of_processes][endtime];


    for (int i=0;i<num_of_processes;i++) {
        for (int t=0;t<endtime;t++) {
            time_line[i][t] = ' ';
        }
    }


    //printf("%d",endtime);
    //compute stats for first process
    finish_times[0]=processes[0].st_p+processes[0].arrival_time;
    turn_around[0]=finish_times[0]-processes[0].arrival_time;
    NormTurn[0]=(float)turn_around[0]/processes[0].st_p;
    total_turnaround=turn_around[0];
    total_NormTurn=NormTurn[0];

    for(int t=processes[0].arrival_time;t<finish_times[0];t++){
        time_line[0][t]='*';

    }

    //for loop to compute stats for the rest of processes
    for(int i=1;i<num_of_processes;i++){
        finish_times[i]=processes[i].st_p+finish_times[i-1];
        turn_around[i]=finish_times[i]-processes[i].arrival_time;
        NormTurn[i]=(float)turn_around[i]/processes[i].st_p;
        total_turnaround+=turn_around[i];
        total_NormTurn+=NormTurn[i];

        int start=finish_times[i-1];
        for(int t=processes[i].arrival_time;t<start;t++){
            time_line[i][t]='.';
        }
        for(int t=start;t<finish_times[i];t++)
            time_line[i][t]='*';


    }
    avg_turnaround=(float)total_turnaround/num_of_processes;
    avg_NormTurn=total_NormTurn/num_of_processes;

 /*
    printf("Average Turnaround Time: %.2f\n", avg_turnaround);
    printf("Average Normalized Turnaround Time: %.2f\n", avg_NormTurn);
    
   printf("Timeline:\n");
    for (int i = 0; i < num_of_processes; i++) {
        printf("%c: ", processes[i].name);
        for (int t = 0; t < endtime; t++) {
            printf("%c", time_line[i][t]);
        }
        printf("\n");
    }
    */
   if(!(strcmp(type,"stats")))
    print_stats(processes,num_of_processes,finish_times,turn_around,NormTurn,avg_turnaround,avg_NormTurn,1,1);
   else
    print_trace(processes,time_line,1,1);
    
}

void RoundRobin(process processes[], char *type, int q) {
    int finish_times[num_of_processes];
    int turn_around[num_of_processes];
    float NormTurn[num_of_processes];
    int total_turnaround=0;
    float total_NormTurn=0.0;
    float avg_turnaround=0.0;
    float avg_NormTurn=0.0;
    int remaining_time[num_of_processes];
    char time_line[num_of_processes][endtime];
    for (int i=0;i<num_of_processes;i++) {
        for (int t=0;t<endtime;t++) {
            time_line[i][t]=' ';
        }
    }

    
    for (int i=0;i<num_of_processes;i++) {
        remaining_time[i]=processes[i].st_p; // Set initial service time
    }

    int current_time=0;
    Queue queue;
    init_queue(&queue);

  
    enqueue(&queue,0);//enqueue first process
    while(!is_empty(&queue)){
        int runtime;
        int id =dequeue(&queue); //pop first process
        if (remaining_time[id]<q) //compute the runtime for the process
            runtime=remaining_time[id];
        else
            runtime=q;
        int start_time=current_time;
        for(int i=0;i<num_of_processes;i++){//check for arrival of a new process
            if(processes[i].arrival_time>start_time && processes[i].arrival_time<=current_time+runtime && !is_in_queue(&queue,i)){
                enqueue(&queue,i);
            }
        }
        
        
        
        current_time+=runtime;//update current
        for (int t=start_time;t<current_time;t++) {
            time_line[id][t]='*';
        }
        remaining_time[id]-=runtime; //update remaining
        if (remaining_time[id]>0) {//if not finished add back to the queue
            enqueue(&queue, id);
        } else {//compute stats
            finish_times[id]=current_time;
            turn_around[id]=finish_times[id]-processes[id].arrival_time;
            NormTurn[id]=(float)turn_around[id]/processes[id].st_p;
            total_turnaround+=turn_around[id];
            total_NormTurn+=NormTurn[id];
        }
        for(int i=0;i<num_of_processes;i++){
            if(i!=id && is_in_queue(&queue,i)){
                for(int t=processes[i].arrival_time;t<current_time;t++){
                    if(time_line[i][t]==' ')
                    time_line[i][t]='.';
                }
                
            }
        }

    }


    

    avg_turnaround=(float)total_turnaround/num_of_processes;
    avg_NormTurn=total_NormTurn/num_of_processes;

    
    if (!(strcmp(type, "stats"))) {
        print_stats(processes, num_of_processes, finish_times, turn_around, NormTurn, avg_turnaround, avg_NormTurn, 2, q);
    } else {
        print_trace(processes, time_line, 2, q);
    }
}
void SPN(process processes[],char *type){
    int finish_times[num_of_processes];
    int turn_around[num_of_processes];
    float NormTurn[num_of_processes];
    int total_turnaround=0;
    float total_NormTurn=0.0;
    float avg_turnaround=0.0;
    float avg_NormTurn=0.0;
    char time_line[num_of_processes][endtime];
    for (int i=0;i<num_of_processes;i++) {
        for (int t=0;t<endtime;t++) {
            time_line[i][t]=' ';
        }
    }
    for (int i=0;i<num_of_processes-1;i++) { //sort by arrival times 
        for (int j=i+1;j<num_of_processes;j++) {
            if (processes[i].arrival_time>processes[j].arrival_time) {
                process temp=processes[i];
                processes[i]=processes[j];
                processes[j]=temp;
            }
        }
    }
    int current_time=0;
    int completed=0;//indicate how many completed processes
    int is_completed[num_of_processes];
    for (int i=0;i<num_of_processes;i++) { //init complete array
        is_completed[i]=0; 
    }
    while (completed<num_of_processes) {//until all processes are finished
        int min_remaining=1000;
        int next_process=-1;

        
        for (int i=0;i<num_of_processes;i++) {//get the process with the shortest service time
            if (processes[i].arrival_time<=current_time && !is_completed[i]) {
                if (processes[i].st_p<min_remaining) {
                    min_remaining=processes[i].st_p;
                    next_process=i;
                }
            }
        }
        if (next_process==-1) {
            current_time++;
            continue;
        }
        //compute stats
        int start_time=current_time;
        int finish_time=start_time+processes[next_process].st_p;
        current_time=finish_time;

        finish_times[next_process]=finish_time;
        turn_around[next_process]=finish_time-processes[next_process].arrival_time;
        NormTurn[next_process]=(float)turn_around[next_process]/processes[next_process].st_p;

        total_turnaround+=turn_around[next_process];
        total_NormTurn+=NormTurn[next_process];

        
        is_completed[next_process] = 1;//update completed array

        
        for (int t=start_time;t<finish_time;t++) {
            time_line[next_process][t]='*';
        }

        completed++;
    }
    avg_turnaround=(float)total_turnaround/num_of_processes;
    avg_NormTurn=(float)total_NormTurn/num_of_processes;
    for(int i=0;i<num_of_processes;i++){
        for(int t=processes[i].arrival_time;t<finish_times[i];t++){
            if(time_line[i][t]==' ')
                time_line[i][t]='.';
        }
    }
    if (!(strcmp(type, "stats"))) {
        print_stats(processes, num_of_processes, finish_times, turn_around, NormTurn, avg_turnaround, avg_NormTurn, 3, 1);
    } else {
        print_trace(processes, time_line, 3, 1);
    }



}
void SRT(process processes[],char *type){
    
    int finish_times[num_of_processes];
    int turn_around[num_of_processes];
    float NormTurn[num_of_processes];
    int total_turnaround=0;
    float total_NormTurn=0.0;
    float avg_turnaround=0.0;
    float avg_NormTurn=0.0;
    char time_line[num_of_processes][endtime];
    int remaining_time[num_of_processes];
    for(int i=0;i<num_of_processes;i++){ //init remaining times
        remaining_time[i]=processes[i].st_p;
        
    }
    //printf("\n");
    for (int i=0;i<num_of_processes;i++) {
        for (int t=0;t<endtime;t++) {
            time_line[i][t]=' ';
        }
    }
    
    Queue queue;
    init_queue(&queue);
    int current_time=0;
    
    //enqueue(&queue,0);
    do{
        
        for(int i=0;i<num_of_processes;i++){//check for newly arrived processes
            if((processes[i].arrival_time)==current_time && !is_in_queue(&queue,i) && remaining_time[i]>0){
                enqueue(&queue,i);
                
            }
        }
        sort_RT_queue(&queue,remaining_time);//sort by the remaining times
        int pid=dequeue(&queue);//pop first process
        
        int runtime=1;
        
        for (int t=current_time;t<current_time+1;t++) {
            time_line[pid][t]='*';
        }
        current_time=current_time+runtime;//update current time
        
        remaining_time[pid]-=runtime;//update remaining time 
        if(remaining_time[pid]>0){//if not finished add in the queue again
            enqueue(&queue,pid);
        }
        else{//if finished compute the stats
            finish_times[pid]=current_time;
            turn_around[pid]=finish_times[pid]-processes[pid].arrival_time;
            NormTurn[pid]=(float)turn_around[pid]/processes[pid].st_p;
            total_turnaround+=turn_around[pid];
            total_NormTurn+=NormTurn[pid];

        }


    }while (current_time<endtime);

    avg_turnaround=(float)total_turnaround/num_of_processes;
    avg_NormTurn=total_NormTurn/num_of_processes;
    for(int i=0;i<num_of_processes;i++){
        for(int t=processes[i].arrival_time;t<finish_times[i];t++){
            if(time_line[i][t]==' ')
                time_line[i][t]='.';
        }
    }
    if (!(strcmp(type, "stats"))) {
        print_stats(processes, num_of_processes, finish_times, turn_around, NormTurn, avg_turnaround, avg_NormTurn, 4, 1);
    } else {
        print_trace(processes, time_line, 4, 1);
    }
}
void HRRT(process processes[],char* type){
    int finish_times[num_of_processes];
    int turn_around[num_of_processes];
    float NormTurn[num_of_processes];
    int total_turnaround=0;
    float total_NormTurn=0.0;
    float avg_turnaround=0.0;
    float avg_NormTurn=0.0;
    char time_line[num_of_processes][endtime];
    int completed[num_of_processes];
    for (int i=0;i<num_of_processes;i++) { //init timeline and copleted processes array
        for (int t=0;t<endtime;t++) {
            time_line[i][t]=' ';
        }
        completed[i]=0;
    }
    
    int current_time=0;

    while(current_time<endtime){
        int pid=-1;
        float max_rr=-1;
        for(int i=0;i<num_of_processes;i++){ //compute response ratio for each process thet arrived and not completed and get the max
            if(completed[i]==0 && processes[i].arrival_time<=current_time)
            {   float waiting_time=current_time-processes[i].arrival_time;
                float response_ratio=(float)(waiting_time+processes[i].st_p)/processes[i].st_p;
                if(response_ratio>max_rr || (response_ratio==max_rr && processes[i].arrival_time<processes[pid].arrival_time)){
                    max_rr=response_ratio;
                    pid=i;
                }

            }

        }
        if(pid==-1){//if no process was chosen 
            current_time++;
            continue;
        }
        int start_time=current_time;
        current_time+=processes[pid].st_p;//no preemption so current time get incremented by the whole service time

        for (int t=start_time;t<current_time;t++) {
            time_line[pid][t]='*';
        }
        completed[pid] = 1; //update complete array and calculate stats
        finish_times[pid]=current_time;
        turn_around[pid]=finish_times[pid]-processes[pid].arrival_time;
        NormTurn[pid]=(float)turn_around[pid]/processes[pid].st_p;
        total_turnaround+=turn_around[pid];
        total_NormTurn+=NormTurn[pid];

    }
    avg_turnaround=(float)total_turnaround/num_of_processes;
    avg_NormTurn=total_NormTurn/num_of_processes;
    for(int i=0;i<num_of_processes;i++){
        for(int t=processes[i].arrival_time;t<finish_times[i];t++){
            if(time_line[i][t]==' ')
                time_line[i][t]='.';
        }
    }
    if (!(strcmp(type, "stats"))) {
        print_stats(processes, num_of_processes, finish_times, turn_around, NormTurn, avg_turnaround, avg_NormTurn, 5, 1);
    } else {
        print_trace(processes, time_line, 5, 1);
    }
}
void FB_2i(process processes[],char*type){ // same code for FB-1 Go to FB-1 for comments
    int finish_times[num_of_processes];
    int turn_around[num_of_processes];
    float NormTurn[num_of_processes];
    int total_turnaround=0;
    float total_NormTurn=0.0;
    float avg_turnaround=0.0;
    float avg_NormTurn=0.0;
    char time_line[num_of_processes][endtime];
    processQueue queues[10];
    int remaining_time[num_of_processes];
    for(int i=0;i<num_of_processes;i++){
        processes[i].remaining_time=processes[i].st_p;
        processes[i].index=i;
        
    }
    int process_in_queue[num_of_processes];
    for(int i=0;i<num_of_processes;i++){
        process_in_queue[i]=-1;
    }
    for(int i=0;i<10;i++){
        init_pqueue(&(queues[i]),1 << i);
    }
    for (int i=0;i<num_of_processes;i++) {
        for (int t=0;t<endtime;t++) {
            time_line[i][t]=' ';
        }
    }
    int current_time=0;
    for(int i=0;i<num_of_processes;i++){
            if(process_in_queue[i]==-1 && processes[i].arrival_time<=current_time)
            {   penqueue(&(queues[0]),processes[i]);
                process_in_queue[i]=0;
            }
            
            }
    while(current_time<endtime){
        
        process pid;
        int q;
        int runtime;
        int process_found=0;
        for(int i=0;i<10;i++){
            if(!is_pqempty(&(queues[i]))){
                pid=pdequeue(&(queues[i]));
                q=queues[i].quantum;
                process_found=1;
                break;
            }
        }
        if (process_found!=1) {
            current_time++; 
            continue;
        }
        if(pid.remaining_time<q)
            runtime=pid.remaining_time;
        else
            runtime=q;
        for (int t=current_time;t<current_time+runtime;t++) {
            time_line[pid.index][t]='*';
        }
        current_time=current_time+runtime;
        pid.remaining_time-=runtime;
        for(int i=0;i<num_of_processes;i++){
            if(process_in_queue[i]==-1 && processes[i].arrival_time<=current_time)
            {   penqueue(&(queues[0]),processes[i]);
                process_in_queue[i]=0;
            }
            
            }
        //processes[pid.index].remaining_time=pid.remaining_time;
        if(pid.remaining_time>0){
            if(pid.index==0 && is_pqempty(&(queues[0]))){
                process_in_queue[pid.index]=0;
            }
            else
            process_in_queue[pid.index]++;
            penqueue(&(queues[process_in_queue[pid.index]]),pid);
        }
        else{
            finish_times[pid.index]=current_time;
            turn_around[pid.index]=finish_times[pid.index]-processes[pid.index].arrival_time;
            NormTurn[pid.index]=(float)turn_around[pid.index]/processes[pid.index].st_p;
            total_turnaround+=turn_around[pid.index];
            total_NormTurn+=NormTurn[pid.index];


        }
        
    }
    avg_turnaround=(float)total_turnaround/num_of_processes;
    avg_NormTurn=total_NormTurn/num_of_processes;
    for(int i=0;i<num_of_processes;i++){
        for(int t=processes[i].arrival_time;t<finish_times[i];t++){
            if(time_line[i][t]==' ')
                time_line[i][t]='.';
        }
    }
    if (!(strcmp(type, "stats"))) {
        print_stats(processes, num_of_processes, finish_times, turn_around, NormTurn, avg_turnaround, avg_NormTurn, 7, 1);
    } else {
        print_trace(processes, time_line, 7, 1);
    }


}
void FB1(process processes[],char*type){
    int finish_times[num_of_processes];
    int turn_around[num_of_processes];
    float NormTurn[num_of_processes];
    int total_turnaround=0;
    float total_NormTurn=0.0;
    float avg_turnaround=0.0;
    float avg_NormTurn=0.0;
    char time_line[num_of_processes][endtime];
    processQueue queues[10]; //array of queues
    int remaining_time[num_of_processes];
    for(int i=0;i<num_of_processes;i++){
        processes[i].remaining_time=processes[i].st_p; //init remaining time
        processes[i].index=i;
        
    }
    int process_in_queue[num_of_processes]; //array to store which process is in which queue
    for(int i=0;i<num_of_processes;i++){
        process_in_queue[i]=-1; 
    }
    for(int i=0;i<10;i++){ //initialize queues
        init_pqueue(&(queues[i]),1); //init queues with its quantums
    }
    for (int i=0;i<num_of_processes;i++) {
        for (int t=0;t<endtime;t++) {
            time_line[i][t]=' ';
        }
    }
    int current_time=0;
    int q;
    for(int i=0;i<num_of_processes;i++){ //add arrived processes in first queue
            if(process_in_queue[i]==-1 && processes[i].arrival_time<=current_time)
            {   penqueue(&(queues[0]),processes[i]);
                q=1;
                process_in_queue[i]=0;
            }
            
            }
    while(current_time<endtime){
        
        process pid;
        
        int runtime;
        int process_found=0;
        for(int i=0;i<10;i++){ // pass through queues to get first process
            if(!is_pqempty(&(queues[i]))){
                pid=pdequeue(&(queues[i]));
                q=queues[i].quantum;
                process_found=1;
                break;
            }
        }
        if (process_found!=1) {
            current_time++; 
            continue;
        }
        if(pid.remaining_time<q) //compute runtime
            runtime=pid.remaining_time;
        else
            runtime=q;
        for (int t=current_time;t<current_time+runtime;t++) {
            time_line[pid.index][t]='*';
        }
        current_time=current_time+runtime; //update current
        pid.remaining_time-=runtime;
        for(int i=0;i<num_of_processes;i++){ //check for newly arrived processes
            if(process_in_queue[i]==-1 && processes[i].arrival_time<=current_time)
            {   penqueue(&(queues[0]),processes[i]);
                process_in_queue[i]=0;
            }
            
            }
        //processes[pid.index].remaining_time=pid.remaining_time;
        if(pid.remaining_time>0){ //if process didnt finish 
            if(pid.index==0 && is_pqempty(&(queues[0]))){ // if no new process arrived the last process enter first queue again
                process_in_queue[pid.index]=0;
            }
            else //if a new process arrived the last process enter the next level queue
            process_in_queue[pid.index]++;
            penqueue(&(queues[process_in_queue[pid.index]]),pid);
        }
        else{ //compute the stats if finished
            finish_times[pid.index]=current_time;
            turn_around[pid.index]=finish_times[pid.index]-processes[pid.index].arrival_time;
            NormTurn[pid.index]=(float)turn_around[pid.index]/processes[pid.index].st_p;
            total_turnaround+=turn_around[pid.index];
            total_NormTurn+=NormTurn[pid.index];


        }
        
    }
    avg_turnaround=(float)total_turnaround/num_of_processes; //averages
    avg_NormTurn=total_NormTurn/num_of_processes;
    for(int i=0;i<num_of_processes;i++){ //put . in timeline with no * 
        for(int t=processes[i].arrival_time;t<finish_times[i];t++){
            if(time_line[i][t]==' ')
                time_line[i][t]='.';
        }
    }
    if (!(strcmp(type, "stats"))) {
        print_stats(processes, num_of_processes, finish_times, turn_around, NormTurn, avg_turnaround, avg_NormTurn,6, 1);
    } else {
        print_trace(processes, time_line,6, 1);
    }
}
void display_process_queue(processQueue *q) { //for testing
    if (is_pqempty(q)) {
        printf("The queue is empty.\n");
        return;
    }

    printf("Ready Queue: ");
    int i = q->head;
    while (i != q->tail) {
        process p = q->processes[i];
        printf("[Process %c | Arrival: %d | Priority: %d] ", 
                p.name, p.arrival_time, p.st_p);
        i = (i+1) % MAX_QUEUE_SIZE;
        printf("\n");  // Circular queue
    }
    printf("\n");
}

void aging(process processes[],int q){
    
    char time_line[num_of_processes][endtime];
    
    processQueue ready_queue;
    init_pqueue(&ready_queue,1);
    for (int i=0;i<num_of_processes;i++) { //initialize timeline
        for (int t=0;t<endtime;t++) {
            time_line[i][t]=' ';
        }
    }
    for(int i=0;i<num_of_processes;i++){
        
        processes[i].index=i;
        processes[i].waiting_time=0;
        processes[i].initial_priority=processes[i].st_p;
        
    }
    int current_time=0,start_time=0;
    
    
    for(int i=0;i<num_of_processes;i++){ //enqueue first process 
            if(processes[i].arrival_time<=current_time && !pis_in_queue(&ready_queue,processes[i]))
            {   penqueue(&(ready_queue),processes[i]);
            }
            }
    
    
    while(current_time<endtime){
        start_time=current_time; //set start time as curent time
        sort_pqueue_by_priority(&ready_queue); //sort 
        
        process pid=pdequeue(&ready_queue);// gert first queue with highest priority
        pid.waiting_time=0; //reset waiting time
        pid.st_p=pid.initial_priority; //reset priority
        for (int t=current_time;t<current_time+q && t<endtime;t++) {
            time_line[pid.index][t]='*';
        }
        
        current_time+=q; //update current
        
        
        int h=ready_queue.head;
        while(h!=ready_queue.tail){ //update priorities of processes in ready queue
            ready_queue.processes[h].waiting_time+=q;
            ready_queue.processes[h].st_p+=q;
            h=(h+1)%MAX_QUEUE_SIZE;
        }
        penqueue(&ready_queue,pid); //add the last ran process back to the queue
        for(int i=0;i<num_of_processes;i++){ //check for arrival of new processes
            if( processes[i].arrival_time>start_time&& processes[i].arrival_time<=current_time && !pis_in_queue(&ready_queue,processes[i]))
            {   processes[i].st_p++;
                penqueue(&(ready_queue),processes[i]);
            }
        
        
        
        //pid.st_p++;
        
            
        }
        //display_process_queue(&ready_queue);
        sort_pqueue_by_priority(&ready_queue);
        //printf("5\n");
        
        
    }
    for(int i=0;i<num_of_processes;i++){
        for(int t=processes[i].arrival_time;t<endtime;t++){
            if(time_line[i][t]==' ')
                time_line[i][t]='.';
        }
    }
    
    print_trace(processes, time_line,8, 1);
    

}
int main() {
    char line1[20];
    char line2[50];
    char line3[5];
    char line4[5];
    char p_line[20];
    fgets(line1,sizeof(line1),stdin);
    //printf("%s\n",line1);
    line1[strcspn(line1,"\n")]='\0';
    fgets(line2,sizeof(line2),stdin);
    //printf("%s\n",line2);
    line2[strcspn(line2,"\n")]='\0';
    fgets(line3,sizeof(line3),stdin);
    //printf("%s\n",line3);
    line3[strcspn(line3,"\n")]='\0';
    fgets(line4,sizeof(line4),stdin);
    //printf("%s\n",line4);
    line4[strcspn(line4,"\n")]='\0';
    endtime=atoi(line3);
    num_of_processes=atoi(line4);
    
    process processes[num_of_processes];
    for(int i=0;i<num_of_processes;i++){
        fgets(p_line,sizeof(p_line),stdin);
        p_line[strcspn(p_line,"\n")]='\0';
        sscanf(p_line,"%c,%d,%d",&processes[i].name,&processes[i].arrival_time,&processes[i].st_p);
       // printf("Process %d: %c Arrival: %d Service: %d\n",i+1,processes[i].name,processes[i].arrival_time,processes[i].st_p);
    }
    int num_of_algorithms=1;
    for(int i=0;i<strlen(line2);i++){
        if(line2[i]==',')
            num_of_algorithms++;
    }
    algorithm algorithms[num_of_algorithms];
    char *token=strtok(line2, ",");
    int idx=0;

    
    while (token != NULL) {
        int number=0;
        int q=-1;
        char *hyphen_pos=strchr(token,'-');
        
        if (hyphen_pos!=NULL) {
            *hyphen_pos='\0'; 
            number=atoi(token);  
            q=atoi(hyphen_pos+1); 
        } else {
            number=atoi(token);
        }
        algorithms[idx].number=number;
        algorithms[idx].q=q;
        idx++;
        token=strtok(NULL,",");
    }
    
    
   for (int i = 0; i < num_of_algorithms; i++) {

    if(algorithms[i].number==1)
        FCFS(processes, line1);
    if(algorithms[i].number==2)
        RoundRobin(processes, line1, algorithms[i].q);
    if(algorithms[i].number==3)
        SPN(processes, line1);
    if(algorithms[i].number==4)
        SRT(processes, line1);
    if(algorithms[i].number==5)
        HRRT(processes, line1);
    if(algorithms[i].number==6)
        FB1(processes, line1);
    if(algorithms[i].number==7)
        FB_2i(processes, line1);
    if(algorithms[i].number==8){
        
        aging(processes, algorithms[i].q);
}
   }

    return 0;
}