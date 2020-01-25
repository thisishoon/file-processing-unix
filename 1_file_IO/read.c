#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    
    char buf[100];
    
    FILE *fp1, *fp2;
    int  offset, size;
    

    fp1 = fopen(argv[1] , "r");  //읽는 목적으로
    offset = atoi(argv[2]);      //문자열을 정수형으로 변환
    
        fseek(fp1,offset,SEEK_SET); //offset만큼 포인터를 이동시킨 뒤
	size=fread(&buf,1,10,fp1);  //10바이트만큼 읽고
        for(int i=0; i<size; i++)   //10바이트동안 반복
	printf("%c",buf[i]);
       
    
    fclose(fp1);
    
    
    
    
    
    
    
    
    return 0;
}


