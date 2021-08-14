#include<stdio.h>
#include<stdlib.h>
// Struct to hold the data and the pointer to the next element.
struct element{ 
    float data; 
    struct element* next; 
};
// Append the new element to the start of the stack
void push(float data, struct element** stack){
    struct element* Element = (struct element*)malloc(sizeof(struct element)); 
    Element -> data = data; 
    Element -> next = *stack;  
    (*stack) = Element;  
}
// Remove element from the top of the stack
float pop(struct element** stack){
    float r=0;
    if(*stack != NULL){
        r = (*stack) -> data;
        struct element* tempPtr = *stack;
        *stack = (*stack) -> next;
        //free(tempPtr);
    }
    return r;
    
}
// Display the element at the top of the stack
void top(struct element* stack){
    if(stack != NULL){
    printf("Element on top: %f\n", stack -> data);
    }
    else{
        printf("The stack is empty.\n");
    }
}