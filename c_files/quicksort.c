#include "stack.c"
void quicksort(struct element** stack, int n_el){
    float pivot = (float) pop(&(*stack));
    if(n_el <= 1){
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

    while (count < n_el){
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
    free(stack1);
    free(stack2);
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
    free(rstack1);
    free(rstack2);
    return;
}