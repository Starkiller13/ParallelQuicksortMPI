#include <stdio.h>
#include <omp.h>
#include <time.h>
#include "stack.c"
int i,j,k,flag=1;
FILE *f;
float el_time;
void quicksort_bs(float ** A, int n);
void quicksort(struct element** stack, int it);

int main(int argc, char** argv) {
    int N = 1000000;
    float a[N];
    f= fopen("numbersBIG.txt","r");
    for(i=0;i<N;i++)fscanf(f,"%f\n",&a[i]);
    fclose(f);
    float * a_p = a;
    struct element* stack = (struct element*)malloc(sizeof(struct element));
    stack -> next = NULL;
    stack -> data = 0;
    double begin = omp_get_wtime();
    #pragma omp parallel
    {
      #pragma omp single
      {
      
      quicksort_bs(&a_p, N);
      
      }
    }
    double end = omp_get_wtime();
    el_time+=(double)(end-begin);
    for(i=0;i<N-1;i++){
	    if(a[i]>a[i+1])flag=0;
    }
    printf("Elapsed time %f seconds",el_time);
    printf("\nArray is ordered:");
    printf("%s\n", flag?"true":"false");
}

//Works with floats greater than 0
void quicksort_bs(float ** a, int n){
    struct element* stack = (struct element*)malloc(sizeof(struct element));
    stack -> next = NULL;
    stack -> data = 0;
    #pragma omp parallel for
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
    int my_num = omp_get_thread_num();
    //printf("Level %d running on thread %d\n",it,my_num);
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
            return;
        } 
        if(tmp<pivot){
            push(tmp,&stack1);
        }
        else{
            push(tmp,&stack2);
        }
    }while(tmp = pop(&(*stack)));

    #pragma omp task shared(stack1)
    {
      quicksort(&stack1,it+1);
    }
    #pragma omp task shared(stack2)
    {
      quicksort(&stack2,it+1);
    }
    #pragma omp taskwait
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

