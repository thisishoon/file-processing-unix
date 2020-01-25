#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
    
    char buf[10];
    
    FILE *fp1;
    
    

    fp1 = fopen(argv[1] , "a"); //붙여쓰는 목적으로
    
    
        fseek(fp1,0,SEEK_END); //파일 포인터를 마지막으로 이동시킨 뒤
	
	int size = strlen(argv[2]); //데이터의 크기(10바이트)를 읽어온 뒤

	fwrite(argv[2],1,size,fp1); //크기만큼 붙여쓰기
       
    
    fclose(fp1);
    
    
    
    
    
    
    
    
    return 0;
}



