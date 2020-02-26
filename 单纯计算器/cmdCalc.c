/**@wlx
  *simple cmd calculator
  *Method: 调度场
  *finished on 2019.11.12
  */
#define OPSTACK_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "opstack.h" //一个自己实现的栈

#define MAXSIZE 100                    
#define BUFFERLEN 14
#define isoperator(c) (c == '+' || c == '-' || c == '/' || c == '*' || c == '%' || c == '=')
char* s_gets(char *st, int n);
char* readByWord(char *str_input, char *word);
int op_preced(const char c); 
int op_argnum(const char c); 
int SYA(char* infix, char* postfix); //调度场
int calcpostfix(char* postfix); //计算后缀表达式
double calc(char signal, double *op, int *error_flag);

int main(void) {
	puts("Simple Command Line Calculator");
	char read[MAXSIZE] = "";
	
	while (s_gets(read, MAXSIZE)) {
		char postfix[MAXSIZE] = "";
		if(*read) {
			if (SYA(read, postfix)) {
				//puts(postfix);
				if (calcpostfix(postfix))
					puts("try again with a proper expression!");
			} else {
				puts("try again with a proper expression!");
			}
		}
	}
	return 0;
}

int calcpostfix(char* postfix) {
	char word[BUFFERLEN] = "", *readptr = postfix;
	double tempval = 0;
	stack calcstack;
	stack_init(&calcstack);
	while ( (readptr = readByWord(readptr, word)) ) {
		//puts(word);
		if (isdigit(*word)) {
			sscanf(word, "%lf", &tempval);
			calcstack.push(&calcstack, &tempval, typeof(tempval));
		} else if (isoperator(*word)) {
			int argnum_need = op_argnum(*word);
			double op[argnum_need];
			for (int i = 1; i <= argnum_need ; ++i) {
				if (calcstack.not_empty) {
					calcstack.pop(&calcstack, op+argnum_need-i, 0);
				} else if (i <= argnum_need) {
					puts("argument number unmatched");
					return 1;
				}
			}
			int error_flag = 0;
			tempval = calc(*word, op, &error_flag);
			if (error_flag) {
				return 1;
			} else {
				calcstack.push(&calcstack, &tempval, typeof(tempval));
			}
		}
	}
	if (calcstack.not_empty == 1) {
		double show;
		calcstack.pop(&calcstack, &show, typeof(show));
		printf("%g\n", show);
	} else if(calcstack.not_empty > 1) {
		puts("argument number unmatched");
		return 1;
	}
	
	return 0;
}

double calc(char signal, double *op, int* error_flag) {
	switch(signal) {
		case '+':
			return op[0]+op[1];
			break;
		case '-':
			return op[0]-op[1];
			break;
		case '*':
			return op[0]*op[1];
			break;
		case '/':
			return op[0]/op[1];
			break;
		case '%':
			if ((int)op[0] == op[0] && (int)op[1] == op[1]) {
				return (int)op[0] % (int)op[1];
			} else {
				puts("% operation demands integers");
				*error_flag = 1;
				return 0;
			} break;
		case '=':
			return op[0] == op[1];
		default :
			puts("unkonwn operator");
			*error_flag = 2;
			return 0;
	}
}

int SYA(char* infix, char* postfix) {
	stack opstack;
	stack_init(&opstack);
	char* outputstart = postfix;
	if (!isdigit(*infix) && *infix != '(') {
		puts("not begin with number or parentheses");
		opstack.destroy(&opstack);
		return 0;
	}
	while (*infix) {
		if (isdigit(*infix)){
			*(postfix++) = *(infix++);
		} else if (*infix == '.'&&isdigit(*(infix-1))) {
			char *temp = postfix-1, flag = 1;
			while (*temp != ' ' && temp >= outputstart) {
				if (*(temp--) == '.') {
					flag = 0;
					break;
				}
			}
			if (flag) {
				*(postfix++) = *(infix++);
			} else {
				puts("invalid number input");
				opstack.destroy(&opstack);
				return 0;
				//++infix
			}
		} else if (isoperator(*infix)) {
			while (opstack.not_empty) {
				if (op_preced(opstack.top(&opstack, typeof('c'))) >= op_preced(*infix)) {// 1==typeof('c')
					*(postfix++) = ' ';	
					opstack.pop(&opstack, postfix++, typeof('c'));
				} else {
					break;
				}
			}
			*(postfix++) = ' ';	
			opstack.push(&opstack, infix++, typeof('c'));
		} else if (*infix == '(') {
			opstack.push(&opstack, infix++, typeof('c'));
		} else if (*infix == ')') {
			while (opstack.not_empty && opstack.top(&opstack, typeof('c'))!='(') {
				*(postfix++) = ' ';
				opstack.pop(&opstack, postfix++, typeof('c'));
			}
			if (opstack.not_empty) {
				opstack.pop(&opstack, NULL, typeof('c'));
			} else {
				puts("unmatched parentheses");
				opstack.destroy(&opstack);
				return 0;
			}
			++infix;
		} else {
			puts("illegal character");
			opstack.destroy(&opstack);
			return 0;
		}
	}
	
	if (opstack.not_empty) {
		while (opstack.not_empty && opstack.top(&opstack, typeof('c')) != '(') {
			*(postfix++) = ' ';	
			opstack.pop(&opstack, postfix++, typeof('c'));
		}
		if (opstack.top(&opstack, typeof('c')) == '(') {
			puts("unmatched parentheses");
			opstack.destroy(&opstack);
			return 0;
		}
	}
	
	opstack.destroy(&opstack);
	return 1;
}





int op_preced(const char c) {
	switch (c) {
		case '*': case '/': case '%':
			return 3;
		case '+': case '-': 
			return 2;
		case '=': 
			return 1;
		default :		
			return 0;
	}
}

int op_argnum(const char c) {
	switch (c) {
		case '+': case '-': case '*': case '/': case '%': case '=': 
			return 2;
		default :
			return 0;
	}
}

char* readByWord(char *str_input, char *word) {
	char* temp = str_input;
	if (*temp) {
		for (int i = 0; *temp != ' ' && *temp != '\0' && i < BUFFERLEN-1; ++i) {
			*(word++) = *(temp++);
		}
		*word = '\0';
		if (*temp == ' ') {
			return temp+1;
		} else if (*temp == '\0') {
			return temp;
		} else {
			while (*(temp++) != ' ');
			return temp;
		}
	} else {
		return NULL;
	}
}

char* s_gets(char *st, int n) {
	register char* ret_val;
	register char* find;

	ret_val = fgets(st, n, stdin);
	if (ret_val) {
		find = strchr(st, '\n');        //replace '\n' with '\0'
		if (find)
			*find = '\0';
		else
			while (getchar() != '\n')   //eatline
				continue;
	}
	return ret_val;
}