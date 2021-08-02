#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include "stack.c"

float* merge(float* arr1, int n1, float* arr2, int n2);
void quicksort(struct element** stack, int it);

int main(int argc, char** argv) {
    if(argc!= 3){
        perror("Usage: mpirun ./mpi_quicksort <filename> <size>");
        return 1;
    }
    int N = atoi(argv[2]);
    float input[N];
    float *chunk,* ip = input,*data;
    int this_chunk;
    int i,j,k,chunk_size,flag=1;
    struct element* stack = (struct element*)malloc(sizeof(struct element));
    stack -> next = NULL;
    stack -> data =(float) 0;
    FILE *f = NULL;
    MPI_Status status;
    double start, end, el_time;
    int rank, buf, np;
    //start

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    if (!rank){
        printf("\nUsing	%d processor cores\n",np);
        f = fopen(argv[1],"r");
        if(f == NULL) {
            printf("Error in opening file\n");
            return 1;
        }
        for(i=0;i<N;i++)fscanf(f,"%f\n",&input[i]);
        fclose(f);
        f = NULL;
    }
    MPI_Barrier(MPI_COMM_WORLD);

    start = MPI_Wtime();

    chunk_size = (N % np == 0)?(N/np):(N/np + 1);
    this_chunk = (N >= chunk_size*(rank+ 1))?chunk_size : (N-chunk_size*rank);
    chunk = (float *)malloc(this_chunk *sizeof(float));

    //printf("P_rank: %d, chunk_size: %d\n",rank,this_chunk);
    
    MPI_Scatter(input, chunk_size, MPI_FLOAT, chunk,
        chunk_size, MPI_FLOAT, 0, MPI_COMM_WORLD);
    
    for(i=0;i<this_chunk;i++)
        push(chunk[i],&stack);
    
    quicksort(&stack, 0);
    data = NULL;
    chunk = (float *)malloc(this_chunk *sizeof(float));
    for(int j=this_chunk-1;j>=0;j--){ chunk[j] = pop(&stack);
    //printf("%f\n",chunk[j]);
    }

    //printf("Sorted chunk from processor %d\n", rank);
    int count = 1;

    //Merge Step
    for(int c = 1; c < np; c = 2 * c){
        if (rank % (2*c) != 0) {
            MPI_Send(chunk, this_chunk, MPI_INT, rank - c, 0, MPI_COMM_WORLD);
            break;
        }
        if (rank + c < np) {
            int received_chunk_size = (N >= chunk_size*(rank + 2*c))?(chunk_size*c):(N- chunk_size*(rank + c));
            float* chunk_received;
            chunk_received = (float*)malloc(received_chunk_size * sizeof(float));
            
            MPI_Recv(chunk_received, received_chunk_size, MPI_INT, rank + c, 0, MPI_COMM_WORLD, &status);
 
            data = merge(chunk, this_chunk, chunk_received, received_chunk_size);
            
            free(chunk);
            free(chunk_received);
            chunk = data;
            this_chunk += received_chunk_size;
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();
    el_time+=(double)(end-start);
    if(!rank){
        //Checks if the array is ordered
        f = fopen("out.txt","w");
        if(f == NULL){perror("\nCannot access current directory");}
        fprintf(f,"%.5f\n",chunk[0]);
        for(i=0;i<this_chunk-1;i++){
	        if(chunk[i]>chunk[i+1])flag=0;
            fprintf(f,"%.5f\n",chunk[i+1]);
        }
        fclose(f);
        printf("\n    **Results**\n");
        printf("Elapsed time: %f seconds\n",el_time);
        printf("Sorted Elements: %d\n", this_chunk);
        printf("Array is ordered: ");
        printf("%s\n", flag?"true":"false");
        printf("Output file is out.txt\n");
    }
    MPI_Finalize();
    return 0;
}

void quicksort(struct element** stack, int it){
    float pivot = (float) pop(&(*stack));
    if(pivot==(float)0) return;
    float tmp;
    struct element* stack1 = (struct element*)malloc(sizeof(struct element));
    stack1 -> next = NULL;
    stack1 -> data =(float) 0;
    struct element* stack2 = (struct element*)malloc(sizeof(struct element));
    stack2 -> next = NULL;
    stack2 -> data = (float) 0;
    struct element* rstack1 = (struct element*)malloc(sizeof(struct element));
    rstack1 -> next = NULL;
    rstack1 -> data = (float) 0;
    struct element* rstack2 = (struct element*)malloc(sizeof(struct element));
    rstack2 -> next = NULL;
    rstack2 -> data = (float) 0;
    push(pivot, &stack1);
    tmp = (float) pop(&(*stack));
    do{
        if(tmp==0){
            push(0, &(*stack));
            push(pivot, &(*stack));
            free(stack1);
            free(stack2);
            free(rstack1);
            free(rstack2);
            return;
        } 
        if(tmp<pivot){
            push(tmp,&stack1);
        }
        else{
            push(tmp,&stack2);
        }
    }while(tmp = pop(&(*stack)));

    //Recursive step
    quicksort(&stack1,it+1);
    quicksort(&stack2,it+1);

    push(0,&(*stack));
    
    while(tmp = (float)pop(&stack1)){
        push(tmp,&(rstack1));
    }
    while(tmp = (float)pop(&stack2)){
        push(tmp,&(rstack2));
    }
    while(tmp = (float)pop(&rstack1)){
        push(tmp,&(*stack));
    }
    while(tmp = (float)pop(&rstack2)){
        push(tmp,&(*stack));
    }
    free(stack1);
    free(stack2);
    free(rstack1);
    free(rstack2);
    return;
}

float* merge(float* arr1, int n1, float* arr2, int n2){   
    float* res = (float*)malloc((n1 + n2) * sizeof(float));
    int i=0,j=0,k;
    for (k = 0; k < n1 + n2; k++) {
        if (i >= n1) {
            res[k] = arr2[j];
            j++;}
        else if (j >= n2) {
            res[k] = arr1[i];
            i++;}
        else if (arr1[i] < arr2[j]) {
            res[k] = arr1[i];
            i++;}
        else {
            res[k] = arr2[j];
            j++;}
    }
    return res;
}
