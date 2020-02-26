/**
 * @wlx
 * commandline caculator
 * basic function:elecFee feeDetail toCURRENCY quit help    *calculate simple expression*
 * it is a beta version..
 * finished on 2019.11.13
 */
 
#pragma c11 on //make sure your gcc version higher than 4.7
#include "interactive.h"
#include "SYA_Calc.h"   //need C11 feature

#define RMBTOUSD 0.1415
#define RMBTOJPY 15.3779
typedef enum {ERRORNAME, RMB, USD, JPY} MYCURRENCY;

void cmdInterpreter(char* readLine, double * const feeptr, double * const elecptr, unsigned short * const flagptr);
void help(void);
void titleLog(void);
void demandInput(void) {fontcolor(PURPLE, 0); printf("Input> "); defaultcolor();} //print a purple "Input>"
double elecFee(double elec);
void feeDetail(double elec, double fee);
double toCURRENCY(double, MYCURRENCY);
MYCURRENCY strtoMYCURRENCY(char* str_temp);


int main(void) {
	char str_temp[MAXSIZE] ="";
	double elec = 0, feeRMB = 0;
	unsigned short callingflag = 0; // 1: elecfee 2:   4:   8:
    
	titleLog();
	demandInput();
	while (s_cmdgets(str_temp, MAXSIZE)) { //read-in circle
		if (*str_temp) {
			cmdInterpreter(str_temp, &feeRMB, &elec, &callingflag); ///it is UGLY, I'm just too lazy to change it
		}
		demandInput();
	}

	return 0;
}

///lazy again... making errReport and calculation tied together may make this much more clear
//basically it's simply a bunch of if-else 
void cmdInterpreter(char* str_temp, double * const ptrtofeeRMB, double * const ptrtoelec, unsigned short * const ptrtoflag) {
	short argnumber = 0;
	fontcolor(WHITE, 1);
	if (isdigit(*str_temp) || *str_temp == '(') { //normal calculator
		double result;
		if (!Calc(str_temp, &result)) {
			printf("%g\n", result);
		}
	} else if (isalpha(*str_temp)) { //strheadcmp to choose function
		if (strheadcmp(str_temp, "elecfee")) {
			str_temp = readByWord(str_temp, NULL);	    //strcpy(str_temp, strheadcmp(str_temp, "feedetail"));

			if ((argnumber=argnumCounter(str_temp)) == 1) {
				double result;
				if (!Calc(str_temp, &result)) { 
					printf("%.1f RMB\n", *ptrtofeeRMB = elecFee(*ptrtoelec = result)); 
					*ptrtoflag |= 1;
				}
			} else {
				ErrReport_argnum("elecfee", 1, argnumber);
			}
		} else if (strheadcmp(str_temp, "feedetail")) {
			str_temp = readByWord(str_temp, NULL);		
			
			if (argnumber = argnumCounter(str_temp)) {
				ErrReport_argnum("feedetail", 0, argnumber);
			} else {
				if (*ptrtoflag&1) {
					feeDetail(*ptrtoelec, *ptrtofeeRMB);
				} else {
					ErrReport_callingOrder("elecfee", "feedetail");
				}
			}
		} else if (strheadcmp(str_temp, "tocurrency")) {
			str_temp = readByWord(str_temp, NULL);

			if ((argnumber = argnumCounter(str_temp)) == 1) {
				if (*ptrtoflag&1) {
					MYCURRENCY targetCurrency;

					if (targetCurrency = strtoMYCURRENCY(str_temp)) { //targetCurrency != ERRORNAME
						printf("%.1f\n", toCURRENCY(*ptrtofeeRMB, targetCurrency));
					} else {
						ErrReport_currSupport(str_temp);
					}

				} else {
					ErrReport_callingOrder("elecfee", "tocurrency");
				}
			} else {
				ErrReport_argnum("tocurrency", 1, argnumber);
			}

		} else if (strheadcmp(str_temp, "quit")) {
			str_temp = readByWord(str_temp, NULL);

			if (argnumber = argnumCounter(str_temp)) {
				ErrReport_argnum("quit", 0, argnumber);
			} else {
				fontcolor(YELLOW, 0);
				puts("Bye!");
				defaultcolor();
				Sleep(3360);
				exit(0);
			}
		} else if (strheadcmp(str_temp, "help")) {
			str_temp = readByWord(str_temp, NULL);

			if (argnumber = argnumCounter(str_temp)) {
				ErrReport_argnum("help", 0, argnumber);
			} else {
				help();
			}
		} else {
			ErrReport_cmdNotExist();
		}
	} else {
		ErrReport_cmdNotExist();
	}
}

double elecFee(double elec) {
	if (elec < 0) {
		return -1;
	} else if (elec <= 2760) {
		return 0.538*elec;
	} else if (elec <= 4800) {
		return 0.538*2760+(elec-2760)*0.588;
	} else {
		return 0.538*2760+(4800-2760)*0.588+(elec-4800)*0.838;
	}
}

void feeDetail(double elec, double fee) {
	if (elec < 0) {
		puts("ERROR!");
	} else if (elec <= 2760) {
		printf("%.1f * 0.538 = %.1f RMB\n", elec, fee);
	} else if (elec <= 4800) {
		printf("2760 * 0.538 + (%.1f-2760) * 0.588 = %.1f RMB\n", elec, fee);
	} else {
		printf("2760 * 0.538 + (4800-2760) * 0.588 + (%.1f-4800) * 0.838 = %.1f RMB\n", elec, fee);
	}
}

MYCURRENCY strtoMYCURRENCY(char* str_temp) {
	if (!strcmp(str_temp, "usd"))
		return USD;
	else if (!strcmp(str_temp, "jpy"))
		return JPY;
	else if (!strcmp(str_temp, "rmb"))
		return RMB;
	else
		return ERRORNAME;
}

double toCURRENCY(double feeRMB, MYCURRENCY currency) {
	switch (currency) {
		case USD: return feeRMB*RMBTOUSD; break;
		case JPY: return feeRMB*RMBTOJPY; break;
		case RMB: return feeRMB; break;
	}
}


void help(void) {
	puts("   Commands available(not case sensitive):");
	puts("");
	puts("   <statements>                      evaluate <statement>");
	puts("   elecfee     <double>              show the electric fee (in RMB)");
	puts("   feedetail   *void*                show the detail of last calculation");
	puts("   tocurrency  <enum{RMB,USA,JPY}>   show the fee in the currency chosen");
	puts("   quit        *void*                exit the calculator");
	puts("");
}

void titleLog(void) {
	fontcolor(WHITE, 1);
	puts("It's a commandline calculator.");
	puts("Basically it can calculate the electric fee in Hangzhou or do some simple calculations.");
	printf("Type ");
	fontcolor(TURQ, 0); 	printf("help");
	fontcolor(WHITE, 1); 	printf(" to view all the command available.\n");
}
