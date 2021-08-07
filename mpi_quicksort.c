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
    float *chunk,*data;
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
        printf("\nUsing %d processor cores\n",np);
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
    chunk_size = (N%np==0)?(N/np):(N/np + 1);
    
    switch (N)
    {
        case 1:
            this_chunk = 1;
            chunk = (float *)malloc(this_chunk *sizeof(float));
            chunk[0]=input[0];
            break;
        case 2:
            this_chunk = 2;
            chunk = (float *)malloc(this_chunk *sizeof(float));
            chunk[0]=input[0];
            chunk[1]=input[1];
            if(input[0]>input[1]){
                chunk[0]=input[1];
                chunk[1]=input[0];
            }
            break;
        default:
            this_chunk = (N >= chunk_size*(rank+ 1))?chunk_size : (N-chunk_size*rank);
            chunk = (float *)malloc(this_chunk *sizeof(float));
            MPI_Scatter(input, chunk_size, MPI_FLOAT, chunk,
            chunk_size, MPI_FLOAT, 0, MPI_COMM_WORLD);
    
            for(i=0;i<this_chunk;i++)
                push(chunk[i],&stack);

            quicksort(&stack, this_chunk);
            
            data = NULL;
            chunk = (float *)malloc(this_chunk *sizeof(float));

            for(int j=this_chunk-1;j>=0;j--){ chunk[j] = pop(&stack);}

            //Merge Step
            int c = 1;//exponential counter
            while(c<np){
                if (rank % (2*c) != 0) { //Senders
                    MPI_Send(chunk, this_chunk, MPI_INT, rank - c, 0, MPI_COMM_WORLD);
                    break;
                }
                if (rank + c < np) { //Receivers, 0 is always a receiver and the merges ends in 0
                    int received_chunk_size = (N >= chunk_size*(rank + 2*c))?(chunk_size*c):(N - chunk_size*(rank + c));
                    float* chunk_received;
                    chunk_received = (float*)malloc(received_chunk_size * sizeof(float));

                    MPI_Recv(chunk_received, received_chunk_size, MPI_INT, rank + c, 0, MPI_COMM_WORLD, &status);

                    data = merge(chunk, this_chunk, chunk_received, received_chunk_size);

                    free(chunk);
                    free(chunk_received);
                    chunk = data;
                    this_chunk += received_chunk_size;
                }
                c = 2*c;
            }
            MPI_Barrier(MPI_COMM_WORLD);//Just waits till everything is completed
            break;
    }

    //printf("P_rank: %d, chunk_size: %d\n",rank,this_chunk);
    end = MPI_Wtime();
    el_time+=(double)(end-start);
    if(!rank){
        f = fopen("out_mpi.txt","w"); 
        if(f == NULL){perror("\nCannot access current directory");}
        fprintf(f,"%.5f\n",chunk[0]);
        for(i=0;i<this_chunk-1;i++){
	        if(chunk[i]>chunk[i+1]) flag=0;//Checks if the array is ordered
            fprintf(f,"%.5f\n",chunk[i+1]);
        }
        fclose(f);
        printf(" -> Elapsed time: %f seconds\n",el_time);
        printf(" -> Array is ordered: ");
        printf("%s\n", flag?"true":"false");
        printf(" -> Output file is out_mpi.txt\n");
    }
    MPI_Finalize();
    //End
    return 0;
}

void quicksort(struct element** stack, int it){
    float pivot = (float) pop(&(*stack));
    if(it <= 1){
        push(pivot, &(*stack));
        return;
    }
    /*Stacks initialization
        - rstacks needed to flip the stack
        - I've implemented the algorithm via linked list stacks,
            using arrays was too easy
    */
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
    float tmp;
    int c1=1, c2=0, rc1=0, rc2=0, count = 1;

    while (count < it){
        tmp = (float) pop(&(*stack));
        if(tmp<pivot){
            push(tmp,&stack1);
            c1++;
        }
        else{
            push(tmp,&stack2);
            c2++;
        }
        count++;
    }
    rc1=c1;rc2=c2;
    //Recursive step
    quicksort(&stack1,c1);
    quicksort(&stack2,c2);
    while(c1){
        tmp = (float)pop(&stack1);
        push(tmp,&(rstack1));
        c1--;
    }
    while(c2){
        tmp = (float)pop(&stack2);
        push(tmp,&(rstack2));
        c2--;
    }
    while(rc1){
        tmp = (float)pop(&rstack1);
        push(tmp,&(*stack));
        rc1--;
    }
    
    while(rc2){
        tmp = (float)pop(&rstack2);
        push(tmp,&(*stack));
        rc2--;
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
