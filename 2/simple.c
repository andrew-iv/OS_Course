/*
 * =====================================================================================
 *
 *       Filename:  simple.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  24.04.2012 03:08:14
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Andrey Ivanov
 *        Company:  Ural State University
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <unistd.h>
#include<errno.h>
int main()
{
	while(1)
	{
		if(fork()<0)
		fprintf(stderr, "Couldn't fork: %s\n", strerror(errno));	
	}
return 0;
}
