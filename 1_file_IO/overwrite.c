#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){

FILE *fp1;

int offset;

fp1 = fopen(argv[1] , "r+");  //읽고 쓰는 목적으로

offset = atoi( argv[2] ); // 입력받은 offset 문자열을 정수형으로 변환

fseek(fp1,offset,SEEK_SET); //파일 포인터를 offset으로 이동시킨 뒤

int size = strlen
    
fwrite(argv[3],1,10,fp1); //데이터(10바이트)를 쓰기

fclose(fp1);





return 0;
}
