#include "MathIsFun.h"
#include <stdbool.h>
#include <stdio.h>

/*
 * Calculates the largest integer less or equal than the square root of x.
 * funSqrt(10) = 3
 * funSqrt(16) = 4
 * @param x - An integer for which the function applies
 * @return the value of |_sqrt(x)_|, if x is negative then the function
 * returns -1.
 */
int funSqrt(int x);

int funPow(int x, int n, int d) {
	int result = 0;
	if(d==1)
		return 0; //for any x and n, (x^n)mod1 = 0
	if(n==0)
		return 1%d;
	while(x<0)
		x+=d;
	if(n==1)
		return x%d;
	if (n%2==0){
		result= funPow(x,n/2,d);
		return ((result*result)%d);
	}
	else{
		result= funPow(x,(n-1)/2,d);
		return (((result*result)*(x%d))%d);
	}
}

int funSqrt(int x) {
	int low = 0, high = x + 1, mid = 0;
	if(x < 0)
		return -1;
	while(high - low > 1){
		mid = (low + high) / 2;
		if((mid*mid <= x) && (x <= (mid+1)*(mid+1)))
			return (((mid+1)*(mid+1) == x) ? (mid+1) : mid);
		if(mid*mid < x)
			low = mid;
		else
			high = mid;
	}
	return mid;
}

/*
 * Checks if x is a prime
 * @param x - the number to be checked
 * @return - true if x is a prime, false otherwise
 */
bool funPrimeCheck(int x) {
	bool res = true; //for now "x is prime"
	int sqrt_x = funSqrt(x);
	if(x<=1)
		return false;
	for(int i=2; i<=sqrt_x; i++)
		if(x%i == 0)
			res = false; //i is a divisor of x, therefore x is not prime
	return res;
}

/*
 * Checks if x is a palindrome.
 * A palindromic number is a number that is the same when written forwards or backwards.
 * For example:
 * 11, 121, 1331 are all palindromic numbers.
 * 112, 124, 331 are not palindromic numbers.
 * @param x - the number to be checked
 * @return - true if x is a palindrome, false otherwise
 */
bool funPalindromeCheck(int x) {
	if(x<0){
		return false;
	}
	int y=0,z=x;
	while(z>0){
		y*=10;
		y+=z%10;
		z/=10;
	}
	if(y==x){
		return true;
	}
	return false;
}
