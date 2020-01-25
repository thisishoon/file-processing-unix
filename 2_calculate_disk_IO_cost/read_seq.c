#include <stdio.h>
#include <stdlib.h>
#include "record.h"
#include <time.h>
#include <sys/time.h>

int main(int argc, char **argv)
{

	if(argc != 3) exit(1);
	
	int recordNum = atoi(argv[2]);
	FILE* fstream;
	fstream = fopen(argv[1],"r+");

	char buff[250];
	struct timeval start, end;
	long result=0;
	
	gettimeofday(&start,NULL);
	for(int i=0; i<recordNum; i++){
		fread(buff, 250, 1, fstream);
	}
	gettimeofday(&end,NULL);
	
	result =  (  1000000 * (end.tv_sec - start.tv_sec) )  + (end.tv_usec-start.tv_usec);

	printf("%ld us",result);
	fclose(fstream);
	


	return 0;
}
