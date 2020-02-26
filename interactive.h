/**@wlx
  *finished on 2019.11.13
  */
  
#ifndef OPSTACK_H_INCLUDED
#	include <stdio.h>
#	include <stdlib.h>
#	include <string.h>
#	define OPSTACK_H_INCLUDED
#endif
#include <windows.h> //for changing the color of cmd

#define MAXSIZE 100
#define BUFFERLEN 14
#define DEFAULT_COLOR 7  
typedef enum {BLACK, BLUE, GREEN, TURQ, RED, PURPLE, YELLOW, WHITE} COLOR;

///read in strings and process strings
char* cmd_gets(char *s, int n, FILE *stream);
char* s_cmdgets(char *st, int n);
char* readByWord(char *str_input, char *word);
char* strheadcmp(char* str1, char* str2);
short argnumCounter(char* argstr);

///process errors
void ErrReport_argnum(char* functionName, short argnumber, short actualargnumber);
void ErrReport_cmdNotExist(void);
void ErrReport_illegalExpr(char* expr);
void ErrReport_callingOrder(char* funcName_prev, char* funcName_post);
void ErrReport_currSupport(char* currName);

///change color
void fontcolor(COLOR color, short highlight) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color | (highlight<<3));
}
void defaultcolor(void) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), DEFAULT_COLOR);
}


/**Implementation of functions
  *
  */

char* readByWord(char *str_input, char *word) {
	char* temp = str_input;
	if (word) {
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
	} else { //word == NULL  
		while (*temp != ' ' && *temp) temp++;   //'\0' == 0
		return *temp ? temp+1 : temp; 
	}
}

char* strheadcmp(char* str1, char* str2) {
	while (*str1 && *str2) {
		if (*str1 == *str2) {
			str1++;
			str2++;
		} else {
			return NULL;
		}
	}
	return ((*str1==' '||*str1=='\0') ? (*str1 ? str1+1 : str1) : NULL);
}

char* s_cmdgets(char *st, int n) {
	register char* ret_val;
	register char* find;

	ret_val = cmd_gets(st, n, stdin);
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

char* cmd_gets(char *s, int n, FILE *stream) {
	register int c;
	register char *cs;
	cs = s;

	while(--n > 0 && (c = tolower(getc(stream))) != EOF) { //no difference if you use CAPITALS
		if (c == ' ' && (*(cs-1) == ' '||cs == s)) {//always one ' ' no matter how many ' ' there are
			++n;								    //and the first char stored will never be ' '
			continue;
		} else if((*cs++ = c) == '\n') {
			if (*(cs-2) == ' ') {				//dispose of the trailing ' '
				*(--cs-1) = '\n';
			}
			break;
		}
	}

	*cs = '\0';
	return (c == EOF && cs == s) ? NULL : s ;
}

short argnumCounter(char* argstr) { //actually just count the number of ' '
	if (*argstr) {
		short count = 1;
		while (*(++argstr)) {
			if (*argstr == ' ') {
				++count;
			}
		}
		return count;
	} else {
		return 0;
	}
}


/**
 *  I refered to the style of GHCI (interactive haskell) to design my style of error report (red "error" & big "?")
 *  BTW haskell is an interesting language (though i can merely do helloworld with it)
 */
 
void ErrReport_argnum(char* functionName, short argnumber, short actualargnumber) {
	if (argnumber > actualargnumber) {
		fontcolor(RED, 0); 		printf("error: ");
		fontcolor(WHITE, 1);	printf("too few ( ");
		fontcolor(YELLOW, 0);	printf("%d", actualargnumber);
		fontcolor(WHITE, 1);	printf(" ) arguments to function ");
	} else if (argnumber < actualargnumber) {
		fontcolor(RED, 0);		printf("error: ");
		fontcolor(WHITE, 1);	printf("too many ( ");
		fontcolor(YELLOW, 0);	printf("%d", actualargnumber);
		fontcolor(WHITE, 1);	printf(" ) arguments to function ");
	} else {
		return;
	}
	fontcolor(TURQ, 0);		puts(functionName);
	fontcolor(WHITE, 1);	printf("     ? Function ");
	fontcolor(TURQ, 0);		printf(functionName);
	fontcolor(WHITE, 1);	printf(" needs ");
	fontcolor(YELLOW, 0);	printf("%d", argnumber);
	fontcolor(WHITE, 1);	printf(" argument\n");
}

void ErrReport_cmdNotExist(void) {
	fontcolor(RED, 0); 		printf("error: ");
	fontcolor(WHITE, 1); 	printf("Command doesn't exist!\n     ? Type ");
	fontcolor(TURQ, 0); 	printf("help");
	fontcolor(WHITE, 1); 	puts(" to view all the command available.\n");
}

void ErrReport_illegalExpr(char* expr) {
	fontcolor(RED, 0); 		printf("error: ");
	fontcolor(WHITE, 1);    printf("Can't calculate illegal expression \"");
	fontcolor(YELLOW, 0);   printf(expr);
	fontcolor(WHITE, 1);	puts("\"\n     ? Try again with a proper expression!");
}

void ErrReport_callingOrder(char* funcName_prev, char* funcName_post) {
	fontcolor(RED, 0); 		printf("error: ");
	fontcolor(WHITE, 1);	printf("you are trying to call ");
	fontcolor(TURQ, 0);		printf(funcName_post);
	fontcolor(WHITE, 1);	printf(" before ");
	fontcolor(TURQ, 0);		printf(funcName_prev);
	fontcolor(WHITE, 1); 	printf(" is called!\n     ? Please call ");
	fontcolor(TURQ, 0);		printf(funcName_prev);
	fontcolor(WHITE, 1);	puts(" first.");
}

void ErrReport_currSupport(char* currName) {
	fontcolor(RED, 0); 		printf("error: ");
	fontcolor(WHITE, 1);    printf("Currency \"");
	fontcolor(YELLOW, 0);   printf(currName);
	fontcolor(WHITE, 1);	printf("\" not found.\n     ? Support ");
	fontcolor(YELLOW, 0);	printf("usd rmb jpy");
	fontcolor(WHITE, 1);	puts(" only.");
}
