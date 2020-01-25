#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include "record.h"
#include <sys/time.h>
//필요하면 header file 추가 가능


#define SUFFLE_NUM	10000	// 이 값은 마음대로 수정할 수 있음

void GenRecordSequence(int *list, int n);
void swap(int *a, int *b);
// 필요한 함수가 있으면 더 추가할 수 있음

//
// argv[1]: 레코드 파일명
// argv[2]: 레코드 파일에 저장되어 있는 전체 레코드 수
//
int main(int argc, char **argv)
{
	if(argc!=3) exit(1);
	int *read_order_list;
	int num_of_records;
	num_of_records=atoi(argv[2]);
	
	FILE* fstream;
	fstream = fopen(argv[1], "r+");

	read_order_list = (int *)calloc(num_of_records, sizeof(int));

	
	GenRecordSequence(read_order_list, num_of_records);
	
	
	struct timeval start, end;
	char buff[250];
	long  result;

	gettimeofday(&start,NULL);
	for(int i=0; i<=num_of_records; i++){
		fseek(fstream, read_order_list[i]*250, SEEK_SET);
		fread(buff, 250, 1, fstream);
	}
	gettimeofday(&end,NULL);
	result =  ( 1000000 * (end.tv_sec - start.tv_sec) ) + (end.tv_usec - start.tv_usec) ;
	printf("%ld us",result);
	fclose(fstream);

	return 0;
}

void GenRecordSequence(int *list, int n)
{
	int i, j, k;

	srand((unsigned int)time(0));

	for(i=0; i<n; i++)
	{
		list[i] = i;
	}
	
	for(i=0; i<SUFFLE_NUM; i++)
	{
		j = rand() % n;
		k = rand() % n;
		swap(&list[j], &list[k]);
	}

	return;
}

void swap(int *a, int *b)
{
	int tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;

	return;
}

