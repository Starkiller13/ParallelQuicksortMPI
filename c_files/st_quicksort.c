#include <stdio.h>
#include <time.h>
#include "quicksort.c"

int main(int argc, char** argv) {
    if(argc!= 3){
        perror("Usage: mpirun ./mpi_quicksort <filename> <size>");
        return 1;
    }
    int i,j,k,flag=1;
    double el_time;
    FILE *f;
    struct element* stack;
    stack = (struct element*)malloc(sizeof(struct element));
    stack -> next = NULL;
    stack -> data = 0;
    int N = atoi(argv[2]);
    float a[N];
    f = fopen(argv[1],"r");
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
    f = fopen("outputs/out_st.txt","w");
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
    printf(" -> Output file is outputs/out_st.txt\n");
    return 0;
}

