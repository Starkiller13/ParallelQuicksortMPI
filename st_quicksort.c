#include <stdio.h>
#include "stack.c"
#include<time.h>
int i,j,k,flag=1;
double el_time;
FILE *f;

void quicksort_bs(float ** A, int n);
void quicksort(struct element** stack, int it);

int main(int argc, char** argv) {
    if(argc!= 3){
        perror("Usage: mpirun ./mpi_quicksort <filename> <size>");
        return 1;
    }
    int N = atoi(argv[2]);
    float a[N];
    f= fopen(argv[1],"r");
    if(f == NULL){
        perror("\nFile not found");
        return 1;
    }
    for(i=0;i<N;i++)fscanf(f,"%f\n",&a[i]);
    fclose(f);
    float * a_p = a;
    clock_t begin = clock();
    quicksort_bs(&a_p, N);
    clock_t end = clock();
    for(i=0;i<N-1;i++){
	if(a[i]>a[i+1])flag=0;
    }
    el_time+=(double)(end-begin)/CLOCKS_PER_SEC;
    //Checks if the array is ordered
    f = fopen("out_st.txt","w");
    if(f == NULL){perror("\nCannot access current directory");}
    fprintf(f,"%f\n",a[0]);
    for(i=0;i<N-1;i++){
	    if(a[i]>a[i+1])flag=0;
        fprintf(f,"%.5f\n",a[i+1]);
    }
    fclose(f);
    printf("\n    **Results**\n");
    printf("Elapsed time: %f seconds\n",el_time);
    printf("Array is ordered: ");
    printf("%s\n", flag?"true":"false");
    printf("Output file is out_st.txt\n");
    return 0;
}

//Works with floats greater than 0
void quicksort_bs(float ** a, int n){
    float *A = (float*)malloc(n*sizeof(float));
    struct element* stack = (struct element*)malloc(sizeof(struct element));
    stack -> next = NULL;
    stack -> data = 0;

    for(i=0;i<n;i++){
        push((*a)[i],&stack);
    }

    quicksort(&stack,0);

    for(i=n-1;i>=0;i--){
        (*a)[i] = pop(&stack);
    }
    free(stack);
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

