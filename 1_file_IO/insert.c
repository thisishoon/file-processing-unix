#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

FILE *fp1;  char temp[10]; char*  buf;
 int size=0;

fp1 = fopen(argv[1] , "r+"); //읽고 쓰기위한 목적으로

int offset = atoi ( argv[2] ); //offset 문자열을 정수형으로 변환

fseek(fp1,0,SEEK_END);    //배열의 마지막 위치를 읽어옴
    
size = ftell(fp1) - offset ;  //복사하기위한 길이 구하기

buf = malloc(sizeof(char) * size);     //버퍼 생성

fseek(fp1,offset,SEEK_SET);        //offset부터 

fread(buf,1,size,fp1);       //구한 길이만큼 읽어서 버퍼에 저장

fseek(fp1,offset,SEEK_SET);    //off셋부터

fwrite(argv[3],1,10,fp1);    //data를 쓰고

fwrite(buf,1,size,fp1);   // 버퍼에 저장되어있던 data를 쓴다

free(buf);

fclose(fp1);


return 0;
}
