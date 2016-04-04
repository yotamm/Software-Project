#include <stdio.h>
#include "MathIsFun.h"

int main(void){
	int user_choice, x, n, d, val;
	printf("Welcome to Math Is Fun - beta version\n");
	printf("Supported operations are:\n");
	printf("1 - Power Calculation\n");
	printf("2 - Prime Check\n");
	printf("3 - Palindrome Check\n");
	printf("Please enter operation number (1/2/3): \n");
	scanf("%d",&user_choice);
	switch (user_choice){
		case 1:
			printf("Please enter three space separated numbers: \n");
			scanf("%d %d %d",&x, &n, &d);
			val = funPow(x,n,d);
			printf("res = %d\n", val);
			break;
		case 2:
			printf("Please enter an integer: \n");
			scanf("%d",&x);
			val = funPrimeCheck(x);
			printf("res = %s\n", (val ? "true" : "false"));
			break;
		case 3:
			printf("Please enter an integer: \n");
			scanf("%d",&x);
			val = funPalindromeCheck(x);
			printf("res = %s\n", (val ? "true" : "false"));
			break;
		default:
			break;
	}
	return 0;
}
