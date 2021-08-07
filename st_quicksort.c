#include <stdio.h>
#include "stack.c"
#include<time.h>
int i,j,k,flag=1;
double el_time;
FILE *f;
struct element* stack;

void quicksort(struct element** stack, int it);

int main(int argc, char** argv) {
    if(argc!= 3){
        perror("Usage: mpirun ./mpi_quicksort <filename> <size>");
        return 1;
    }
    stack = (struct element*)malloc(sizeof(struct element));
    stack -> next = NULL;
    stack -> data = 0;
    int N = atoi(argv[2]);
    float a[N];
    f= fopen(argv[1],"r");
    if(f == NULL){
        perror("\nFile not found");
        return 1;
    }
    for(i=0;i<N;i++)fscanf(f,"%f\n",&a[i]);
    fclose(f);
    clock_t begin = clock();
    for(i=0;i<N;i++){push((a)[i],&stack);}
    
    quicksort(&stack, N);

    for(i=N-1;i>=0;i--){a[i] = pop(&stack);}
    free(stack);
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
	    if(a[i]>a[i+1]) flag=0;
        fprintf(f,"%.5f\n",a[i+1]);
    } 
    fclose(f);
    printf(" -> Elapsed time: %f seconds\n",el_time);
    printf(" -> Array is ordered: ");
    printf("%s\n", flag?"true":"false");
    printf(" -> Output file is out_st.txt\n");
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