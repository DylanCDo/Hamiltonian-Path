#include "stack.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//The stack structure provided by the assignment pdf.
struct Stack {
    uint32_t top;
    uint32_t capacity;
    uint32_t *items;
};

//This function is responsible for allocating memory for any created stack. This code is provided by the assignment pdf.
Stack *stack_create(uint32_t capacity) {
    Stack *s = (Stack *) malloc(sizeof(Stack));
    if (s) {
        s->top = 0;
        s->capacity = capacity;
        s->items = (uint32_t *) calloc(capacity, sizeof(uint32_t));
        if (!s->items) {
            free(s);
            s = NULL;
        }
    }
    return s;
}

//This function is responsible for freeing the allocated memory of a stack. This code is provided by the assignment pdf.
void stack_delete(Stack **s) {
    if (*s && (*s)->items) {
        free((*s)->items);
        free(*s);
        *s = NULL;
    }
    return;
}

//This function returns the top of the stack which would be the stack size.
uint32_t stack_size(Stack *s) {
    return s->top;
}

//This function checks if the stack is empty and returns true if it is and false otherwise.
bool stack_empty(Stack *s) {
    if (stack_size(s) == 0) {
        return true;
    }
    return false;
}

//This function checks if the stack is full and returns true if it is and false otherwise.
bool stack_full(Stack *s) {
    if (stack_size(s) == s->capacity) {
        return true;
    }
    return false;
}

//This function is responsible pushing a vertex onto a stack. Returns true if it is succesful and returns false otherwise.
bool stack_push(Stack *s, uint32_t x) {
    if (!stack_full(s)) {
        s->items[s->top] = x;
        s->top += 1;
        return true;
    }
    return false;
}

//This function is responsible for popping a vertex from a stack. Returns true if it is successful and returns false otherwise.
bool stack_pop(Stack *s, uint32_t *x) {
    if (!stack_empty(s)) {
        s->top -= 1;
        *x = s->items[s->top];
        return true;
    }
    return false;
}

//This function is similar to stack_pop, but instead of altering the top of the stack, it only changes a variable to the top of the stack and returns true if it is succesful and returns false otherwise.
bool stack_peek(Stack *s, uint32_t *x) {
    if (!stack_empty(s)) {
        *x = s->items[s->top - 1];
        return true;
    }
    return false;
}

//This function is responsible for copying a stack onto another. It checks the length of both stacks and determines how much it should copy from one to the other based on the size of the stack. It also copies the top of the stack.
void stack_copy(Stack *dst, Stack *src) {
    if (dst->capacity < src->capacity) {
        for (uint32_t i = 0; i < dst->capacity; i++) {
            dst->items[i] = src->items[i];
        }
        dst->top = src->top;
    } else if (src->capacity <= dst->capacity) {
        for (uint32_t i = 0; i < src->capacity; i++) {
            dst->items[i] = src->items[i];
        }
        dst->top = src->top;
    }
}

//This is the print function that is used in the path.c file. This code was provided by the assignment pdf.
void stack_print(Stack *s, FILE *outfile, char *cities[]) {
    for (uint32_t i = 0; i < s->top; i += 1) {
        fprintf(outfile, "%s", cities[s->items[i]]);
        if (i + 1 != s->top) {
            fprintf(outfile, " -> ");
        }
    }
    fprintf(outfile, "\n");
}
