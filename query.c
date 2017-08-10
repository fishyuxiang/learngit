/*
 ============================================================================
 Name        : log.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include<time.h>

#include "dnsfunction.h"

int main(int argc,char ** argv) {
	BSTree T;
	T=NULL;
	T=createBST(argv[2],argv[3],T);
	reslines=0;
	inordertraverse(T);
	quicksort(0,reslines-1);
	show(atoi(argv[1]));
	return 0;
}
