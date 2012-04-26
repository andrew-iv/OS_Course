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
const int new_pgid=31337;
int main()
{
	while(1)
	{
		int f =fork();
		if(f<0)
		{
			fprintf(stderr, "Couldn't fork: %s\n", strerror(errno));	
		}
		if(f>0)
		{
			int err = setpgid(f,new_pgid);
			if(err == -1 )
			{
			fprintf(stderr, "Couldn't setpgid: %s\n", strerror(errno));		
			continue;
			}
			err= getpgid(f);
			if(err == -1)
			{
				fprintf(stderr,"Couldn;t getpgid: %s\n", strerror(errno));
			}
			printf("%d\n",err);

		}
	}
return 0;
}
