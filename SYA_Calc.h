/**@wlx
  *calculate simple expressions in string
  *based on SYA
  *finished on 2019.11.13
  */

#include "opstack.h" //A stack implemented by myself
#include <ctype.h>

#define isoperator(c) (c == '+' || c == '-' || c == '/' || c == '*' || c == '%' || c == '=')
int op_prior(const char c); 
int op_argnum(const char c); 
int SYA(char* infix, char* postfix); //Shunting Yard Algorithm
int calcpostfix(char* postfix, double* result); 
double calcSingle(char signal, double *op, int *error_flag);

int Calc(char* expr, double *result);

/**Implementation of functions
  *
  */

int Calc(char* expr, double *result) {
	char postfix[MAXSIZE] = "";
	if (*expr) {
		if (SYA(expr, postfix)) {
			if (calcpostfix(postfix, result)) {
				ErrReport_illegalExpr(expr);
				return 1;
			}
		} else {
			ErrReport_illegalExpr(expr);
			return 1;
		}
	} 
	return 0;
}

int calcpostfix(char* postfix, double* result) {
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
					//puts("argument number unmatched");
					return 1;
				}
			}
			int error_flag = 0;
			tempval = calcSingle(*word, op, &error_flag);
			if (error_flag) {
				return 1;
			} else {
				calcstack.push(&calcstack, &tempval, typeof(tempval));
			}
		}
	}
	if (calcstack.not_empty == 1) {
		calcstack.pop(&calcstack, result, typeof(*result));
	} else if(calcstack.not_empty > 1) {
		//puts("argument number unmatched");
		return 1;
	}
	
	return 0;
}

double calcSingle(char signal, double op[], int* error_flag) {
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
				//puts("% operation demands integers");
				*error_flag = 1;
				return 0;
			} break;
		case '=':
			return op[0] == op[1];
		default :
			//puts("unkonwn operator");
			*error_flag = 2;
			return 0;
	}
}

int SYA(char* infix, char* postfix) {
	stack opstack;
	stack_init(&opstack);
	char* outputstart = postfix;
	if (!isdigit(*infix) && *infix != '(') {
		//puts("not begin with number or parentheses");
		opstack.destroy(&opstack);
		return 0;
	}
	while (*infix) {
		if (isdigit(*infix)){
			*(postfix++) = *(infix++);
		} else if (*infix == '.'&&isdigit(*(infix-1))) {
			char *temp = postfix-1, flag = 1;
			while (*temp != ' ' && temp >= outputstart) { //check if there's more than one '.' in one number
				if (*(temp--) == '.') {
					flag = 0;
					break;
				}
			}
			if (flag) {
				*(postfix++) = *(infix++);
			} else {
				//puts("invalid number input");
				opstack.destroy(&opstack);
				return 0;
				//++infix
			}
		} else if (isoperator(*infix)) {
			while (opstack.not_empty) {
				if (op_prior(opstack.top(&opstack, typeof('c'))) >= op_prior(*infix)) {// 1==typeof('c')
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
				//puts("unmatched parentheses");
				opstack.destroy(&opstack);
				return 0;
			}
			++infix;
		} else {
			//puts("illegal character");
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
			//puts("unmatched parentheses");
			opstack.destroy(&opstack);
			return 0;
		}
	}
	
	opstack.destroy(&opstack);
	return 1;
}

//priority of operator
int op_prior(const char c) {
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

//argument number of a operator
int op_argnum(const char c) {
	switch (c) {
		case '+': case '-': case '*': case '/': case '%': case '=': //here '=' means 'compare'
			return 2;
		default :
			return 0;
	}
}

