/**@wlx
  *Stack implemented with double linked list
  *finished on 2019.11.8
  */
#ifndef OPSTACK_H_INCLUDED
#	include <stdio.h>
#	include <stdlib.h>
#	include <string.h>
#	define OPSTACK_H_INCLUDED
#endif

#define typeof(x) _Generic((x), double: 0, char: 1, int: 1)
typedef struct Node *dbList;
typedef struct stack_info stack;
void stack_init(stack * info);
void stack_destory(stack * info);
static int stack_top(stack *info, int datatype);
static void stack_pop(stack *info, void* target, int datatype);
static void stack_push(stack *info, void* data, int datatype);

struct Node { // 双向链表节点
	union {double oprand;char operator;};
	dbList prev;
	dbList next;
};
struct stack_info {
	dbList head;
	int not_empty; // 存放栈中元素个数
	int (*top) (stack *info, int datatype);
	void (*pop) (stack *info, void* target, int datatype);
	void (*push) (stack *info, void* data, int datatype);
	void (*destroy) (stack * info);
};

void stack_init(stack * info) {
	info->head = (dbList) calloc(1, sizeof(struct Node));

	if (info->head) {
		info->head->prev = info->head;
		info->head->next = info->head;
	} else {
		puts("Stack initalization falied");
		return;
	}
	info->top = stack_top;
	info->pop = stack_pop;
	info->push = stack_push;
	info->destroy = stack_destory;
	info->not_empty = 0;  ///???为什么不能去掉这行
}

void stack_destory(stack * info) {
	while (info->head->prev != info->head) {
		dbList temp = info->head->prev;
		info->head->prev = info->head->prev->prev;
		info->head->prev->prev->next = info->head;
		free(temp);
	}
	free(info->head);
}

static int stack_top(stack *info, int datatype) {
	return datatype ? info->head->prev->operator : info->head->prev->oprand;
}

static void stack_pop(stack *info, void* target, int datatype) {
	//void* ret_val = datatype ? &info->head->prev->operator : &info->head->prev->oprand;
	if (target) {
		if (datatype) {
			memmove(target, &info->head->prev->operator, sizeof(char));
		} else {
			memmove(target, &info->head->prev->oprand, sizeof(double));
		}
	}
	
	if (info->not_empty > 1) {
		dbList temp = info->head->prev;
		info->head->prev = info->head->prev->prev;
		info->head->prev->prev->next = info->head;
		free(temp);
	}
	--info->not_empty;
}

static void stack_push(stack *info, void* data, int datatype) {
	if (info->not_empty) {
		dbList newNode = (dbList) calloc(1, sizeof(struct Node));
		if (datatype) {
			memmove(&newNode->operator, data, sizeof(char));
			//newNode->operator = (char) *data;
		} else {
			memmove(&newNode->oprand, data, sizeof(double));
			//newNode->oprand = (double) *data;
		}
		newNode->next = info->head;
		newNode->prev = info->head->prev;
		info->head->prev->next = newNode;
		info->head->prev = newNode;
	} else {
		if (datatype) {
			memmove(&info->head->operator, data, sizeof(char));
			//info->head->operator = (char) *data;
		} else {
			memmove(&info->head->oprand, data, sizeof(double));
			//info->head->oprand = (double) *data;
		}
	}
	++info->not_empty;
}

