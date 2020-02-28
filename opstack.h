/**@wlx
  *Stack for calculator implemented with circular linked list
  *finished on 2019.11.8
  */
#ifndef OPSTACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define OPSTACK_H


/**
 *  CAUTION! in C, literal char is stored in int type (not char type!)
 *  For example, 'c' has type int (instead of char)
 *  BTW... in case of C++, 'c' do has char type
 *  it seems to be defined by C standard rather than just a implementation issue... So it just sucks
 */
#define typeof(x) _Generic((x), double : 0, char : 1, int : 1) //C11 feature

typedef struct Node *dbList;
typedef struct stack_info stack;
void stack_init(stack *itself);
void stack_destory(stack *itself);
static int stack_top(stack *itself, int datatype);
static void stack_pop(stack *itself, void *target, int datatype);
static void stack_push(stack *itself, void *data, int datatype);

struct Node { // Node of circular linked list
    union {
        double oprand;
        char operator;
    }; //C11 feature
    dbList prev;
    dbList next;
};
struct stack_info { //container
    dbList head;
    int not_empty; // the number of elements in the stack
    int (*top)(stack *itself, int datatype);
    void (*pop)(stack *itself, void *target, int datatype);
    void (*push)(stack *itself, void *data, int datatype);
    void (*destroy)(stack *itself);
};

void stack_init(stack *itself) {
    itself->head = (dbList)calloc(1, sizeof(struct Node));

    if (itself->head) {
        itself->head->prev = itself->head;
        itself->head->next = itself->head;
    } else {
        puts("Stack initalization falied");
        return;
    }
    itself->top = stack_top;
    itself->pop = stack_pop;
    itself->push = stack_push;
    itself->destroy = stack_destory;
    itself->not_empty = 0;
}

void stack_destory(stack *itself) {
    while (itself->head->prev != itself->head) {
        dbList temp = itself->head->prev;
        itself->head->prev = itself->head->prev->prev;
        itself->head->prev->prev->next = itself->head;
        free(temp);
    }
    free(itself->head);
}

static int stack_top(stack *itself, int datatype) {
    return datatype ? itself->head->prev->operator: itself->head->prev->oprand;
}

static void stack_pop(stack *itself, void *target, int datatype) {
    //void* ret_val = datatype ? &itself->head->prev->operator : &itself->head->prev->oprand;
    if (target) {
        if (datatype) {
            memmove(target, &itself->head->prev->operator, sizeof(char));
        } else {
            memmove(target, &itself->head->prev->oprand, sizeof(double));
        }
    }

    if (itself->not_empty > 1) {
        dbList temp = itself->head->prev;
        itself->head->prev = itself->head->prev->prev;
        itself->head->prev->prev->next = itself->head;
        free(temp);
    }
    --itself->not_empty;
}

static void stack_push(stack *itself, void *data, int datatype) {
    if (itself->not_empty) {
        dbList newNode = (dbList)calloc(1, sizeof(struct Node));
        if (datatype) {
            memmove(&newNode->operator, data, sizeof(char));
            //newNode->operator = (char) *data;
        } else {
            memmove(&newNode->oprand, data, sizeof(double));
            //newNode->oprand = (double) *data;
        }
        newNode->next = itself->head;
        newNode->prev = itself->head->prev;
        itself->head->prev->next = newNode;
        itself->head->prev = newNode;
    } else {
        if (datatype) {
            memmove(&itself->head->operator, data, sizeof(char));
            //itself->head->operator = (char) *data;
        } else {
            memmove(&itself->head->oprand, data, sizeof(double));
            //itself->head->oprand = (double) *data;
        }
    }
    ++itself->not_empty;
}

#endif // (!defined OPSTACK_H)